#include "accel.h"
#include "i2c.h"
#include "board.h"


uint8_t accel_who_am_i(uint8_t addr)
{
    uint8_t data[1];
    data[0] = WHO_AM_I_8652;
    i2c_write(ACCEL_ADDR, data, 1, 0);
    i2c_read(ACCEL_ADDR, data, 1);

    return data[0];
}

uint16_t accel_read_x()
{
    uint8_t data_msb[1];
    data_msb[0] = OUT_X_MSB_8652;
    i2c_write(ACCEL_ADDR, data_msb, 1, 0);
    i2c_read(ACCEL_ADDR, data_msb, 1);
    
    uint8_t data_lsb[1];
    data_lsb[0] = OUT_X_LSB_8652;
    i2c_write(ACCEL_ADDR, data_lsb, 1, 0);
    i2c_read(ACCEL_ADDR, data_lsb, 1);

    return data_msb[0] << 8 | data_lsb[0];
}

uint16_t accel_read_y()
{
    uint8_t data_msb[1];
    data_msb[0] = OUT_Y_MSB_8652;
    i2c_write(ACCEL_ADDR, data_msb, 1, 0);
    i2c_read(ACCEL_ADDR, data_msb, 1);
    
    uint8_t data_lsb[1];
    data_lsb[0] = OUT_Y_LSB_8652;
    i2c_write(ACCEL_ADDR, data_lsb, 1, 0);
    i2c_read(ACCEL_ADDR, data_lsb, 1);

    return data_msb[0] << 8 | data_lsb[0];
}

uint16_t accel_read_z()
{
    uint8_t data_msb[1];
    data_msb[0] = OUT_Z_MSB_8652;
    i2c_write(ACCEL_ADDR, data_msb, 1, 0);
    i2c_read(ACCEL_ADDR, data_msb, 1);
    
    uint8_t data_lsb[1];
    data_lsb[0] = OUT_Z_LSB_8652;
    i2c_write(ACCEL_ADDR, data_lsb, 1, 0);
    i2c_read(ACCEL_ADDR, data_lsb, 1);

    return data_msb[0] << 8 | data_lsb[0];
}

uint16_t accel_config()
{
    uint8_t data[2];
    data[0] = CTRL_REG1_8652;
    data[1] = 0x01;
    i2c_write(ACCEL_ADDR, data, 2, 1);
    printf("Accel Config\r\n");

    return 0;
}


