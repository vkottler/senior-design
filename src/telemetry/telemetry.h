/*
 * Vaughn Kottler, 10/17/2018
 */

#pragma once

#include <stdio.h>
#include <stddef.h>
#include <stdint.h>

#define TELEMETRY_CAPACITY  64
#define TELEMETRY_MTU       1500
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
    uint32_t        manifest_index;
} channel_t;

typedef struct _channel_manifest {
    channel_t *channels;
    uint32_t   count;
    uint32_t   capacity;
} channel_manifest_t;

typedef struct __attribute__((__packed__)) _telemetry_packet {
    uint32_t  channel_count;
    size_t    data_size;
    uint32_t  crc32;
    void     *blob;
} telemetry_packet_t;

uint32_t channel_add(channel_manifest_t *manifest,
                     const char *name, const char *unit,
                     channel_data_t type, size_t size);
void channel_print(FILE *stream, channel_t *channel);

channel_manifest_t *channel_manifest_create(uint32_t capacity);
void channel_manifest_print(FILE *stream, channel_manifest_t *manifest);

telemetry_packet_t *telemetry_packet_create(channel_t *channels,
                                            uint32_t count);
telemetry_packet_t **telemetry_packets_from_manifest(channel_manifest_t *manifest,
                                                    size_t mtu, size_t *npackets);
void telemetry_packet_print(FILE *stream, telemetry_packet_t *packet);
