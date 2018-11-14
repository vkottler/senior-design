#ifndef UART_H
#define UART_H
#include <termios.h>

int uart_filestream;

int uart_write(int uart, unsigned char * buff, int size);
int uart_read(int uart, char * buff);
int uart_config(speed_t baud);

#endif /* UART_H */
