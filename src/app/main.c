#include "main.h"
#include "gyro.h"

int main(void)
{
    io_init();
    periph_init();
    manifest_init();
    gyro_config();

    while (1)
    {
        service_gyro(&gyro);
        blink_handler(75);
        //service_sensors(25);
        if (radio_transmit_state)
        {
            send_high_rate_telemetry(250);
            //send_low_rate_telemetry(1333);
        }
        else if (ticks > radio_resume)
            radio_transmit_state = true;
        check_input();
    }

    return 0;
}
