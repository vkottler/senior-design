#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "command.h"
#include "control.h"
#include "gyro.h"

extern control_t control;
extern bool abort_control;

void report_pids(void)
{
    printf("P: x=%.2f, y=%.2f, z=%.2f\r\n",
           control.px_gain, control.py_gain, control.pz_gain);
    printf("I: x=%.2f, y=%.2f, z=%.2f\r\n",
           control.ix_gain, control.iy_gain, control.iz_gain);
    printf("D: x=%.2f, y=%.2f, z=%.2f\r\n",
           control.dx_gain, control.dy_gain, control.dz_gain);
}

command_status do_pid(int argc, char *argv[]) {

    float gainx, gainy, gainz;
    if (argc == 1)
    {
        report_pids();
        return CMD_SUCCESS;
    }

    if (argc == 5)
    {
        gainx = atof(argv[2]);
        gainy = atof(argv[3]);
        gainz = atof(argv[4]);
        if (argv[1][0] == 'x')
        {
            control.px_gain = gainx;
            control.ix_gain = gainy;
            control.dx_gain = gainz;
        }
        else if (argv[1][0] == 'y')
        {
            control.py_gain = gainx;
            control.iy_gain = gainy;
            control.dy_gain = gainz;
        }
        else if (argv[1][0] == 'z')
        {
            control.pz_gain = gainx;
            control.iz_gain = gainy;
            control.dz_gain = gainz;
        }
        else
        {
            printf("unknown axis '%s'\r\n", argv[2]);
            return FAIL;
        }

        control.i_output_x = 0;
        control.i_output_y = 0;
        control.i_output_z = 0;
        control.prev_dif_x = 0;
        control.prev_dif_y = 0;
        control.prev_dif_z = 0;
        report_pids();

        end_abort();

        return CMD_SUCCESS;
    }

    return USAGE;
}

COMMAND_ENTRY("pid", "pid {x | y | z} { p_gain | i_gain | d_gain } <val>",
              "Set gain values for PID loop", do_pid)
