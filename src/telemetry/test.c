/*
 * Vaughn Kottler, 10/17/2018
 */

#include "telemetry.h"

/*
 * Test program that executes some telemetry code.
 */
int main(int argc, char **argv)
{
    channel_manifest_t *manifest;
    telemetry_packet_t **packets;
    size_t npackets;
    int i;

    /* create channel manifest */
    manifest = channel_manifest_create(TELEMETRY_CAPACITY);
    if (!manifest) return 1;

    /* create sample channels */
    for (i = 0; i < 1000; i++)
    {
        if (i % 2)
            channel_add(manifest, "test_float", "kg",
                        TELEM_FLOAT, sizeof(float));
        else
            channel_add(manifest, "test_channel", "N/A",
                        TELEM_STRING, 256);
    }

    /* create telemetry packets for these channels */
    packets = telemetry_packets_from_manifest(manifest, TELEMETRY_MTU, &npackets);
    if (!packets) return 1;
    printf("%u packets created.\r\n", npackets);
    for (i = 0; i < npackets; i++)
        telemetry_packet_print(stdout, packets[i]);

    /* set data values */
    for (i = 0; i < manifest->count; i++)
    {
        if (i % 2)
            *((float *) manifest->channels[i].data) = (float) i;
        else
            manifest->channels[i].data = "Hello I am String data.";
    }

    /* print manifest */
    for (i = 0; i < 10; i++)
        channel_print(stdout, &manifest->channels[i]);

    return 0;
}
