#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "command.h"
#include "gyro.h"

command_status do_gyro(int argc, char *argv[]) {

    if (argc < 2)
        return USAGE;

    if (argc == 2)
    {
        if (!strcmp("zero", argv[1])) {
            zeroGyroXYZ();
            return CMD_SUCCESS;
        }
    }

    return USAGE;
}

COMMAND_ENTRY("gyro", "zero | { cal [val] }", "Clear gyro accumulator and calibrate the offset for the gyro", do_gyro)
