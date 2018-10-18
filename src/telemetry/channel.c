/*
 * Vaughn Kottler, 10/18/2018
 */

#include "telemetry.h"

#include <stdio.h>
#include <stdlib.h>

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
