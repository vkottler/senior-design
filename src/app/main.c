#include "main.h"
#include <stdio.h>
#include "usart.h"
#include "../control/control.h"

int main(void)
{
    io_init();
    periph_init();
    manifest_init();

    while (1)
    {
        blink_handler(250);
        telem_handler(400);
        check_input();
    }

    return 0;
}
