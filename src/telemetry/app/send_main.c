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
  int uart_filestream = uart_config();
  char c;

/*  uart_write(uart_filestream, "Hello world\r\n", 13);*/
  unsigned char  buff[3];
  buff[0] = 0xc3;
  buff[1] = 0xc3;
  buff[2] = 0xc3;

  uart_write(uart_filestream, buff, 3);

  while(1)
  {
    c = fgetc(stdin);
    uart_write(uart_filestream, (char*)&c, 1);
  }

  close(uart_filestream);
}
