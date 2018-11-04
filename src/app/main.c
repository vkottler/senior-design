#include "main.h"

int main(void)
{
    io_init();
    periph_init();

    while (1)
    {
        blink_handler(250);
    }

    return 0;
}
