#ifndef GYRO_H
#define GYRO_H

#include <stdio.h>


#define GYRO_ADDR 0x20
#define WATERMARK_SIZE 8

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

/* D_STATUS ******************************************************************/
// DATA STATUS
#define ZYXOW_21002             0x80
#define ZOW_21002               0x40
#define YOW_21002               0x20
#define XOW_21002               0x10
#define ZYXDR_21002             0x08
#define ZDR_21002               0x04
#define YDR_21002               0x02
#define XDR_21002               0x01

/* F_STATUS ******************************************************************/
// FIFO STATUS
#define F_OVF_21002             0x80
#define FWMKF_21002             0x40
#define F_CNT_21002             0x3f

/* F_SETUP *******************************************************************/
// FIFO MODE
#define F_MODE_DISABLED_21002   0x00
#define F_MODE_CIRCULAR_21002   0x40
#define F_MODE_STOP_21002       0x80

/* F_EVENT *******************************************************************/
#define F_EVENT_DETECTED_21002  0x20 // F_OVF OR FWMKF
#define FE_TIME_21002           0x1f

/* INT_SOURCE_FLAG ***********************************************************/
#define BOOTEND_21002           0x08
#define SRC_FIFO_21002          0x04
#define SRC_RT_21002            0x02
#define SRC_DRDY_21002          0x01

/* CTRL_REG0 *****************************************************************/
// Full Scale Range
#define FS_2000_DPS_21002       0x00 // 62.5 mdps/LSB
#define FS_1000_DPS_21002       0x01 // 31.25 mdps/LSB
#define FS_500_DPS_21002        0x02 // 15.625 mdps/LSB
#define FS_250_DPS_21002        0x03 // 7.8125 mdps/LSB

/* RT_CFG ********************************************************************/
#define ELE_EN_21002            0x08
#define ELE_DISABLE_21002       0x00
#define ZTEFE_EN_21002          0x04
#define ZTEFE_DISABLE_21002     0x00
#define YTEFE_EN_21002          0x02
#define YTEFE_DISABLE_21002     0x00
#define XTEFE_EN_21002          0x01
#define XTEFE_DISABLE_21002     0x00

/* RT_CFG ********************************************************************/
#define EA_RT_FLAG_21002        0x40
#define ZRT_DETECTED_21002      0x20
#define ZRT_POL_NEG_21002       0x10
#define YRT_DETECTED_21002      0x08
#define YRT_POL_NEG_21002       0x04
#define XRT_DETECTED_21002      0x02
#define XRT_POL_NEG_21002       0x01

/* CTRL_REG1 *****************************************************************/
// Software Reset
#define SOFT_RESET_21002        0x40

// Self-Test Enable
#define SELF_TEST_EN_21002      0x20

// Digital Output Rate
#define ODR_800_HZ_21002        0x00
#define ODR_400_HZ_21002        0x04
#define ODR_200_HZ_21002        0x08
#define ODR_100_HZ_21002        0x0C
#define ODR_50_HZ_21002         0x10
#define ODR_25_HZ_21002         0x14
#define ODR_12_5_HZ_21002       0x18

// Mode
#define MODE_STANDBY_21002      0x00
#define MODE_READY_21002        0x01
#define MODE_ACTIVE_21002       0x03

/* CTRL_REG2 *****************************************************************/
// Default off and routed to INT2
// FIFO Interrupt
#define INT1_CFG_FIFO_21002      0x80
#define INT2_CFG_FIFO_21002      0x00
#define INT_EN_FIFO_21002        0x40

//Rate Threshold interrupt
#define INT1_CFG_RT_21002        0x20
#define INT2_CFG_RT_21002        0x00
#define INT_EN_RT_21002          0x10

// Data Ready interrupt
#define INT1_CFG_DRDY_21002      0x08
#define INT2_CFG_DRDY_21002      0x00
#define INT_EN_DRDY_21002        0x04

// Interrypt logic polarity default active low
#define IPOL_ACTIVE_LOW_21002    0x00
#define IPOL_ACTIVE_HIGH_21002   0x02
// Output driver config default PUSH_PULL
#define PP_OD_PUSH_PULL_21002    0x00
#define PP_OD_OPEN_DRAIN_21002   0x01

/* CTRL_REG3 *****************************************************************/
#define WRAPTOONE_21002          0x08
#define EXTCTRLEN_21002          0x04 // INT2 is input
#define FS_DOUBLE_21002          0x01

void gyro_read_xyz();
int16_t gyro_read_x();
int16_t gyro_read_y();
int16_t gyro_read_z();
uint8_t gyro_who_am_i();
uint16_t gyro_config();
void gyro_read_fifo();
void gyro_int1_callback();
void gyro_int2_callback();

#endif /* GYRO_H */
