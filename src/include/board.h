#ifndef _BOARD__H__
#define _BOARD__H__

#include <stdbool.h>
#include <handlers.h>

#define RADIO_FD 55

int io_init(void);
int periph_init(void);
void delay(uint32_t ms);
void blink_handler(unsigned int blink_int);
void telem_handler(unsigned int interval);

#endif
