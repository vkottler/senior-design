#pragma once

#include <stdio.h>

#include "stateful_spi.h"

/*****************************************************************************/

#define GYRO_SENSITIVITY    (7.8125f / 1000.0f)
#define GYRO_DATA_RATE      (800.0f)
#define GYRO_FIFO_WATERMARK 16

/* these were derived manually */
#define GYRO_X_BIAS (0.000075f)
#define GYRO_Y_BIAS (0.000075f)
#define GYRO_Z_BIAS (0.000075f)

/* make sure we calibrate for a watermark-divisible amount of samples */
#define GYRO_CALIB_SAMPLES  (75 * GYRO_FIFO_WATERMARK)

typedef enum gyro_state
{
    GYRO_NOT_INITED      = 0,
    GYRO_NOT_CALIB       = 1,
    GYRO_BUFFERING       = 2,
    GYRO_GETTING_SAMPLES = 3,
} gyro_state_t;

/*
 * One sample is three 16-bit integers, one for each axis.
 * Want to be able to read as many samples as we configure for the
 * watermark.
 *
 * One issue with this is that if we trigger an overflow, we will need to
 * read all of the samples from the buffer at once.
 *
 * Also add one to make sure we read the status register each time and one to
 * account for the read command.
 */
#define GYRO_SAMPLE_SIZE (3 * sizeof(int16_t))
#define GYRO_BUFFER_SIZE ((32 * GYRO_SAMPLE_SIZE) + 2)

typedef struct gyro
{
    volatile gyro_state_t state;
    volatile bool new_data;
    size_t calib_samples, samples;

    float calib_offset[3];
    float accum[3];

    uint8_t write_buffer[GYRO_BUFFER_SIZE];
    uint8_t read_buffer[GYRO_BUFFER_SIZE];

    uint32_t samples_start_t;
    int drifts_applied[3];
    spi_transaction_t sample;
    spi_state_t *spi;

    uint8_t status;
} gyro_t;

extern gyro_t gyro;

void service_gyro(gyro_t *gyro);
int gyro_config(void);
void dump_gyro(gyro_t *gyro);

#define GYRO_WHOAMI_VAL          0xD7
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
#define INT_SRC_FLAG_21002       0x0B
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
#define LPF_BW_HIGH_21002       0x80
#define LPF_BW_MID_21002        0x40
#define LPF_BW_LOW_21002        0x00
#define HPF_BW_ODR_DIV50        0x00
#define HPF_BW_ODR_DIV100       0x08
#define HPF_BW_ODR_DIV200       0x10
#define HPF_BW_ODR_DIV400       0x18
#define HPF_EN_21002            0x04

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
