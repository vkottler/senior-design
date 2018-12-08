#include "command.h"
#include "channels.h"

command_status do_tm(int argc, char *argv[])
{
    dump_telemetry_manager(&tm);
    return CMD_SUCCESS;
}

COMMAND_ENTRY("tm", "tm", "Telemetry interface.", do_tm)
