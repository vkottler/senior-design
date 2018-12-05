#include <stdio.h>
#include "command.h"
#include <string.h>
#include "board.h"

command_status do_help(int argc, char *argv[]) {

	unsigned int i;

	if (argc == 1) {
		printf("\r\nTry 'help COMMAND' for a command's usage.\r\n\r\n");
		for (i = 0; i < NUM_COMMANDS; i++)
			printf("%s\t- %s\r\n", commands[i].name, commands[i].help);
	} 
	putchar('\r'); putchar('\n');

	return CMD_SUCCESS;
}

COMMAND_ENTRY(
	"help",
	"help [command_name]",
	"Display a command's help message.",
	do_help
)
