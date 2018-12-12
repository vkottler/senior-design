#include "main.h"
#include "gyro.h"
#include "battery.h"

int main(void)
{
    io_init();
    periph_init();

    if (!manifest_init())
    {
        printf("manifest init failed\r\n");
        fault_loop();
    }

    gyro_config();

    tm.t_zero = ticks;
    while (1)
    {
        /* toggle the LED if it makes sense to */
        blink_handler(75);

        /* perform any work needing servicing */
        run_critical();

        /* service incoming commands */
        check_input();

        /* use remaining time to publish telemetry */
        publish_telemetry(&tm);
    }

    return 0;
}
