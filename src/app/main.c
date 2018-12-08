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
        service_sensors(25);

        /* send telemetry if transmission is possible */
        if (radio_transmit_state)
        {
            send_high_rate_telemetry(666);
            send_low_rate_telemetry(2000);
        }
        else if (ticks > radio_resume)
        {
            radio_transmit_state = true;
            USART1->CR1 |= USART_CR1_TXEIE;
        }

        /* re-enable the telemetry stream if the radio is available */
        if (radio_buffer_full && gpio_readPin(GPIOB, 2))
        {
            USART1->CR1 |= USART_CR1_TXEIE;
            radio_buffer_full = false;
        }

        /* service incoming commands */
        check_input();
    }

    return 0;
}
