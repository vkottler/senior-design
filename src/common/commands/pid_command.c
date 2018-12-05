#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "command.h"
#include "handlers.h"

extern control_t control;

command_status do_pid(int argc, char *argv[]) {

    if (argc < 2)
        return USAGE;

    if (argc == 2)
    {
        if (!strcmp("gain", argv[1])) {
            printf("Proportional gain: %.2f\r\n", control.p_gain);
            printf("Integral gain: %.2f\r\n", control.i_gain);
            printf("Derivative gain: %.2f\r\n", control.d_gain);
        }
    }


    float gain = atof(argv[2]);
    if (!strcmp("p_gain", argv[1])) {
        control.p_gain = gain;
        return CMD_SUCCESS;
    }
    else if (!strcmp("i_gain", argv[1])) {
        control.i_gain = gain;
        return CMD_SUCCESS;
    }
    else if (!strcmp("d_gain", argv[1])) {
        control.d_gain = gain;
        return CMD_SUCCESS;
    }

    return CMD_SUCCESS;
}

COMMAND_ENTRY("pid", "pid { p_gain | i_gain | d_gain } [val]", "Set gain values for PID loop", do_pid)
