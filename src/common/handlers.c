#include "handlers.h"
#include "accel.h"
#include "lidar.h"
#include "esc.h"
#include "board.h"
#include "board_config.h"
#include "i2c.h"
#include "spi.h"
#include "gpio.h"
#include "gyro.h"
#include "adc.h"
#include "usart.h"
#include "pcbuffer.h"
#include "battery.h"
#include "control.h" 
#include <unistd.h>
#include <string.h>

PC_Buffer *gyro_tx_buf[1], *gyro_rx_buf[1];
void (*fun_ptr)();
bool abort_control = false;
extern control_t control;
void fault_handler(unsigned int blink_int)
{

    static unsigned int curr = 0, prev = 0;

    curr = ticks / blink_int;
    if (curr != prev)
    {
        if (curr % 2) gpio_resetPin(GPIOC, 3);
        else gpio_setPin(GPIOC, 3);
    }
    prev = curr;
}

void fault_loop(void)
{ while (1) { fault_handler(1000); } }

void blink_handler(unsigned int blink_int)
{

    static unsigned int curr = 0, prev = 0;

    curr = ticks / blink_int;
    if (curr != prev)
    {
        if (curr % 2) gpio_resetPin(GPIOA, 1);
        else gpio_setPin(GPIOA, 1);
    }
    prev = curr;
}

void service_lidar(void)
{
    if (lidar_data_ready(LIDAR1))
        *((uint16_t *) manifest.channels[LIDAR1_IND].data) = lidar_readDist(LIDAR1);
    if (lidar_data_ready(LIDAR2))
        *((uint16_t *) manifest.channels[LIDAR2_IND].data) = lidar_readDist(LIDAR2);
}

void run_critical(void)
{
    service_gyro(&gyro);
    service_lidar();
    run_control();
    batt_getBattery();
}

void run_control(void)
{
    if(abort_control) return;
    if(gyro.new_data)
    {
        float raw_input_x, raw_input_y, raw_input_z;
        memcpy(&raw_input_x, manifest.channels[0].data, sizeof(float));
        memcpy(&raw_input_y, manifest.channels[1].data, sizeof(float));
        memcpy(&raw_input_z, manifest.channels[2].data, sizeof(float));

        control_loop_x(raw_input_x, control.throttle);
        control_loop_y(raw_input_y, control.throttle);

        gyro.new_data = false;
    }
}
