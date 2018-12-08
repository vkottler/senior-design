#include "main.h"
#include "gyro.h"

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
        blink_handler(75);

        service_gyro(&gyro);
        service_sensors(25);

        /* service incoming commands */
        check_input();

        /* re-enable the telemetry stream if the radio is available */
        update_radio_state();

        /* use remaining time to publish telemetry */
        publish_telemetry(&tm);
    }

    return 0;
}
