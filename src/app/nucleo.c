#include "nucleo.h"
#include "post.h"
#include "cli.h"

void nucleo_init(void)
{
    nucleo_io_init();
    nucleo_set_clks();
    nucleo_periph_init();
    post();
    printPrompt();
}

int main(void)
{
    nucleo_init();

    while (1)
    {
        nucleo_blink_handler(250);
        check_input();
    }

    return 0;
}
