/*
 * Vaughn Kottler, 08/04/2018
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "control.h"

#include "gyro.h"
#include "esc.h"
#include "timer.h"

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

#define X_AXIS 0
#define Y_AXIS 1
#define Z_AXIS 2

#define ROLL_AXIS   X_AXIS
#define PITCH_AXIS  Y_AXIS
#define YAW_AXIS    Z_AXIS

int32_t raw_output_roll;
int32_t raw_output_pitch;
int32_t raw_output_yaw;

uint32_t pid_output_roll;
uint32_t pid_output_pitch;
uint32_t pid_output_yaw;

bool calc_pid()
{
    pid_output_roll = raw_output_roll;
    pid_output_pitch= raw_output_pitch;
    pid_output_yaw= raw_output_yaw;
    return true;
}

/*****************************************************************************/
/* ESC_0 (CW)    ESC_1 (CCW)                                                 */
/*      \       /                                                            */
/*       \     /                                                             */
/*        \   /                     |Y+                                      */
/*         \ /                      |                                        */
/*          x                    Z+ o___X+                                   */
/*         / \                                                               */
/*        /   \                                                              */
/*       /     \                                                             */
/*      /       \                                                            */
/* ESC_2 (CCW)   ESC_3 (CW)                                                  */
/*****************************************************************************/
bool control_loop( int32_t input_roll, int32_t input_pitch,
                   int32_t input_yaw, int16_t input_thrst )
{
    int16_t gyro_data[3];
    uint16_t esc_front_left_val;
    uint16_t esc_front_right_val;
    uint16_t esc_back_left_val;
    uint16_t esc_back_right_val;

    // SYSTEM_RATE loop
    static uint32_t last_tick = 0;

    if (!(ticks % 2000) && ticks != last_tick)
    {
/*    while(1)*/
/*    {*/
        // Wait for interrupt on gyro or timeout
        /*    while(!gpio_readPin(GPIOB, 5)) { }*/

        // Read Gyro
        gyro_data[X_AXIS] = gyro_read_x() / 10;
        gyro_data[Y_AXIS] = gyro_read_y() / 10;
        gyro_data[Z_AXIS] = gyro_read_z() / 10;
        printf("\r\nGyro\r\n");
        printf("Gyro X %d\r\n", gyro_data[X_AXIS]);
        printf("Gyro Y %d\r\n", gyro_data[Y_AXIS]);
        printf("Gyro Z %d\r\n", gyro_data[Z_AXIS]);

        // Calculate difference Roll, Pitch and Yaw
        raw_output_roll = gyro_data[ROLL_AXIS] - input_roll;
        raw_output_pitch =  gyro_data[PITCH_AXIS] - input_pitch;
        raw_output_yaw =  gyro_data[YAW_AXIS] - input_yaw;

        // PID tunning
        calc_pid();
        printf("PID\r\n");
        printf("pid_output_roll %ld\r\n", pid_output_roll);
        printf("pid_output_pitch %ld\r\n", pid_output_pitch);
        printf("pid_output_yaw %ld\r\n", pid_output_yaw);

        // Calculate pulse
        esc_front_left_val = input_thrst - pid_output_roll - pid_output_pitch + pid_output_yaw;
        esc_front_right_val = input_thrst + pid_output_roll - pid_output_pitch - pid_output_yaw;
        esc_back_left_val = input_thrst - pid_output_roll + pid_output_pitch - pid_output_yaw;
        esc_back_right_val = input_thrst + pid_output_roll + pid_output_pitch + pid_output_yaw;

        printf("ESC\r\n");
        printf("front_left %d\r\n", esc_front_left_val);
        printf("front_right %d\r\n", esc_front_right_val);
        printf("back_left %d\r\n", esc_back_left_val);
        printf("back_right %d\r\n", esc_back_right_val);

        // Write Motors
        esc_set_pulse(ESC_TABLE[0], esc_front_left_val);
        esc_set_pulse(ESC_TABLE[1], esc_front_right_val);
        esc_set_pulse(ESC_TABLE[2], esc_back_left_val);
        esc_set_pulse(ESC_TABLE[3], esc_back_right_val);
    }
    return true;

}
