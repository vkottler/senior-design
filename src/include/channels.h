#pragma once

#include "telemetry.h"

#include <stdbool.h>

bool manifest_init(void);

#define MAX_CHANNEL_COUNT   64
#define MAX_PACKET_COUNT     8
#define TELEM_NUM_GROUPS     4

#define MAX_TELEM_PERIOD      10000
#define MIN_TELEM_PERIOD      250

#define GYRO_UPDATE_RATE      500
#define GYRO_TELEM_ON         false

#define LIDAR_UPDATE_RATE     1000 
#define LIDAR_TELEM_ON        false

#define BATTERY_UPDATE_RATE   4000
#define BATTERY_TELEM_ON      false

#define THROTTLE_UPDATE_RATE  500
#define THROTTLE_TELEM_ON     false

/* don't want to rename everything */
#define manifest tm._manifest
#define packets tm._packets

#define GYRO_X_IND  0
#define GYRO_Y_IND  1
#define GYRO_Z_IND  2

#define LIDAR1_IND  3
#define LIDAR2_IND  4

#define BATTERY_IND 5

#define POSX_IND    10
#define NEGX_IND    11
#define POSY_IND    12
#define NEGY_IND    13

#define GYRO_XA_IND 14
#define GYRO_YA_IND 15
#define GYRO_ZA_IND 16

typedef struct telemetry_group {
    const char *alias;
    uint32_t rate;
    bool publish;
} telemetry_group_t;

typedef struct telemetry_manager
{
    /* channels */
    channel_manifest_t  _manifest;
    channel_t           data_channels[MAX_CHANNEL_COUNT];

    /* packets */
    telemetry_packet_t *_packets[MAX_PACKET_COUNT];
    bool               *packet_auto_publish[MAX_PACKET_COUNT];
    uint32_t           *packet_update_rates[MAX_PACKET_COUNT];
    uint32_t            packet_last_ticks[MAX_PACKET_COUNT];
    uint32_t            packets_sent[MAX_PACKET_COUNT];
    uint32_t            packets_dropped[MAX_PACKET_COUNT];
    uint32_t            packets_not_published[MAX_PACKET_COUNT];
    uint32_t            packet_index;

    bool                publish;
    uint32_t            t_zero;
} telemetry_manager_t;

int write_frame(frame_type_t frame_type, const char *buf, size_t count);
void publish_telemetry(telemetry_manager_t *tman);
void dump_telemetry_manager(telemetry_manager_t *tman);

extern telemetry_manager_t tm;
extern volatile uint32_t ticks;
extern telemetry_group_t packet_groups[TELEM_NUM_GROUPS];
