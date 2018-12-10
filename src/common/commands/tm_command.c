#include "command.h"
#include "channels.h"

#include <string.h>
#include <stdlib.h>

void all_telem(bool state)
{
    for (int i = 0; i < TELEM_NUM_GROUPS; i++)
        packet_groups[i].publish = state;
}

void all_telem_on(void) { all_telem(true); }
void all_telem_off(void) { all_telem(false); }

command_status do_tm(int argc, char *argv[])
{
    const char *group_name;
    telemetry_group_t *group = NULL;
    bool to_set;
    int i;
    int32_t new_rate;

    if (argc == 1)
    {
        dump_telemetry_manager(&tm);
        return CMD_SUCCESS;
    }

    /* turn all telemetry on or off */
    if (argc == 2)
    {
        if (!strcmp("on", argv[1])) all_telem_on();
        else if (!strcmp("off", argv[1])) all_telem_off();
        else return FAIL;
        return CMD_SUCCESS;
    }

    group_name = argv[1];
    for (i = 0; i < TELEM_NUM_GROUPS; i++)
    {
        if (!strcmp(group_name, packet_groups[i].alias))
        {
            group = &packet_groups[i];
            break;
        }
    }

    if (group == NULL)
    {
        printf("group '%s' not found.\r\n", group_name);
        return FAIL;
    }

    /* turn specific group on or off */
    if (argc == 3)
    {
        if (!strcmp("on", argv[2])) to_set = true;
        else if (!strcmp("off", argv[2])) to_set = false;
        else return USAGE;
        group->publish = to_set;
        return CMD_SUCCESS;
    }

    if (argc != 4) return USAGE;

    /* change the rate of the telemetry */
    else if (strcmp("rate", argv[2]))
    {
        printf("unrecognized argument '%s'\r\n", argv[2]);
        return USAGE;
    }

    new_rate = strtol(argv[3], NULL, 10);
    if (new_rate < MIN_TELEM_PERIOD || new_rate > MAX_TELEM_PERIOD)
    {
        printf("rate must be > %d and < %d\r\n", MIN_TELEM_PERIOD,
               MAX_TELEM_PERIOD);
        return FAIL;
    }
    group->rate = new_rate;

    return CMD_SUCCESS;
}

COMMAND_ENTRY("tm", "tm [<group> { on | off | rate <value> }]",
              "Telemetry interface.", do_tm)
