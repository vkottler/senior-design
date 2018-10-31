#include "../include/uart.h"
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <termios.h>
#include <string.h>
#include <gpio.h>
#include <ground_radio.h>

int main ()
{
  char rx_buffer[20];
  int uart_filestream = uart_config();
  int length = 0;
  int i = 0;

  gpio_config();
  INP_GPIO(19);
  OUT_GPIO(19);
  INP_GPIO(13);
  OUT_GPIO(13);

  // Sleep mode
  GPIO_SET = 1<<19;
  GPIO_SET = 1<<13;
/*  radio_config(uart_filestream);*/
  unsigned char  buff[3];
  buff[0] = 0xc1;
  buff[1] = 0xc1;
  buff[2] = 0xc1;

  uart_write(uart_filestream, buff, 3);
/*  uart_write(uart_filestream, "Hello world\r\n", 13);*/
  while(1)
  {
    length = uart_read(uart_filestream, rx_buffer);
    if (length > 0)
    {
      printf("0x");
      for (i = 0; i < length; i ++)
      printf("%x", *rx_buffer);
      printf("\n");
    }
  }

  close(uart_filestream);
}
