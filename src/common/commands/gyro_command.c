#include "command.h"
#include "gyro.h"

command_status do_gyro(int argc, char *argv[])
{
    (void) argc;
    (void) argv;
    dump_gyro(&gyro);
    return CMD_SUCCESS;
}

COMMAND_ENTRY("gyro", "gyro",
              "Dump information about the gyro sensor.", do_gyro)
