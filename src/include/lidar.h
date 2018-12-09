#pragma once

#include <stdbool.h>

#define TFMINI_DATA_Len             9
#define TFMINT_DATA_HEAD            0x59

typedef enum _LIDAR_ID {
    LIDAR1 = 1, LIDAR2 = 2,
} LIDAR_ID;

typedef enum lidar_states {
    frame_header1,
    frame_header2,
    dist_l,
    dist_h,
    strength_l,
    strength_h,
    reserved,
    raw_qual,
    checksum
} lidar_states_t;

typedef struct lidar {
    volatile          lidar_states_t state;
    volatile uint8_t  checksum;
    volatile uint16_t dist;
    volatile uint16_t valid_dist;
    volatile bool     new_data;
} lidar_t;

uint16_t lidar_readDist(LIDAR_ID id);
bool lidar_data_ready(LIDAR_ID id);
void lidar1_callback(char c);
void lidar2_callback(char c);
