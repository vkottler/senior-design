#ifndef _CLI__H__
#define _CLI__H__
#include "esc.h"

void printPrompt(void);
void check_input(void);
void process_input(char *buffer);

extern PC_Buffer *tx_buf[3], *rx_buf[3];
extern const ESC_TypeDef ESC_TABLE[];
#endif

