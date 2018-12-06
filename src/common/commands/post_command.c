#include "command.h"
#include "post.h"

command_status do_post(int argc, char *argv[])
{
    post();
    return CMD_SUCCESS;
}

COMMAND_ENTRY("post", "post", "View post output again.", do_post)
