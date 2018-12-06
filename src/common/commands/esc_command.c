#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "command.h"
#include "esc.h"

command_status do_esc(int argc, char *argv[]) {

    if (argc < 2)
        return USAGE;

    uint16_t val = atoi(argv[2]);
    if (!strcmp("x_pos", argv[1])) {
	    esc_set_pulse(ESC_TABLE[X_POS_INDEX], val);
        return CMD_SUCCESS;
    }
    else if (!strcmp("x_neg", argv[1])) {
	    esc_set_pulse(ESC_TABLE[X_NEG_INDEX], val);
        return CMD_SUCCESS;
    }
    else if (!strcmp("y_pos", argv[1])) {
	    esc_set_pulse(ESC_TABLE[Y_POS_INDEX], val);
        return CMD_SUCCESS;
    }
    else if (!strcmp("y_neg", argv[1])) {
	    esc_set_pulse(ESC_TABLE[Y_NEG_INDEX], val);
        return CMD_SUCCESS;
    }

    return CMD_SUCCESS;
}

COMMAND_ENTRY("esc", "{ x_pos | x_neg | y_pos | y_neg } [val]", "Set ESC speed value 0 - 1000", do_esc)
