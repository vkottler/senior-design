#include <gpio.h>
#include <uart.h>

int radio_config(int uart_filestream)
{
  INP_GPIO(19);
  OUT_GPIO(19);
  INP_GPIO(13);
  OUT_GPIO(13);

  // Sleep mode
  GPIO_SET = 1<<19;
  GPIO_SET = 1<<13;

  unsigned char  buff[3];
  buff[0] = 0xa2;
  buff[1] = 0xa2;
  buff[2] = 0xa2;

  uart_write(uart_filestream, buff, 3);

  // Normal mode
/*  GPIO_CLR = 1<<19;*/
/*  GPIO_CLR = 1<<13;*/

  return 0;
}

