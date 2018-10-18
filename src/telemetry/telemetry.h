/*
 * Vaughn Kottler, 10/17/2018
 */

#pragma once

#define TELEMETRY_CAPACITY  64
#define telemetry_debug     printf

typedef enum _channel_data {
    TELEM_UNDEF  = 0,
    TELEM_INT8   = 1,
    TELEM_UINT8  = 2,
    TELEM_INT16  = 3,
    TELEM_UINT16 = 4,
    TELEM_INT32  = 5,
    TELEM_UINT32 = 6,
    TELEM_FLOAT  = 7,
    TELEM_STRING = 8
} channel_data_t;

typedef struct _channel {
    const char     *name;
    const char     *unit;
    channel_data_t  type;
    size_t          size;
    void           *data;
    unsigned int    manifest_index;
} channel_t;

typedef struct _channel_manifest {
    channel_t    *channels;
    unsigned int  count;
    unsigned int  capacity;
} channel_manifest_t;

typedef struct _telemetry_packet {
    unsigned int     channel_count;
    unsigned int    *manifest_indices;
    void            *data;
    size_t           data_size;
} telemetry_packet_t;

channel_t *add_channel(channel_manifest_t *manifest,
                       const char *name, const char *unit,
                       channel_data_t type, size_t size, void *data);
channel_manifest_t *create_channel_manifest(unsigned int capacity);
telemetry_packet_t *create_telemetry_packet(channel_t *channels,
                                            unsigned int count);
