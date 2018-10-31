#ifndef UART_H
#define UART_H

int uart_write(int uart, unsigned char * buff, int size);
int uart_read(int uart, char * buff);
int uart_config();

#endif /* UART_H */
