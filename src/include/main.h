#ifndef MAIN_H
#define MAIN_H

#include "stm32f303xe.h"
#include <main.h>
#include <gpio.h>
#include <board.h>
#include <pcbuffer.h>
#include <cli.h>

extern PC_Buffer *tx_buf[2], *rx_buf[2];

#endif /* MAIN_H */
