#include "stateful_spi.h"

#include <stdio.h>

spi_state_t spi1_state;

/*
 * Handle a write event for a spi transaction on a specific interface.
 */
static void spi_handle_write(SPI_TypeDef *interface,
                      spi_transaction_t *transaction)
{
    uint8_t to_write;
    /* prevent future write attempts in error state */
    //if (transaction->state == SPI_TRANSACTION_ERROR)
    //    LL_SPI_DisableIT_TXE(interface);

    /* write until the transmit buffer is full or we're done writing */
    while (LL_SPI_IsActiveFlag_TXE(interface) &&
           transaction->written < transaction->to_write)
    {
        to_write = transaction->write_buffer[transaction->written++];
        interface->DR = to_write;
    }

    /* stop writing if we've written everything from the buffer */
    //if (transaction->written >= transaction->to_write)
    //    LL_SPI_DisableIT_TXE(interface);

    transaction->write_calls++;
}

inline bool spi_writing_done(spi_transaction_t *transaction)
{ return transaction->written >= transaction->to_write; }

/*
 * Handle a read event for a spi transaction on a specific interface.
 */
static void spi_handle_read(SPI_TypeDef *interface,
                     spi_transaction_t *transaction)
{
    uint8_t dummy;

    /* prevent future read attempts in error state */
    //if (transaction->state == SPI_TRANSACTION_ERROR)
    //    LL_SPI_DisableIT_RXNE(interface);

    /* read until the receive buffer is empty or we're done writing */
    while (LL_SPI_IsActiveFlag_RXNE(interface) &&
           transaction->read < transaction->to_read)
    {
        dummy = interface->DR;
        transaction->read_buffer[transaction->read++] = dummy;
    }

    /* write to a dummy byte if we don't care about these reads */
    if (transaction->read >= transaction->to_read)
    {
        while (LL_SPI_IsActiveFlag_RXNE(interface))
        {
            dummy = interface->DR;
            (void) dummy;
            transaction->dummy_reads++;
        }
    }

    /* if this is the last read, disable the interface and indicate that
     * the transaction is complete  */
    /*
    if (transaction->dummy_reads + transaction->read >= transaction->to_write)
    {
        LL_SPI_DisableIT_RXNE(interface);
        transaction->state = SPI_TRANSACTION_COMPLETE;
        LL_SPI_Disable(interface);
        transaction->callback(transaction->read_buffer, transaction->read);
    }
    */

    transaction->read_calls++;
}

inline bool spi_reading_done(spi_transaction_t *transaction)
{ return transaction->read + transaction->dummy_reads >= transaction->to_write; }

/*
 * Handle an error event for a spi transaction on a specific interface.
 */
static void spi_handle_error(SPI_TypeDef *interface,
                             spi_transaction_t *transaction)
{
    transaction->state = SPI_TRANSACTION_ERROR;
    LL_SPI_DisableIT_RXNE(interface);
    LL_SPI_DisableIT_TXE(interface);
    LL_SPI_DisableIT_ERR(interface);
    printf("SPI transaction error (0x%lx)\r\n", interface->SR);
}

/*
 * Initialize a spi transaction.
 */
void spi_init_transaction(spi_transaction_t *transaction,
                          const uint8_t *write_buffer, uint8_t *read_buffer,
                          size_t to_write, size_t to_read,
                          transaction_done_callback callback)
{
    transaction->to_write = to_write;
    transaction->to_read = to_read;
    transaction->write_buffer = write_buffer;
    transaction->read_buffer = read_buffer;
    transaction->state = SPI_TRANSACTION_INITED;
    transaction->callback = callback;

    transaction->read_calls = 0;
    transaction->write_calls = 0;
}

/*
 * Initialize a spi state.
 */
void spi_init_state(spi_state_t *spi, SPI_TypeDef *interface)
{
    spi->tx_handle = spi_handle_write;
    spi->rx_handle = spi_handle_read;
    spi->err_handle = spi_handle_error;
    spi->interface = interface;
    spi->current_transaction = NULL;
}

/*
 * Start a transaction on a spi interface.
 */
bool spi_start_transaction(spi_state_t *spi, spi_transaction_t *transaction)
{
    /* don't tamper with an ongoing transaction */
    if (spi->current_transaction->state == SPI_TRANSACTION_ACTIVE)
        return false;

    transaction->written = 0;
    transaction->read = 0;
    transaction->dummy_reads = 0;
    transaction->state = SPI_TRANSACTION_ACTIVE;
    spi->current_transaction = transaction;
    LL_SPI_Enable(spi->interface);
    LL_SPI_EnableIT_RXNE(spi->interface);
    LL_SPI_EnableIT_ERR(spi->interface);
    LL_SPI_EnableIT_TXE(spi->interface);
    return true;
}

/* Check if a transaction is complete. */
bool spi_transaction_complete(spi_transaction_t *transaction)
{ return transaction->state == SPI_TRANSACTION_COMPLETE; }

/* Check if an error has occurred for a transaction. */
bool spi_transaction_error(spi_transaction_t *transaction)
{ return transaction->state == SPI_TRANSACTION_ERROR; }

/* Block until transaction completes. */
bool spi_wait_on_transaction(spi_transaction_t *transaction)
{
    while (!spi_transaction_complete(transaction) &&
           !spi_transaction_error(transaction)) {;}
    return !spi_transaction_error(transaction);
}

void dump_transaction(spi_transaction_t *transaction)
{
    printf("to_write: %u, to_read: %u\r\n",
           transaction->to_write, transaction->to_read);
    printf("read: %u, written: %u, dummy_reads: %u\r\n",
           transaction->read, transaction->written, transaction->dummy_reads);

    printf("read calls: %u, write calls: %u\r\n",
           transaction->read_calls,
           transaction->write_calls);
}

/*
 * Handle an interrupt event for a spi state.
 */
void spi_state_handler(spi_state_t *spi)
{
    static int temp = 0;

    spi_transaction_t *tran = spi->current_transaction;

    if (LL_SPI_IsActiveFlag_RXNE(spi->interface))
    {
        /* check done reading */
        if (spi_reading_done(tran))
            LL_SPI_DisableIT_RXNE(spi->interface);
        else spi->rx_handle(spi->interface, tran);
    }
    if (LL_SPI_IsActiveFlag_TXE(spi->interface))
    {
        /* check done writing */
        if (spi_writing_done(tran))
            LL_SPI_DisableIT_TXE(spi->interface);
        else spi->tx_handle(spi->interface, tran);
    }
    else
        spi->err_handle(spi->interface, tran);

    /* check completely done */
    if (spi_writing_done(tran) && spi_reading_done(tran))
    {
        if (temp > 0)
        {
            printf("0x%lx\r\n", spi->interface->SR);
            temp = 0;
            return;
        }
        tran->state = SPI_TRANSACTION_COMPLETE;
        LL_SPI_Disable(spi->interface);
        tran->callback(tran->read_buffer, tran->read);
        temp++;
    }

}

/* actual symbol that appears in the vector table */
//void SPI1_Handler(void) { spi_state_handler(&spi1_state); }
