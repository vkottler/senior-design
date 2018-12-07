#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "command.h"
#include "esc.h"
#include "handlers.h"

extern control_t control;

command_status do_abort(int argc, char *argv[]) {
    
	control.desired_x = 0;
    control.desired_y = 0;
    control.desired_z = 0;
	control.throttle = 0;

    esc_set_pulse(ESC_TABLE[X_POS_INDEX], 0);
    esc_set_pulse(ESC_TABLE[X_NEG_INDEX], 0);
    esc_set_pulse(ESC_TABLE[Y_POS_INDEX], 0);
    esc_set_pulse(ESC_TABLE[Y_NEG_INDEX], 0);

    return CMD_SUCCESS;
}

COMMAND_ENTRY("abort", "abort", "abort ALL ESCs", do_abort)
