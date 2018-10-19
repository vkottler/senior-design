/*
 * Vaughn Kottler, 10/18/2018
 */

#include <stdlib.h>

#include "telemetry.h"

/*
 * Add a channel to a manifest and return its index.
 *
 * We can't return a pointer to a channel_t because if we have to realloc,
 * all existing channel_t references that came from here become invalid
 * (i.e. OS moves manifest->channels to a new page).
 */
uint32_t channel_add(channel_manifest_t *manifest,
                     const char *name, const char *unit,
                     channel_data_t type, size_t size)
{
    uint32_t index = manifest->count;
    channel_t *channel = &manifest->channels[index];

    /* initialize channel */
    channel->manifest_index = index;
    channel->name = name;
    channel->unit = unit;
    channel->type = type;
    channel->size = size;
    channel->data = NULL;

    /* handle manifest capacity */
    if (++manifest->count == manifest->capacity)
    {
        manifest->channels = realloc(manifest->channels,
                                     manifest->capacity * 2 * sizeof(channel_t));
        if (!manifest->channels)
        {
            telemetry_debug("%s: realloc failed\r\n", __func__);
            return -1;
        }
        manifest->capacity = manifest->capacity * 2;
    }

    return index;
}

/*
 * Convert the channel-data-type enum into a String.
 */
const char *channel_type_to_str(channel_data_t type)
{
    switch (type)
    {
        case TELEM_UNDEF:  return "UNDEF";
        case TELEM_INT8:   return "INT8";
        case TELEM_UINT8:  return "UINT8";
        case TELEM_INT16:  return "INT16";
        case TELEM_UINT16: return "UINT16";
        case TELEM_INT32:  return "INT32";
        case TELEM_UINT32: return "UINT32";
        case TELEM_FLOAT:  return "FLOAT";
        case TELEM_STRING: return "STRING";
    }
    return NULL;
}

/*
 * Print a channel's data to the given IO stream.
 */
void channel_print_data(FILE *stream, channel_t *channel)
{
    fputs("Data: ", stream);
    switch (channel->type)
    {
        case TELEM_UNDEF:  fputs("N/A", stream); break;
        case TELEM_INT8:   fprintf(stream, "%d",   *((int8_t *) channel->data)); break;
        case TELEM_UINT8:  fprintf(stream, "%u",   *((uint8_t *) channel->data)); break;
        case TELEM_INT16:  fprintf(stream, "%d",   *((int16_t *) channel->data)); break;
        case TELEM_UINT16: fprintf(stream, "%u",   *((uint16_t *) channel->data)); break;
        case TELEM_INT32:  fprintf(stream, "%d",   *((int32_t *) channel->data)); break;
        case TELEM_UINT32: fprintf(stream, "%u",   *((uint32_t *) channel->data)); break;
        case TELEM_FLOAT:  fprintf(stream, "%.2f", *((float *) channel->data)); break;
        case TELEM_STRING: fputs((const char *) channel->data, stream); break;
    }
    fputs("\r\n", stream);
}

/*
 * Print a channel to the given IO stream.
 */
void channel_print(FILE *stream, channel_t *channel)
{
    fputs("====================\r\n", stream);
    fprintf(stream, "Name: %s (%u)\r\n", channel->name, channel->manifest_index);
    fprintf(stream, "Unit: %s\r\n", channel->unit);
    fprintf(stream, "Type: %s\r\n", channel_type_to_str(channel->type));
    fprintf(stream, "Size: %u\r\n", channel->size);
    channel_print_data(stream, channel);
    fputs("====================\r\n", stream);
}
