#include <stdio.h>
#include "command.h"
#include "control.h"

command_status do_abort(int argc, char *argv[])
{
    abort();
    printf("manual abort triggered\r\n");
    return CMD_SUCCESS;
}

COMMAND_ENTRY("abort", "abort", "abort ALL ESCs", do_abort)
