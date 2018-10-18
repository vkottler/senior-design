/*
 * Vaughn Kottler, 10/18/2018
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
