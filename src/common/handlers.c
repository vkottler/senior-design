#include "handlers.h"
#include "gpio.h"
#include "accel.h"
#include "gyro.h"
#include "board.h"
#include "i2c.h"
#include "gpio.h"
#include <unistd.h>

void blink_handler(unsigned int blink_int) {

    static unsigned int curr = 0, prev = 0;

    curr = ticks / blink_int;
    if (curr != prev)
    {
        if (curr % 2) gpio_resetPin(GPIOA, 5);
        else gpio_setPin(GPIOA, 5);
    }
    prev = curr;
}

void telem_handler(unsigned int interval) {
    static uint32_t last_tick = 0;

    if (!(ticks % interval) && ticks != last_tick)
    {
        last_tick = ticks;
        printf("ACCEL\r\n");
        short accel_data[3];
        accel_data[0] = accel_read_x();
        accel_data[1] = accel_read_y();
        accel_data[2] = accel_read_z();
        short gyro_data[3];
        gyro_data[0] = gyro_read_x();
        gyro_data[1] = gyro_read_y();
        gyro_data[2] = gyro_read_z();
        printf("x: %d\r\n", (short)accel_data[0]);
        printf("y: %d\r\n", (short)accel_data[1]);
        printf("z: %d\r\n", (short)accel_data[2]);
        printf("GYRO\r\n");
        printf("x: %d\r\n", (short)gyro_data[0]);
        printf("y: %d\r\n", (short)gyro_data[1]);
        printf("z: %d\r\n", (short)gyro_data[2]);
        write(RADIO_FD, (void*)accel_data, 6);
        write(RADIO_FD, (void*)gyro_data, 6);
    }
}

