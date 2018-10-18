/*
 * Vaughn Kottler, 10/17/2018
 */

#include <stdio.h>
#include <stdlib.h>

#include "telemetry.h"

telemetry_packet_t *create_telemetry_packet(channel_t *channels,
                                            unsigned int count)
{
    int i;
    size_t data_size = 0;

    /* construct packet */
    telemetry_packet_t *packet = malloc(sizeof(telemetry_packet_t));
    if (!packet)
    {
        telemetry_debug("%s: malloc failed\n", __func__);
        return NULL;
    }
    packet->channel_count = count;

    /* initialize manifest indices */
    packet->manifest_indices = calloc(count, sizeof(unsigned int));
    if (!packet->manifest_indices)
    {
        free(packet);
        telemetry_debug("%s: calloc failed\n", __func__);
        return NULL;
    }
    for (i = 0; i < count; i++)
    {
        packet->manifest_indices[i] = channels[i].manifest_index;
        data_size += channels[i].size;
    }

    /* create the data blob */
    packet->data = malloc(data_size);
    if (!packet->data)
    {
        free(packet->manifest_indices);
        free(packet);
        return NULL;
    }
    packet->data_size = data_size;

    return packet;
}

channel_manifest_t *create_channel_manifest(unsigned int capacity)
{
    channel_manifest_t *manifest = malloc(sizeof(channel_manifest_t));
    if (!manifest)
    {
        telemetry_debug("%s: malloc failed\n", __func__);
        return NULL;
    }
    manifest->count = 0;
    manifest->channels = calloc(capacity, sizeof(channel_t));
    if (!manifest->channels)
    {
        free(manifest);
        telemetry_debug("%s: calloc failed\n", __func__);
        return NULL;
    }
    manifest->capacity = capacity;
    return manifest;
}

channel_t *add_channel(channel_manifest_t *manifest,
                       const char *name, const char *unit,
                       channel_data_t type, size_t size, void *data)
{
    /* initialize channel */
    channel_t *channel = &manifest->channels[manifest->count];
    channel->manifest_index = manifest->count++;
    channel->name = name;
    channel->unit = unit;
    channel->type = type;
    channel->size = size;
    channel->data = data;

    /* handle manifest capacity */
    if (manifest->count == manifest->capacity)
    {
        manifest->channels = realloc(manifest->channels,
                                     manifest->capacity * 2 * sizeof(channel_t));
        if (!manifest->channels)
        {
            telemetry_debug("%s: realloc failed\n", __func__);
            return NULL;
        }
        manifest->capacity = manifest->capacity * 2;
    }

    return channel;
}
