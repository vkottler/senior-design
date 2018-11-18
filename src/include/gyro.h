#ifndef GYRO_H
#define GYRO_H

#include <stdio.h>


#define GYRO_ADDR 0x20

/*****************************************************************************/
/* Registers                                                                 */
/*****************************************************************************/

#define STATUS_21002             0x00
#define OUT_X_MSB_21002          0x01
#define OUT_X_LSB_21002          0x02
#define OUT_Y_MSB_21002          0x03
#define OUT_Y_LSB_21002          0x04
#define OUT_Z_MSB_21002          0x05
#define OUT_Z_LSB_21002          0x06
#define DR_STATUS_21002          0x07
#define F_STATUS_21002           0x08
#define F_SETUP_21002            0x09
#define F_EVENT_21002            0x0A
#define INT_SRC_FLAG21002        0x0B
#define WHO_AM_I_21002           0x0C
#define CTRL_REG0_21002          0x0D
#define RT_CFG_21002             0x0E
#define RT_SRC_21002             0x0F
#define RT_THS_21002             0x10
#define RT_COUNT_21002           0x11
#define TEMP_21002               0x12
#define CTRL_REG1_21002          0x13
#define CTRL_REG2_21002          0x14
#define CTRL_REG3_21002          0x15

int16_t gyro_read_x();
int16_t gyro_read_y();
int16_t gyro_read_z();
uint8_t gyro_who_am_i();
uint16_t gyro_config();

#endif /* GYRO_H */
