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
        blink_handler(75);
        service_sensors(25);
        send_high_rate_telemetry(250);
        send_low_rate_telemetry(1333);
        check_input();
    }

    return 0;
}
