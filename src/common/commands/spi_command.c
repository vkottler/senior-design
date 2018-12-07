#include "command.h"
#include "stateful_spi.h"
#include "gpio.h"
#include "board.h"

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#define LARGEST_SPI_TRANSACTION 8

void command_transaction_done(const uint8_t *buffer, size_t len)
{
    size_t i;
    for (i = 0; i < len; i++)
        printf("0x%x ", buffer[i]);
}

void command_reset_spi_cs(void)
{ gpio_resetPin(GPIOB, 12); }

void command_set_spi_cs(void)
{ gpio_setPin(GPIOB, 12); }

command_status do_spi(int argc, char *argv[])
{
    spi_transaction_t transaction;
    uint8_t write_buffer[LARGEST_SPI_TRANSACTION],
            read_buffer[LARGEST_SPI_TRANSACTION];
    int num_bytes = argc - 1, i;

    /* make sure the command fits into the buffer(s) */
    if (num_bytes > LARGEST_SPI_TRANSACTION)
    {
        printf("transaction is too big\r\n");
        return FAIL;
    }

    /* initialize buffers */
    for (i = 0; i < num_bytes; i++)
    {
        write_buffer[i] = strtol(argv[i + 1], NULL, 16);
        read_buffer[i] = 0x0;
    }

    /* initialize and start the transaction */
    spi_init_transaction(&transaction, write_buffer, read_buffer,
                         num_bytes, num_bytes, command_set_spi_cs,
                         command_reset_spi_cs, command_transaction_done);
    if (!spi_start_transaction(&spi2_state, &transaction))
    {
        printf("transaction couldn't start\r\n");
        return FAIL;
    }

    /* wait for transaction to complete or error */
    if (!spi_wait_on_transaction(&transaction))
        return FAIL;

    printf("\r\n");

    dump_transaction(&transaction);

    return CMD_SUCCESS;
}


COMMAND_ENTRY("spi", "spi byte1 [byte2,...]",
              "Interact with the SPI sub-system.", do_spi)

bool write_byte_blocking(uint8_t byte)
{
    uint32_t timeout_time = ticks + 10;
    while (!LL_SPI_IsActiveFlag_TXE(SPI1) && ticks < timeout_time) {;}
    if (ticks >= timeout_time)
        return false;
    SPI1->DR = byte;
    SPI1->DR = 0x00;
    return true;
}

bool read_byte_blocking(uint8_t *byte)
{
    uint8_t temp;
    uint32_t timeout_time = ticks + 10;
    while (!LL_SPI_IsActiveFlag_RXNE(SPI1) && ticks < timeout_time) {;}
    if (ticks >= timeout_time)
        return false;
    temp = SPI1->DR;
    *byte = temp;
    return true;
}

int poll_read_dump_byte(void)
{
    uint8_t byte;
    if (LL_SPI_IsActiveFlag_RXNE(SPI1))
    {
        byte = SPI1->DR;
        printf("0x%x ", byte);
        return 1;
    }
    return 0;
}

command_status do_spi_old(int argc, char *argv[])
{
    int i, num_read = 1;
    uint8_t byte;
    uint32_t rx_fifo_thresh = LL_SPI_GetRxFIFOThreshold(SPI1), rx_it, tx_it, err_it;

    /* wait for pending IO on this interface to finish */
    delay(100);

    /* disable interrupts */
    rx_it = LL_I2S_IsEnabledIT_RXNE(SPI1);
    tx_it = LL_I2S_IsEnabledIT_TXE(SPI1);
    err_it = LL_I2S_IsEnabledIT_ERR(SPI1);
    LL_SPI_DisableIT_RXNE(SPI1);
    LL_SPI_DisableIT_TXE(SPI1);
    LL_I2S_DisableIT_ERR(SPI1);

    /* set one-quarter as the threshold for this command to work */
    LL_SPI_SetRxFIFOThreshold(SPI1, LL_SPI_RX_FIFO_TH_QUARTER);

    LL_SPI_Enable(SPI1);
    for (i = 1; i < argc; i++)
    {
        byte = strtol(argv[i], NULL, 16);
        if (LL_SPI_IsActiveFlag_TXE(SPI1))
            SPI1->DR = byte;
        else while (!LL_SPI_IsActiveFlag_TXE(SPI1)) {;}
    }

    /* read the rest of the bytes */
    while (num_read < argc)
        num_read += poll_read_dump_byte();

    LL_SPI_Disable(SPI1);

    printf("\r\n");

    /* set the threshold back */
    LL_SPI_SetRxFIFOThreshold(SPI1, rx_fifo_thresh);

    /* re-enable interrupts if they were set */
    if (rx_it)
        LL_SPI_EnableIT_TXE(SPI1);
    if (tx_it)
        LL_SPI_EnableIT_RXNE(SPI1);
    if (err_it)
        LL_SPI_EnableIT_ERR(SPI1);

    return CMD_SUCCESS;
}
