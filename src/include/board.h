#ifndef _BOARD__H__
#define _BOARD__H__

#include <stdbool.h>
#include <stdint.h>
#include "handlers.h"

#include "channels.h"

#define RADIO_FD 55

void io_init(void);
int periph_init(void);
void delay(uint32_t ms);
void blink_handler(unsigned int blink_int);
void telem_handler(unsigned int interval);

void command_reset_spi1_cs(void);
void command_set_spi1_cs(void);

#endif
