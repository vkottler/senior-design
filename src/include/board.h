#ifndef _BOARD__H__
#define _BOARD__H__

#include <stdbool.h>

#define USB_UART USART2

int io_init(void);
int periph_init(void);
void blink_handler(unsigned int blink_int);

#endif
