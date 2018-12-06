#ifndef _USART__H__
#define _USART__H__

#include <stdio.h>
#include <stdbool.h>
#include "stm32f303xe.h"
#include "pcbuffer.h"

#define NUM_UARTS       3

#define USART_BUF		2048
#define LIDAR_BUF	    9	
#define USART_INT_PRIO	4

#define RADIO_SILENCE_BYTE  0xAA
#define RADIO_SILENCE_TICKS 2000

#define USB_UART	USART3

typedef enum {
	APBX =		0,
	SYSCLK =	1,
	LSE_SRC =	2,
	HSI_SRC =	3
} usart_clk_src_t;

int usart_config(
	USART_TypeDef* usart, usart_clk_src_t src, uint32_t control[3],
	uint32_t baud, bool ie
);

int _getc(USART_TypeDef* usart, bool block, char *c);
int _putc(USART_TypeDef* usart, bool block, char data);
PC_Buffer *get_tx(USART_TypeDef* usart);
PC_Buffer *get_rx(USART_TypeDef* usart);

extern PC_Buffer *tx_buf[NUM_UARTS], *rx_buf[NUM_UARTS];
extern volatile uint32_t radio_resume;
volatile bool radio_transmit_state;

#endif
