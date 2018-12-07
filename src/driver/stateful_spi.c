#include "stateful_spi.h"

#include <stdio.h>

spi_state_t spi1_state, spi2_state;

/*
 * Handle a write event for a spi transaction on a specific interface.
 */
static void spi_handle_write(SPI_TypeDef *interface,
                      spi_transaction_t *transaction)
{
    size_t remaining = transaction->to_write - transaction->written;

    /* prevent future write attempts in error state */
    if (transaction->state == SPI_TRANSACTION_ERROR)
        LL_SPI_DisableIT_TXE(interface);

    /* write byte(s) */
    if (remaining == 1)
        *((uint8_t *) &interface->DR) = transaction->write_buffer[transaction->written++];
    else
    {
        interface->DR = ((const uint16_t *) transaction->write_buffer)[transaction->written / 2];
        transaction->written += 2;
    }

    /* done writing, prevent future calls until the next transaction */
    if (transaction->written == transaction->to_write)
        LL_SPI_DisableIT_TXE(interface);
}

inline bool spi_writing_done(spi_transaction_t *transaction)
{ return transaction->written >= transaction->to_write; }

/*
 * Handle a read event for a spi transaction on a specific interface.
 */
static void spi_handle_read(SPI_TypeDef *interface,
                     spi_transaction_t *transaction)
{
    uint8_t value = *((uint8_t *) &interface->DR);

    /* prevent future read attempts in error state */
    if (transaction->state == SPI_TRANSACTION_ERROR)
        LL_SPI_DisableIT_RXNE(interface);

    /* read byte(s) */
    if (transaction->read < transaction->to_read)
        transaction->read_buffer[transaction->read++] = value;
    else
        transaction->dummy_reads++;

    /* when we have read the same amount of bytes that were written, the
     * overall transaction is complete */
    if (transaction->read + transaction->dummy_reads == transaction->to_write)
    {
        LL_SPI_Disable(interface);
        LL_SPI_DisableIT_RXNE(interface);
        transaction->state = SPI_TRANSACTION_COMPLETE;
        transaction->set_cs();
        transaction->callback(transaction->read_buffer, transaction->read);
    }
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
                          cs_setter set_cs, cs_resetter reset_cs,
                          transaction_done_callback callback)
{
    transaction->to_write = to_write;
    transaction->to_read = to_read;
    transaction->write_buffer = write_buffer;
    transaction->read_buffer = read_buffer;
    transaction->state = SPI_TRANSACTION_INITED;
    transaction->set_cs = set_cs;
    transaction->reset_cs = reset_cs;
    transaction->callback = callback;
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

    transaction->reset_cs();
    transaction->written = 0;
    transaction->read = 0;
    transaction->dummy_reads = 0;
    transaction->state = SPI_TRANSACTION_ACTIVE;
    spi->current_transaction = transaction;

    /* enable the interface */
    LL_SPI_EnableIT_RXNE(spi->interface);
    LL_SPI_EnableIT_ERR(spi->interface);
    LL_SPI_EnableIT_TXE(spi->interface);
    LL_SPI_Enable(spi->interface);

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
}

/*
 * Handle an interrupt event for a spi state.
 */
void spi_state_handler(spi_state_t *spi)
{
    if (LL_SPI_IsActiveFlag_RXNE(spi->interface))
        spi->rx_handle(spi->interface, spi->current_transaction);
    if (LL_SPI_IsActiveFlag_TXE(spi->interface))
        spi->tx_handle(spi->interface, spi->current_transaction);
    else
        spi->err_handle(spi->interface, spi->current_transaction);
}

/* actual symbol that appears in the vector table */
void SPI1_Handler(void) { spi_state_handler(&spi1_state); }
