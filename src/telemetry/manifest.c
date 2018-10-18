/*
 * Vaughn Kottler, 10/18/2018
 */

#include "telemetry.h"

#include <stdio.h>
#include <stdlib.h>

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
