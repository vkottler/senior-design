#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "command.h"
#include "esc.h"
#include "board.h"

command_status do_esc(int argc, char *argv[]) {

    if (argc < 2)
        return USAGE;

    if (!strcmp("calibrate", argv[1])) {
        esc_set_pulse(ESC_TABLE[X_POS_INDEX], 0);
        esc_set_pulse(ESC_TABLE[X_NEG_INDEX], 0);
        esc_set_pulse(ESC_TABLE[Y_POS_INDEX], 0);
        esc_set_pulse(ESC_TABLE[Y_NEG_INDEX], 0);

        // Wait for ESC to latch
        delay(7000);

        esc_set_pulse(ESC_TABLE[X_POS_INDEX], 0);
        esc_set_pulse(ESC_TABLE[X_NEG_INDEX], 0);
        esc_set_pulse(ESC_TABLE[Y_POS_INDEX], 0);
        esc_set_pulse(ESC_TABLE[Y_NEG_INDEX], 0);
    }

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

    return USAGE;
}

COMMAND_ENTRY("esc", "{ x_pos | x_neg | y_pos | y_neg } [val]", "Set ESC speed value 0 - 1000", do_esc)
