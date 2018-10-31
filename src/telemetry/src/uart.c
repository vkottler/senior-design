#include <stdio.h>
#include <unistd.h>			//Used for UART
#include <fcntl.h>			//Used for UART
#include <termios.h>		//Used for UAR
#include <string.h>

int uart_write(int uart, char * buff)
{

  if(uart != -1)
  {
    int count = write(uart, buff, strlen(buff));		//filestream, bytes to write, number of bytes to writedd
    if (count < 0)
    {
      close(uart);
      printf("uart tx error\n");
      return -1;
    }
  }
}

int uart_read(int uart, char * buff)
{
  int rx_length = 0;
  if(uart != -1)
  {
    rx_length = read(uart, (void*)buff, 255);		//Filestream, buffer to store in, number of bytes to read (max)
    if (rx_length < 0)
    {
      //An error occured (will occur if there are no bytes)
     // printf("Error negative bytes read\n");
    }
    else if (rx_length == 0)
    {
      //no data waiting
    }
    else
    {
      //Bytes received
      buff[rx_length] = '\0';
    }
  }
  return rx_length;
}

int uart_config()
{
  int uart0_filestream = -1;
  uart0_filestream = open("/dev/ttyAMA0", O_RDWR | O_NOCTTY | O_NDELAY);		//Open in non blocking read/write mode
  if (uart0_filestream == -1)
  {
    printf("Error - Unable to open UART.  Ensure it is not in use by another application\n");
    close(uart0_filestream);
  }
  //baud rate:- B1200, B2400, B4800, B9600
  //CSIZE: CS8
  // IGNPAR = Ignore characters with parity errors

  struct termios options;
  tcgetattr(uart0_filestream, &options);
  options.c_cflag = B9600 | CS8 | CLOCAL | CREAD;		//<Set baud rate
  options.c_iflag = IGNPAR;
  options.c_oflag = 0;
  options.c_lflag = 0;
  tcflush(uart0_filestream, TCIFLUSH);
  tcsetattr(uart0_filestream, TCSANOW, &options);

  return uart0_filestream;
}
