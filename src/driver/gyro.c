#include "gyro.h"
#include "spi.h"
#include "board.h"
#include "pcbuffer.h"
#include <string.h>
#include <stdlib.h>

uint8_t buff[WATERMARK_SIZE];

#define CALIBRATE_NUM 100

PC_Buffer *gyro_tx_buf[1], *gyro_rx_buf[1];
static int16_t gyro_offset[3] = {0, 0, 0};
bool calibrate = true;
extern void (*fun_ptr)();
void  gyro_xyz_Callback(void);
void  SPI1_Rx_Callback(void);

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
        gyro_offset[0] = gyro_offset[0] / CALIBRATE_NUM;
        gyro_offset[1] = gyro_offset[1] / CALIBRATE_NUM;
        gyro_offset[2] = gyro_offset[2] / CALIBRATE_NUM;
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
        *((int16_t *) manifest.channels[i].data) += (data[0] << 8 | data[1]) - gyro_offset[i];
    }
}

void gyro_write(uint8_t reg, uint8_t data)
{
    uint8_t tx_buff[2];
    tx_buff[0] = reg;
    tx_buff[1] = data;

    SET_BIT(SPI1->CR1, SPI_CR1_SPE);                     // SPI On

    while (pc_buffer_full(gyro_tx_buf[0])) {;}

	/* safely add the desired character */
	__disable_irq();
	pc_buffer_add(gyro_tx_buf[0], tx_buff[0]);
	__enable_irq();

    while (pc_buffer_full(gyro_tx_buf[0])) {;}

	/* safely add the desired character */
	__disable_irq();
	pc_buffer_add(gyro_tx_buf[0], tx_buff[1]);
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
  /*  uint8_t CTRL0, CTRL1, CTRL2, F_SETUP;

    CTRL1 = SOFT_RESET_21002 | MODE_STANDBY_21002;
    gyro_write(CTRL_REG1_21002, CTRL1);
    delay(10);

    F_SETUP = F_MODE_CIRCULAR_21002 | WATERMARK_SIZE;
    CTRL0 = FS_250_DPS_21002;
    CTRL1 = ODR_800_HZ_21002;
    CTRL2 = INT1_CFG_FIFO_21002 | INT_EN_FIFO_21002 |
            IPOL_ACTIVE_HIGH_21002;

    gyro_write(F_SETUP_21002, F_SETUP);
    gyro_write(CTRL_REG0_21002, CTRL0);
    gyro_write(CTRL_REG1_21002, CTRL1);
    gyro_write(CTRL_REG2_21002, CTRL2);

    CTRL1 = MODE_ACTIVE_21002;
    gyro_write(CTRL_REG1_21002, CTRL1);
*/

   fun_ptr = &SPI1_Rx_Callback;
	gyro_tx_buf[0] = (PC_Buffer *) malloc(sizeof(PC_Buffer));
    gyro_rx_buf[0] = (PC_Buffer *) malloc(sizeof(PC_Buffer));

    if (!gyro_tx_buf[0] || !gyro_rx_buf[0]) 
        return -1;
    if (!pc_buffer_init(gyro_tx_buf[0], 255))
        return -1;
    if (!pc_buffer_init(gyro_rx_buf[0], 255))	
        return -1;

    gyro_write(CTRL_REG1_21002, MODE_ACTIVE_21002);
        gyro_read_xyz();

    for(uint32_t i = 0; i < CALIBRATE_NUM; i ++)
    {
        gyro_read_xyz();
        setOffset();
    }

    return 0;
}

// FIFO
void  gyro_xyz_Callback(void)
{
  uint16_t data = (SPI1->DR);
  if (!pc_buffer_full(gyro_rx_buf[0]))
      pc_buffer_add(gyro_rx_buf[0], (char) (data >> 8));
}

void  SPI1_Rx_Callback(void)
{
  (SPI1->DR);
}

