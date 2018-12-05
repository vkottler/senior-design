#include "stm32f303xe.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "pcbuffer.h"
#include "cli.h"
#include "accel.h"
#include "esc.h"
#include "timer.h"
#include "usart.h"
#include "command.h"

#define RADIO_RX_BUF rx_buf[0]

char buffer[BUFSIZ];
/* how to print a backspace to the console */
char backspace[3] = {0x08, ' ', 0x08};

int tokenize(char **tok, int max_tokens, char *buf, int buflen) {

	int curr_token = 0, curr_char = 0;
	bool whitespace_flag = true;

	/* continue while we have characters left and space for tokens */
	while (curr_token < max_tokens && curr_char < buflen && buf[curr_char] != '\0') {

		/* previous has been whitespace, we have a new command */
		if (whitespace_flag && buf[curr_char] != ' ') {
			tok[curr_token++] = &buf[curr_char];
			whitespace_flag = false;
		}
		/* end of a command reached, write '\0' here to terminate */
		else if (!whitespace_flag && buf[curr_char] == ' ') {
			buf[curr_char] = '\0';
			whitespace_flag = true;
		}
		curr_char++;
	}

	/* if any unused token spaces remain, indicate this with NULL */
	if (curr_token != max_tokens)
		tok[curr_token] = NULL;

	return curr_token;
}

char *tokens[MAX_TOKENS];
void process_input(char *buf) {

	int num_tokens;

	/* tokenize the input (i.e. 'argv' style array) */
	num_tokens = tokenize(tokens, MAX_TOKENS, buf, USART_BUF);
	if (!num_tokens) return;

	exec_command(num_tokens, tokens);
}

inline void printPrompt(void) {
    printf("=> ");
    fflush(stdout);
}

void check_input(void) {
    if (pc_buffer_messageAvailable(RADIO_RX_BUF)) {
        pc_buffer_getMessage(RADIO_RX_BUF, buffer, 128);
        if (buffer[0] != '\0') {
            process_input(buffer);
        }
        printPrompt();
    }
}

