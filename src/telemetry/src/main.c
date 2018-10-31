#include "../include/uart.h"
#include <fcntl.h>			//Used for UART

int main ()
{
  unsigned char rx_buffer[20];
  int uart_filestream = uart_config();
  int length = 0;
  uart_write(uart_filestream, "HELLO world\r\n");
  while(1)
  {
    length = uart_read(uart_filestream, rx_buffer);
    if (length > 0) printf("%s", rx_buffer);
  }

  close(uart_filestream);
}
