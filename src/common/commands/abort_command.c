#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "command.h"
#include "esc.h"
#include "control.h"

extern control_t control;
extern bool abort_control;

command_status do_abort(int argc, char *argv[])
{
    abort();
    printf("manual abort triggered]\r\n");
    return CMD_SUCCESS;
}

COMMAND_ENTRY("abort", "abort", "abort ALL ESCs", do_abort)
