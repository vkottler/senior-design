#include "handlers.h"
#include "gpio.h"
#include "accel.h"
#include "gyro.h"
#include "battery.h"
#include "lidar.h"
#include "board.h"
#include "board_config.h"
#include "i2c.h"
#include "spi.h"
#include "gpio.h"
#include "adc.h"
#include "usart.h"
#include "pcbuffer.h"
#include <unistd.h>

PC_Buffer *gyro_tx_buf[1], *gyro_rx_buf[1];
void (*fun_ptr)();

void fault_handler(unsigned int blink_int) {

    static unsigned int curr = 0, prev = 0;

    curr = ticks / blink_int;
    if (curr != prev)
    {
        if (curr % 2) gpio_resetPin(GPIOC, 3);
        else gpio_setPin(GPIOC, 3);
    }
    prev = curr;
}

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
        getGyroXYZ();
        gyro_read_xyz();
        batt_startConver();
        batt_getBattery();
        lidar_readDist(1);
        lidar_readDist(2);

    }
}

void  SPI1_Tx_Callback(void)
{
    if (!pc_buffer_empty(gyro_tx_buf[0]))
        pc_buffer_remove(gyro_tx_buf[0], (char *) &SPI1->DR);
    else
        LL_SPI_DisableIT_TXE(SPI1);
}

void SPI1_TransferError_Callback(void)
{
  LL_SPI_DisableIT_RXNE(SPI1);
  LL_SPI_DisableIT_TXE(SPI1);
  blink_handler(50);
}

void SPI1_Handler(void)
{
    if(LL_SPI_IsActiveFlag_RXNE(SPI1))
    {
        (*fun_ptr)();
    }
    else if(LL_SPI_IsActiveFlag_TXE(SPI1))
    {
        SPI1_Tx_Callback();
    }
    else if(LL_SPI_IsActiveFlag_OVR(SPI1))
    {
        SPI1_TransferError_Callback();
    }
}
