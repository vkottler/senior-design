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
  char rx_buffer[20];
  uart_filestream = uart_config();
  int length = 0;
  int i = 0;

  gpio_config();
  radio_config();
/*  radio_print_parm();*/

  while(1)
  {
    length = uart_read(uart_filestream, rx_buffer);
    if (length > 0)
    {
      for (i = 0; i < length; i ++) {
        printf("%x", rx_buffer[i]);
        fflush(stdout);
      }
    }
  }

  close(uart_filestream);
}
