#include "gyro.h"
#include "i2c.h"
#include "board.h"
#include <string.h>

uint8_t buff[WATERMARK_SIZE];

void gyro_write(uint8_t reg, uint8_t data)
{
    delay(10);
    uint8_t data_send[2];
    data_send[0] = reg;
    data_send[1] = data;
    i2c_write(GYRO_ADDR, data_send, 2, 1);
}

void gyro_read(uint8_t reg, uint8_t *data)
{
    delay(10);
    data[0] = reg;
    i2c_write(GYRO_ADDR, data, 1, 0);
    i2c_read(GYRO_ADDR, data, 1);
}

void gyro_read_multiple(uint8_t reg, uint8_t *data, uint16_t size)
{
    data[0] = reg;
    i2c_write(GYRO_ADDR, data, 1, 0);
    i2c_read(GYRO_ADDR, data, size);
}

uint8_t gyro_who_am_i()
{
    uint8_t data[1];
    gyro_read(WHO_AM_I_21002, data);
    return data[0];
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
    uint8_t CTRL0, CTRL1, CTRL2, F_SETUP;

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

    return 0;
}

// FIFO
void gyro_int1_callback()
{
    gyro_read_fifo();
}

void gyro_int2_callback()
{
}
