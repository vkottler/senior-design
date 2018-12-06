#pragma once

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#include "spi.h"

typedef enum spi_transaction_state {
    SPI_TRANSACTION_INITED   = 0,
    SPI_TRANSACTION_ACTIVE   = 1,
    SPI_TRANSACTION_COMPLETE = 2,
    SPI_TRANSACTION_ERROR    = 3,
} spi_transaction_state_t;

typedef void (*transaction_done_callback)(const uint8_t *buffer, size_t len);

typedef struct spi_transaction {
    size_t to_write, to_read;
    volatile size_t read, written, dummy_reads;
    const uint8_t *write_buffer;
    uint8_t *read_buffer;
    volatile spi_transaction_state_t state;
    transaction_done_callback callback;

    size_t read_calls, write_calls;
} spi_transaction_t;

typedef void (*spi_handle)(SPI_TypeDef *interface,
                           spi_transaction_t *transaction);

typedef struct spi_state {
    spi_handle tx_handle;
    spi_handle rx_handle;
    spi_handle err_handle;
    SPI_TypeDef *interface;
    spi_transaction_t *current_transaction;
} spi_state_t;

extern spi_state_t spi1_state;

void spi_init_state(spi_state_t *spi, SPI_TypeDef *interface);
void spi_init_transaction(spi_transaction_t *transaction,
                          const uint8_t *write_buffer, uint8_t *read_buffer,
                          size_t to_write, size_t to_read,
                          transaction_done_callback callback);

bool spi_transaction_complete(spi_transaction_t *transaction);
bool spi_transaction_error(spi_transaction_t *transaction);
bool spi_start_transaction(spi_state_t *spi, spi_transaction_t *transaction);
bool spi_wait_on_transaction(spi_transaction_t *transaction);
void dump_transaction(spi_transaction_t *transaction);
