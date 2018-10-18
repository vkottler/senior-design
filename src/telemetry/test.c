/*
 * Vaughn Kottler, 10/17/2018
 */

#include <stdio.h>
#include <stdlib.h>

#include "telemetry.h"

int main(int argc, char **argv)
{
    channel_manifest_t *manifest;
    channel_t *sample_channel;
    float sample_channel_data = 123.123;

    manifest = create_channel_manifest(TELEMETRY_CAPACITY);

    /* create sample channel */
    for (int i = 0; i < 10000; i++)
    {
        sample_channel = add_channel(manifest, "test_channel", "kg", TELEM_FLOAT,
                                     sizeof(float), &sample_channel_data);
    }
    printf("Everything worked.\n");

    return 0;
}
