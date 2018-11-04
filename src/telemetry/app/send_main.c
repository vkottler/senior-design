#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <termios.h>
#include <uart.h>
#include <string.h>
#include <gpio.h>
#include <ground_radio.h>

int main ()
{
  uart_filestream = uart_config(B9600);
  char cmd[100];
  int size;

  gpio_config();
  radio_config();

  while(1)
  {
    printf("> ");
    fgets(cmd, 100, stdin);
    if (strcmp(cmd, "param\n") == 0)
    {
      radio_print_param();
    }
    else if (strcmp(cmd, "version\n") == 0)
    {
      radio_print_ver();
    }
    else if (strcmp(cmd, "set param\n") == 0)
    {
      radio_set_param();
    }
    else if (strcmp(cmd, "mode normal\n") == 0)
    {
      radio_set_mode(NORMAL);
    }
    else
    {
      printf("Sending \r\n");
      uart_write(uart_filestream, (char*)cmd, strlen(cmd));
    }
  }

  close(uart_filestream);
}
