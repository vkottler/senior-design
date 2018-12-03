/*
 * Vaughn Kottler, 08/04/2018
 */

#pragma once

#include <stdint.h>
#include <stdbool.h>

/*******************************************************************/
/*                        Configurations                           */
/*******************************************************************/

#define control_debug   printf

#ifndef SYSTEM_RATE
#define SYSTEM_RATE    20
#endif

#define GRAVITY_m       9.80665f
#define GRAVITY_mm      (GRAVITY_m*1000.0f)

/* sensor parameters */
#define IMU_MIN_RATE    1.56f
#define IMU_MAX_RATE    800.0f
#define IMU_BITS        16
#define IMU_NUM_RANGES  3
#define IMU_FIFO_DEPTH  32

#define GYRO_MIN_RATE   12.5f
#define GYRO_MAX_RATE   800.0f
#define GYRO_BITS       16
#define GYRO_NUM_RANGES 4
#define GYRO_FIFO_DEPTH 32

/*******************************************************************/


/*******************************************************************/
/*                           Data Types                            */
/*******************************************************************/

typedef struct _motor_throttles {
    uint8_t pos_x, neg_x, pos_y, neg_y;
} motor_throttles_t;

typedef struct __attribute__((__packed__)) _imu_data {
    int16_t ax, ay, az;
} imu_data_t;

typedef struct __attribute__((__packed__)) _gyro_data {
    int16_t vx, vy, vz;
} gyro_data_t;

typedef struct _motion_configuration {
    float imu_rate, gyro_rate;
    float imu_sample_period, gyro_sample_period;
    float imu_lsb, gyro_lsb;
} motion_configuration_t;

typedef struct _motion_state {

    /* rolling average raw data storage */
    unsigned int imu_roll_len, gyro_roll_len;
    unsigned int imu_curr, gyro_curr;
    int16_t *roll_ax, *roll_ay, *roll_az;
    int16_t *roll_gx, *roll_gy, *roll_gz;

    /* averaged measurements */
    int16_t avg_ax, avg_ay, avg_az;
    int16_t avg_gx, avg_gy, avg_gz;
    
    /* accumulated measurements */
    int16_t acc_ax, acc_ay, acc_az;
    int16_t acc_gx, acc_gy, acc_gz;

    /* requested state */
    //float set_vx, set_vy, set_vz;
    //float set_degx, set_degy, set_degz;
} motion_state_t;

/*******************************************************************/


/*******************************************************************/
/*                     Global Variables                            */
/*******************************************************************/

extern gyro_data_t gyro_data[GYRO_FIFO_DEPTH];
extern imu_data_t  imu_data[IMU_FIFO_DEPTH];

/*******************************************************************/


/*******************************************************************/
/*                          Public API                             */
/*******************************************************************/

bool control_init(uint8_t gyro_range_idx, uint8_t imu_range_idx,
                  motion_state_t *state,
                  motion_configuration_t *config,
                  float imu_rate, float gyro_rate);

bool control_loop( int32_t input_roll, int32_t input_pitch,
                   int32_t input_yaw, int16_t input_thrst );
/*******************************************************************/
