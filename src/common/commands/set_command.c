#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "command.h"
#include "control.h"

extern control_t control;
command_status do_set(int argc, char *argv[]) {
    if (argc < 2)
        return USAGE;
    float val = atof(argv[2]);
    if (!strcmp("req_x", argv[1])) {
        control.desired_x = val;
        return CMD_SUCCESS;
    }
    else if (!strcmp("req_y", argv[1])) {
        control.desired_y = val;
        return CMD_SUCCESS;
    }
    else if (!strcmp("req_z", argv[1])) {
        control.desired_z = val;
        return CMD_SUCCESS;
    }
    else if (!strcmp("throttle", argv[1])) {
        control.throttle = val;
        return CMD_SUCCESS;
    }
    else if (!strcmp("p_gain", argv[1])) {
        control.p_gain = val;
        return CMD_SUCCESS;
    }
    else if (!strcmp("i_gain", argv[1])) {
        control.i_gain = val;
        return CMD_SUCCESS;
    }
    else if (!strcmp("d_gain", argv[1])) {
        control.d_gain = val;
        return CMD_SUCCESS;
    }
    return USAGE;
}
COMMAND_ENTRY("set", "{ req_x | req_y | req_z | throttle | p_gain | i_gain | d_gain } [val]", "Set gain values for PID loop", do_set)
