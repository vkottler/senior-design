#ifndef ACCEL_H
#define ACCEL_H

#include <stdio.h>


#define ACCEL_ADDR 0x1D

/*****************************************************************************/
/* Registers                                                                 */
/*****************************************************************************/

#define STATUS_8652             0x00
#define OUT_X_MSB_8652          0x01
#define OUT_X_LSB_8652          0x02
#define OUT_Y_MSB_8652          0x03
#define OUT_Y_LSB_8652          0x04
#define OUT_Z_MSB_8652          0x05
#define OUT_Z_LSB_8652          0x06

// 8652_RESERVED 0x07
// 8652_RESERVED 0x08

#define F_SETUP_8652            0x09
#define TRIG_CFG_8652           0x0A
#define SYSMOD_8652             0x0B
#define INT_SOURCE_8652         0x0C
#define WHO_AM_I_8652           0x0D
#define XYZ_DATA_CFG_8652       0x0E
#define HP_FILTER_CUTOFF_8652   0x0F
#define PL_STATUS_8652          0x10
#define PL_CFG_8652             0x11
#define PL_COUNT_8652           0x12
#define PL_BF_ZCOMP_8652        0x13
#define PL_L_THIS_REG_8652      0x14
#define FF_MT_CFG_8652          0x15
#define FF_MT_SRC_8652          0x16
#define FF_MT_THS_8652          0x17
#define FF_MT_COUNT_8652        0x18

// 8652_RESERVED 0x19
// 8652_RESERVED 0x1A
// 8652_RESERVED 0x1B
// 8652_RESERVED 0x1C

#define TRANSIENT_CFG_8652      0x1D
#define TRANSIENT_SRC_8652      0x1E
#define TRANSIENT_THS_8652      0x1F
#define TRANSIENT_COUNT_8652    0x20
#define PULSE_CFG_8652          0x21
#define PULSE_SRC_8652          0x22
#define PULSE_THSX_8652         0x23
#define PULSE_THSY_8652         0x24
#define PULSE_THSZ_8652         0x25
#define PULSE_TMLT_8652         0x26
#define PULSE_LTCY_8652         0x27
#define PULSE_WIND_8652         0x28
#define ASLP_COUNT_8652         0x29
#define CTRL_REG1_8652          0x2A
#define CTRL_REG2_8652          0x2B
#define CTRL_REG3_8652          0x2C
#define CTRL_REG4_8652          0x2D
#define CTRL_REG5_8652          0x2E
#define OFF_X_8652              0x3F
#define OFF_Y_8652              0x30
#define OFF_Z_8652              0x31

uint16_t accel_read_x();
uint16_t accel_read_y();
uint16_t accel_read_z();
uint8_t accel_who_am_i();
uint16_t accel_config();

#endif /* ACCEL_H */
