#ifndef _CLI__H__
#define _CLI__H__

void printPrompt(void);
void check_input(void);
void process_input(char *buffer);

extern PC_Buffer *tx_buf[2], *rx_buf[2];

#endif

