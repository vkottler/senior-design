#include "gyro.h"
#include "i2c.h"
#include "board.h"
#include <string.h>

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

    return (int16_t)(data_msb[0] << 8 | data_lsb[0] ) >> 4;
}

int16_t gyro_read_y()
{
    uint8_t data_msb[1];
    gyro_read(OUT_Y_MSB_21002, data_msb);

    uint8_t data_lsb[1];
    gyro_read(OUT_Y_LSB_21002, data_lsb);

    return (int16_t)(data_msb[0] << 8 | data_lsb[0] ) >> 4;
}

int16_t gyro_read_z()
{
    uint8_t data_msb[1];
    gyro_read(OUT_Z_MSB_21002, data_msb);

    uint8_t data_lsb[1];
    gyro_read(OUT_Z_LSB_21002, data_lsb);

    return (int16_t)(data_msb[0] << 8 | data_lsb[0] ) >> 4;
}

uint16_t gyro_config()
{

    gyro_write(CTRL_REG1_21002, 0x02);
    return 0;
}
