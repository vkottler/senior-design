#include "main.h"
#include <stdio.h>
#include "usart.h"

int main(void)
{
    io_init();
    periph_init();

    while (1)
    {
        blink_handler(500);
        telem_handler(1000);
        check_input();
    }

    return 0;
}
