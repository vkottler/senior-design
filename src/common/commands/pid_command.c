#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "command.h"
#include "control.h"
#include "gyro.h"

extern control_t control;
extern bool abort_control;

command_status do_pid(int argc, char *argv[]) {

    float gain;
    if (argc == 1)
    {
        printf("Proportional gain: %.2f\r\n", control.p_gain);
        printf("Integral gain: %.2f\r\n", control.i_gain);
        printf("Derivative gain: %.2f\r\n", control.d_gain);
        return CMD_SUCCESS;
    }

    if (argc == 4)
    {
        gain = atof(argv[1]);
        control.p_gain = gain;
        gain = atof(argv[2]);
        control.i_gain = gain;
        gain = atof(argv[3]);
        control.d_gain = gain;
        control.i_output_x = 0;
        control.i_output_y = 0;
        control.i_output_z = 0;
        control.prev_dif_x = 0;
        control.prev_dif_y = 0;
        control.prev_dif_z = 0;
        control.throttle = START_THROTTLE;
        control.desired_x = 0;
        control.desired_y = 0;
        control.desired_z = 0;
        gyro.accum[0] = 0.0f;
        gyro.accum[1] = 0.0f;
        gyro.accum[2] = 0.0f;
        abort_control = false;
        printf("Proportional gain: %.2f\r\n", control.p_gain);
        printf("Integral gain: %.2f\r\n", control.i_gain);
        printf("Derivative gain: %.2f\r\n", control.d_gain);
        return CMD_SUCCESS;
    }

    return USAGE;
}

COMMAND_ENTRY("pid", "pid { p_gain | i_gain | d_gain } [val]", "Set gain values for PID loop", do_pid)
