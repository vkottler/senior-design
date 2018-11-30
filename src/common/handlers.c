#include "handlers.h"
#include "gpio.h"
#include "accel.h"
#include "gyro.h"
#include "board.h"
#include "i2c.h"
#include "spi.h"
#include "gpio.h"
#include <unistd.h>

void blink_handler(unsigned int blink_int) {

    static unsigned int curr = 0, prev = 0;

    curr = ticks / blink_int;
    if (curr != prev)
    {
        if (curr % 2) gpio_resetPin(GPIOA, 1);
        else gpio_setPin(GPIOA, 1);
    }
    prev = curr;
}

void telem_handler(unsigned int interval) {
    static uint32_t last_tick = 0;

    if (!(ticks % interval) && ticks != last_tick)
    {
        last_tick = ticks;
    printf("X: %d\r\n", gyro_read_x());
    printf("Y: %d\r\n", gyro_read_y());
    printf("Z: %d\r\n", gyro_read_z());
    }
}

