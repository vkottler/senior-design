#include <uart.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <termios.h>
#include <string.h>
#include <gpio.h>
#include <ground_radio.h>

int main ()
{
  char rx_buffer[1024];
/*  uart_filestream = uart_config(B9600);*/
  uart_filestream = uart_config(B115200);
  int length = 0;
  int i = 0;

  gpio_config();
  radio_config();
/*  uart_filestream = uart_config(B115200);*/

  while(1)
  {
    length = uart_read(uart_filestream, rx_buffer);
    if (length > 0)
    {
      for (i = 0; i < length; i ++) {
        printf("%x", rx_buffer[i]);
        fflush(stdout);
      }
      printf("\n");
    }
  }

  close(uart_filestream);
}
