#pragma once

#include "usart.h"
#include "esc.h"

#define MAX_TOKENS	16

#ifndef NUCLEO
#define CLI_INPUT_BUF get_rx(USART1)
#else
#define CLI_INPUT_BUF get_rx(USART2)
#endif

extern const ESC_TypeDef ESC_TABLE[];

void printPrompt(void);
void check_input(void);
void process_input(char *buffer);
