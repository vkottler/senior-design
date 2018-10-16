/*
 * Vaughn Kottler, 08/04/2018
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "control.h"

const float gyro_deg_per_lsb[GYRO_NUM_RANGES] =
{
    /* 250  deg/s  */ 250.0f  / (float) (2 << GYRO_BITS),
    /* 500  deg/s  */ 500.0f  / (float) (2 << GYRO_BITS),
    /* 1000 deg/s  */ 1000.0f / (float) (2 << GYRO_BITS),
    /* 2000 deg/s  */ 2000.0f / (float) (2 << GYRO_BITS),
};

/* total_range / 2^N where the measurements are N bits */
const float imu_mmss_per_lsb[IMU_NUM_RANGES] =
{
    /* +/- 2g */  ((4.0f*GRAVITY_mm) / (float) (2 << IMU_BITS)),
    /* +/- 4g */  ((8.0f*GRAVITY_mm) / (float) (2 << IMU_BITS)),
    /* +/- 8g */ ((16.0f*GRAVITY_mm) / (float) (2 << IMU_BITS)),
};
const float imu_mss_per_lsb[IMU_NUM_RANGES] =
{
    /* +/- 2g */  ((4.0f*GRAVITY_m) / (float) (2 << IMU_BITS)),
    /* +/- 4g */  ((8.0f*GRAVITY_m) / (float) (2 << IMU_BITS)),
    /* +/- 8g */ ((16.0f*GRAVITY_m) / (float) (2 << IMU_BITS)),
};

/* want to set up DMA into these arrays from I2C / SPI */
gyro_data_t gyro_data[GYRO_FIFO_DEPTH];
imu_data_t  imu_data[IMU_FIFO_DEPTH];

bool control_init(uint8_t gyro_range_idx, uint8_t imu_range_idx,
                  motion_state_t *state,
                  motion_configuration_t *config,
                  float imu_rate, float gyro_rate)
{
    unsigned int temp_uint;

    /* check requested sensitivity */
    if (gyro_range_idx >= GYRO_NUM_RANGES ||
        imu_range_idx  >= IMU_NUM_RANGES)
    {
        control_debug("%s: Impossible gyro/imu range: %u (max %u), %u (max %u)\r\n",
                      __func__,
                      gyro_range_idx, GYRO_NUM_RANGES,
                      imu_range_idx,  IMU_NUM_RANGES);
        return false;
    }

    /* check requested sample rates */
    if (imu_rate < IMU_MIN_RATE || imu_rate > IMU_MAX_RATE)
    {
        control_debug("%s: Impossible imu rate: %.1f (%.1f - %.1f)\r\n",
                      __func__, imu_rate, IMU_MIN_RATE, IMU_MAX_RATE);
        return false;
    }
    if (gyro_rate < GYRO_MIN_RATE || gyro_rate > GYRO_MAX_RATE)
    {
        control_debug("%s: Impossible gyro rate: %.1f (%.1f - %.1f)\r\n",
                      __func__, gyro_rate, GYRO_MIN_RATE, GYRO_MAX_RATE);
        return false;
    }

    /* initialize all values to zero */
    memset(gyro_data, 0, sizeof(gyro_data_t)*GYRO_FIFO_DEPTH);
    memset(imu_data,  0, sizeof(imu_data_t)*IMU_FIFO_DEPTH);
    memset(config,    0, sizeof(motion_configuration_t));
    memset(state,     0, sizeof(motion_state_t));

    /* set configuration data */
    config->imu_rate = imu_rate;
    config->gyro_rate = gyro_rate;
    config->imu_sample_period =  1.0f / config->imu_rate;
    config->gyro_sample_period = 1.0f / config->gyro_rate;
    config->imu_lsb = imu_mmss_per_lsb[imu_range_idx];
    config->gyro_lsb = gyro_deg_per_lsb[gyro_range_idx];

    /* calculate rolling average length */
    temp_uint = (unsigned int) imu_rate;
    state->imu_roll_len = temp_uint / SYSTEM_RATE;
    if (temp_uint % SYSTEM_RATE) state->imu_roll_len++;
    temp_uint = (unsigned int) gyro_rate;
    state->gyro_roll_len = temp_uint / SYSTEM_RATE;
    if (temp_uint % SYSTEM_RATE) state->gyro_roll_len++;

    /* allocate memory for rolling averages */
    state->roll_ax = calloc(state->imu_roll_len,  sizeof(int16_t));
    state->roll_ay = calloc(state->imu_roll_len,  sizeof(int16_t));
    state->roll_az = calloc(state->imu_roll_len,  sizeof(int16_t));
    state->roll_gx = calloc(state->gyro_roll_len, sizeof(int16_t));
    state->roll_gy = calloc(state->gyro_roll_len, sizeof(int16_t));
    state->roll_gz = calloc(state->gyro_roll_len, sizeof(int16_t));
    if (!state->roll_ax || !state->roll_ay || !state->roll_az ||
        !state->roll_gx || !state->roll_gy || !state->roll_gz)
    {
        control_debug("%s: A memory request for rolling average arrays failed\r\n", __func__);
        return false;
    }

    /* report initialization results */
    control_debug("System Rate:      %d.0 Hz (%.1f ms)\r\n",
                  SYSTEM_RATE, 1000.0f / (float) SYSTEM_RATE);
    control_debug("IMU  Sample Rate: %.1f Hz (%.1f ms)\r\n",
                  config->imu_rate,
                  1000.0f * config->imu_sample_period);
    control_debug("Gyro Sample Rate: %.1f Hz (%.1f ms)\r\n",
                  config->gyro_rate,
                  1000.0f * config->gyro_sample_period);
    control_debug("IMU  precision:   %.4f mm/s^2\r\n",
                  config->imu_lsb);
    control_debug("Gyro precision:   %.4f deg/s\r\n",
                  config->gyro_lsb);
    control_debug("IMU  r-avg depth: %u\r\n",
                  state->imu_roll_len);
    control_debug("Gyro r-avg depth: %u\r\n",
                  state->gyro_roll_len);

    return true;
}
