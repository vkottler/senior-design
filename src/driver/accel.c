#include "accel.h"
#include "i2c.h"
#include "board.h"
#include <string.h>

void accel_write(uint8_t reg, uint8_t data)
{
    delay(10);
    uint8_t data_send[2];
    data_send[0] = reg;
    data_send[1] = data;
    i2c_write(ACCEL_ADDR, data_send, 2, 1);
}

void accel_read(uint8_t reg, uint8_t *data)
{
    delay(10);
    data[0] = reg;
    i2c_write(ACCEL_ADDR, data, 1, 0);
    i2c_read(ACCEL_ADDR, data, 1);
}

void accel_read_multiple(uint8_t reg, uint8_t *data, uint16_t size)
{
    data[0] = reg;
    i2c_write(ACCEL_ADDR, data, 1, 0);
    i2c_read(ACCEL_ADDR, data, size);
}

uint8_t accel_who_am_i()
{
    uint8_t data[1];
    accel_read(WHO_AM_I_8652, data);
    return data[0];
}

int16_t accel_read_x()
{
    uint8_t data_msb[1];
    accel_read(OUT_X_MSB_8652, data_msb);

    uint8_t data_lsb[1];
    accel_read(OUT_X_LSB_8652, data_lsb);

    return (int16_t)(data_msb[0] << 8 | data_lsb[0] ) >> 4;
}

int16_t accel_read_y()
{
    uint8_t data_msb[1];
    accel_read(OUT_Y_MSB_8652, data_msb);

    uint8_t data_lsb[1];
    accel_read(OUT_Y_LSB_8652, data_lsb);

    return (int16_t)(data_msb[0] << 8 | data_lsb[0] ) >> 4;
}

int16_t accel_read_z()
{
    uint8_t data_msb[1];
    accel_read(OUT_Z_MSB_8652, data_msb);

    uint8_t data_lsb[1];
    accel_read(OUT_Z_LSB_8652, data_lsb);

    return (int16_t)(data_msb[0] << 8 | data_lsb[0] ) >> 4;
}


void accel_calibrate()
{
    int16_t x_offset;
    int16_t y_offset;
    int16_t z_offset;
    int16_t x_data = accel_read_x();
    int16_t y_data = accel_read_y();
    int16_t z_data = accel_read_z();

    accel_write(CTRL_REG1_8652, 0x00);          // Standby mode     

    x_offset = x_data / 2 * (-1);          // Compute X-axis offset correction value
    y_offset = y_data / 2 * (-1);          // Compute Y-axis offset correction value
    z_offset = (z_data - 1000) / 2 * (-1);         // Compute Z-axis offset correction value
    accel_write(OFF_X_8652, x_offset);            
    accel_write(OFF_Y_8652, y_offset);      
    accel_write(OFF_Z_8652, z_offset);      

    accel_write(CTRL_REG1_8652, 0x01);
}

uint16_t accel_config()
{
    accel_write(CTRL_REG2_8652, 0x40); // reset registers

    delay(10);


    accel_write(XYZ_DATA_CFG_8652, 0x00);   // full scale range +- 2g range  no high pass filter

    accel_write(CTRL_REG2_8652, 0x02);          // High Resolution mode

    accel_write(CTRL_REG3_8652, 0x00);          // Push-pull, active low interrupt

    accel_write(CTRL_REG4_8652, 0x00);          // NO interrupt

    accel_write(CTRL_REG5_8652, 0x00);          // NO interrupt config 

    accel_write(CTRL_REG1_8652, 0x01);// Active mode       
    accel_calibrate();
    return 0;
}
