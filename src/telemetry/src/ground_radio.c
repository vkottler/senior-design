#include <gpio.h>
#include <uart.h>
#include <ground_radio.h>

int radio_config()
{
  INP_GPIO(mode_0);
  OUT_GPIO(mode_0);
  INP_GPIO(mode_1);
  OUT_GPIO(mode_1);

/*  radio_set_mode(SLEEP);*/
  radio_set_mode(NORMAL);

  return 0;
}

int radio_set_param()
{
  radio_set_mode(SLEEP);
  unsigned char  buff[6];
  buff[0] = 0xC0;
  buff[1] = 0x00;
  buff[2] = 0x00;
  buff[3] = 0x3f; // 115200
  buff[4] = 0x17;
  buff[5] = 0x44;
//c0 0 0 1a 6 44
  uart_write(uart_filestream, buff, 6);
}
int radio_print_param()
{
  radio_set_mode(SLEEP);

  unsigned char  buff[3];
  buff[0] = 0xC1;
  buff[1] = 0xC1;
  buff[2] = 0xC1;

  uart_write(uart_filestream, buff, 3);

  return 0;
}

int radio_print_ver()
{
  radio_set_mode(SLEEP);

  unsigned char  buff[3];
  buff[0] = 0xC3;
  buff[1] = 0xC3;
  buff[2] = 0xC3;

  uart_write(uart_filestream, buff, 3);
  return 0;
}


int radio_set_mode(radio_mode_type mode)
{
  switch(mode)
  {
    case NORMAL:
      GPIO_CLR = 1<<mode_0;
      GPIO_CLR = 1<<mode_1;
      break;
    case WAKE_UP:
      GPIO_SET = 1<<mode_0;
      GPIO_CLR = 1<<mode_1;
      break;
    case POWER_SAVING:
      GPIO_CLR = 1<<mode_0;
      GPIO_SET = 1<<mode_1;
      break;
    case SLEEP:
      GPIO_SET = 1<<mode_0;
      GPIO_SET = 1<<mode_1;
      break;
    default:
      break;
  }
  return 0;
}

int radio_set_parm(char* param)
{
  param[0] = 0xC2;

  uart_write(uart_filestream, param, 6);
}

int radio_set_baud(radio_baud_type baud)
{
  char * param[6];

  param[3] = (char*)baud;
  radio_set_parm((char*)param);
}
