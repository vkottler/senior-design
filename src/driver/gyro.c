#include "gyro.h"
#include "stateful_spi.h"
#include "board.h"
#include "pcbuffer.h"
#include <string.h>
#include <stdlib.h>

uint8_t buff[WATERMARK_SIZE];

#define CALIBRATE_NUM 10000.0f
#define SENSITIVITY   (7.8125f / 1000.0f)

PC_Buffer *gyro_tx_buf, *gyro_rx_buf;
int32_t gyro_accum_offset[3] = {0, 0, 0};
float gyro_offset[3] = {0.0f, 0.0f, 0.0f};
extern void (*fun_ptr)();
void  gyro_xyz_Callback(void);
void  SPI1_Rx_Callback(void);

void setOffset()
{
    uint8_t data[2];
    for(int i = 0; i < 3; i++)
    {
        while (pc_buffer_empty(gyro_rx_buf)){}
        pc_buffer_remove(gyro_rx_buf, (char*) &data[0]);
        while (pc_buffer_empty(gyro_rx_buf)){}
        pc_buffer_remove(gyro_rx_buf, (char*) &data[1]);
        gyro_accum_offset[i] += (int16_t)(data[0] << 8 | data[1]);
    }
}

void zeroGyroXYZ()
{
    float zero = 0;
    for(int i = 0; i < 3; i++)
        memcpy(&zero, manifest.channels[i].data, sizeof(float));
}

void getGyroXYZ()
{
    uint8_t data[2];
    float   gyro_raw;
    float   gyro_deg;

    for(int i = 0; i < 3; i++)
    {
        while (pc_buffer_empty(gyro_rx_buf)){}
        pc_buffer_remove(gyro_rx_buf, (char*) &data[0]);
        while (pc_buffer_empty(gyro_rx_buf)){}
        pc_buffer_remove(gyro_rx_buf, (char*) &data[1]);
        gyro_raw = (float)((int16_t)(data[0] << 8 | data[1]));

        memcpy(&gyro_deg, manifest.channels[i].data, sizeof(float));
        gyro_deg += (gyro_raw * SENSITIVITY - gyro_offset[i]) * 0.025f;

        memcpy(manifest.channels[i].data, &gyro_deg, sizeof(float));
    }
}

void gyro_write(uint8_t reg, uint8_t data)
{
    uint8_t tx_buff[2];
    tx_buff[0] = reg;
    tx_buff[1] = data;

    SET_BIT(SPI1->CR1, SPI_CR1_SPE);                     // SPI On

    while (pc_buffer_full(gyro_tx_buf)) {;}

	/* safely add the desired character */
	__disable_irq();
	pc_buffer_add(gyro_tx_buf, tx_buff[0]);
	__enable_irq();

    while (pc_buffer_full(gyro_tx_buf)) {;}

	/* safely add the desired character */
	__disable_irq();
	pc_buffer_add(gyro_tx_buf, tx_buff[1]);
	__enable_irq();

    LL_SPI_EnableIT_TXE(SPI1);

    CLEAR_BIT(SPI1->CR1, SPI_CR1_SPE);                   // SPI Off
}

void gyro_read(uint8_t reg, uint8_t *data)
{
    gyro_write((reg | 0x80), 0x00);
    *data = 0xff;
}

void gyro_read_multiple(uint8_t reg, uint8_t *data, uint16_t size)
{
    gyro_write((reg | 0x80), 0x00);
    *data = 0xff;
}

uint8_t gyro_who_am_i()
{
    uint8_t data[1];
    gyro_read(WHO_AM_I_21002, data);
    return data[0];
}

void gyro_read_xyz()
{
   fun_ptr =  &gyro_xyz_Callback;

    gyro_read_x();
    gyro_read_y();
    gyro_read_z();

}

int16_t gyro_read_x()
{
    uint8_t data_msb[1];
    gyro_read(OUT_X_MSB_21002, data_msb);

    uint8_t data_lsb[1];
    gyro_read(OUT_X_LSB_21002, data_lsb);

    return (int16_t) (data_msb[0] << 8 | data_lsb[0]);
}

int16_t gyro_read_y()
{
    uint8_t data_msb[1];
    gyro_read(OUT_Y_MSB_21002, data_msb);

    uint8_t data_lsb[1];
    gyro_read(OUT_Y_LSB_21002, data_lsb);

    return (int16_t)(data_msb[0] << 8 | data_lsb[0] );
}

int16_t gyro_read_z()
{
    uint8_t data_msb[1];
    gyro_read(OUT_Z_MSB_21002, data_msb);

    uint8_t data_lsb[1];
    gyro_read(OUT_Z_LSB_21002, data_lsb);

    return (int16_t)(data_msb[0] << 8 | data_lsb[0]);
}

void gyro_read_fifo()
{
    int i;
    for (i = 0; i < WATERMARK_SIZE; i ++);
    gyro_read_multiple(OUT_X_MSB_21002, buff, 6);
}

uint16_t gyro_config()
{
    uint8_t CTRL1, CTRL0;

    fun_ptr = &SPI1_Rx_Callback;
    gyro_tx_buf = (PC_Buffer *) malloc(sizeof(PC_Buffer));
    gyro_rx_buf = (PC_Buffer *) malloc(sizeof(PC_Buffer));

    if (!gyro_tx_buf || !gyro_rx_buf) 
        return -1;
    if (!pc_buffer_init(gyro_tx_buf, 255))
        return -1;
    if (!pc_buffer_init(gyro_rx_buf, 255))	
        return -1;

    CTRL1 = SOFT_RESET_21002 | MODE_STANDBY_21002;
    gyro_write(CTRL_REG1_21002, CTRL1);
    delay(50);

    CTRL0 = FS_250_DPS_21002;
    CTRL1 = ODR_800_HZ_21002;

    gyro_write(CTRL_REG0_21002, CTRL0);
    gyro_write(CTRL_REG1_21002, CTRL1);
    delay(50);

    CTRL1 = MODE_ACTIVE_21002;
    gyro_write(CTRL_REG1_21002, CTRL1);

    gyro_read_xyz();

    for(uint32_t i = 0; i < (uint32_t) CALIBRATE_NUM; i++)
    {
        gyro_read_xyz();
        setOffset();
    }

    gyro_offset[0] = gyro_accum_offset[0] / CALIBRATE_NUM;
    gyro_offset[1] = gyro_accum_offset[1] / CALIBRATE_NUM;
    gyro_offset[2] = gyro_accum_offset[2] / CALIBRATE_NUM;
    gyro_offset[0] = gyro_offset[0] * SENSITIVITY;
    gyro_offset[1] = gyro_offset[1] * SENSITIVITY;
    gyro_offset[2] = gyro_offset[2] * SENSITIVITY;

    printf("x offset: %0.2f\r\ny offset: %0.2f\r\nz offset: %0.2f\r\n",
           gyro_offset[0], gyro_offset[1], gyro_offset[2]);

    return 0;
}

// FIFO
void  gyro_xyz_Callback(void)
{
  uint16_t data = (SPI1->DR);
  if (!pc_buffer_full(gyro_rx_buf))
      pc_buffer_add(gyro_rx_buf, (char) (data >> 8));
}

void  SPI1_Rx_Callback(void)
{ (SPI1->DR); }

/*
void write_done_discard(const uint8_t *buffer, size_t len)
{
    (void) buffer;
    (void) len;
}

void gyro_write(uint8_t reg, uint8_t data)
{
    uint8_t tx_buf[2], rx_buf[2];
    tx_buf[0] = reg;
    tx_buf[1] = data;
    spi_transaction_t tran;
    spi_init_transaction(&tran, tx_buf, rx_buf, 2, 2, write_done_discard);
    spi_start_transaction(&spi1_state, &tran);
    spi_wait_on_transaction(&tran);
}
*/
