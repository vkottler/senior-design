#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "command.h"
#include "handlers.h"

extern control_t control;

command_status do_control(int argc, char *argv[]) {

    if (argc < 2)
        return USAGE;

    float val = atof(argv[2]);
    if (!strcmp("x", argv[1])) {
        control.desired_x = val;
        return CMD_SUCCESS;
    }
    else if (!strcmp("y", argv[1])) {
        control.desired_y = val;
        return CMD_SUCCESS;
    }
    else if (!strcmp("z", argv[1])) {
        control.desired_z = val;
        return CMD_SUCCESS;
    }
    else if (!strcmp("up", argv[1])) {
        control.throttle = val;
        return CMD_SUCCESS;
    }

    return USAGE;
}

COMMAND_ENTRY("control", "{ x | y | z | up } [val]", "Set desired values", do_control)
