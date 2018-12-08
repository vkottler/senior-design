#pragma once

#include "telemetry.h"

#include <stdbool.h>

bool manifest_init(void);

#define MAX_CHANNEL_COUNT   64
#define MAX_PACKET_COUNT     8

#define GYRO_UPDATE_RATE      666
#define LIDAR_UPDATE_RATE     666
#define BATTERY_UPDATE_RATE   2000
#define THROTTLE_UPDATE_RATE  2000

/* don't want to rename everything */
#define manifest tm._manifest
#define packets tm._packets

typedef struct telemetry_manager
{
    /* channels */
    channel_manifest_t  _manifest;
    channel_t           data_channels[MAX_CHANNEL_COUNT];

    /* packets */
    telemetry_packet_t *_packets[MAX_PACKET_COUNT];
    bool                packet_auto_publish[MAX_PACKET_COUNT];
    uint32_t            packet_update_rates[MAX_PACKET_COUNT];
    uint32_t            packet_last_ticks[MAX_PACKET_COUNT];
    uint32_t            packets_sent[MAX_PACKET_COUNT];
    uint32_t            packets_dropped[MAX_PACKET_COUNT];
    uint32_t            packet_index;

    bool                publish;
    uint32_t            t_zero;
} telemetry_manager_t;

int write_frame(frame_type_t frame_type, const char *buf, size_t count);
void publish_telemetry(telemetry_manager_t *tman);
void dump_telemetry_manager(telemetry_manager_t *tman);

extern telemetry_manager_t tm;
extern volatile uint32_t ticks;
