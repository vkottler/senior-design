#include "command.h"
#include "stateful_spi.h"
#include "gpio.h"
#include "board.h"

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#define LARGEST_SPI_TRANSACTION 8

void command_transaction_done(const uint8_t *buffer, size_t len)
{ for (size_t i = 0; i < len; i++) printf("0x%x ", buffer[i]); }

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
                         num_bytes, num_bytes, command_set_spi1_cs,
                         command_reset_spi1_cs, command_transaction_done);
    if (!spi_start_transaction(&spi1_state, &transaction))
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
