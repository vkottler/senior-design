#include "handlers.h"
#include "gpio.h"
#include "accel.h"
#include "gyro.h"
#include "board.h"
#include "i2c.h"
#include "spi.h"
#include "gpio.h"
#include "pcbuffer.h"
#include <unistd.h>

#define CALIBRATE_NUM 20

PC_Buffer *gyro_tx_buf[1], *gyro_rx_buf[1];
static int16_t gyro_data[3] = {0, 0, 0};
static int16_t gyro_offset[3] = {0, 0, 0};
bool calibrate = true;
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

void setOffset()
{
    uint8_t data[2];
    static uint8_t cnt = 0;
    cnt ++;
    for(int i = 0; i < 3; i++)
    {
        while (pc_buffer_empty(gyro_rx_buf[0])){}
        pc_buffer_remove(gyro_rx_buf[0], (char*) &data[0]);
        while (pc_buffer_empty(gyro_rx_buf[0])){}
        pc_buffer_remove(gyro_rx_buf[0], (char*) &data[1]);
        gyro_offset[i] += (data[0] << 8 | data[1]);
    }
    if(cnt == CALIBRATE_NUM) {
        calibrate = false;
        gyro_offset[0] = gyro_offset[0] / 10;
        gyro_offset[1] = gyro_offset[1] / 10;
        gyro_offset[2] = gyro_offset[2] / 10;
    }

}
void getGyroXYZ()
{
    uint8_t data[2];
    for(int i = 0; i < 3; i++)
    {
        while (pc_buffer_empty(gyro_rx_buf[0])){}
        pc_buffer_remove(gyro_rx_buf[0], (char*) &data[0]);
        while (pc_buffer_empty(gyro_rx_buf[0])){}
        pc_buffer_remove(gyro_rx_buf[0], (char*) &data[1]);
        gyro_data[i] += (data[0] << 8 | data[1]) - gyro_offset[i];
    }
    printf("\r\n X: %d\r\n", gyro_data[0]);
    printf("Y: %d\r\n", gyro_data[1]);
    printf("Z: %d\r\n", gyro_data[2]);

}

void telem_handler(unsigned int interval) {
    static uint32_t last_tick = 0;

    if (!(ticks % interval) && ticks != last_tick)
    {
        last_tick = ticks;
        if (calibrate) setOffset();
        else getGyroXYZ();

        gyro_read_xyz();
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
