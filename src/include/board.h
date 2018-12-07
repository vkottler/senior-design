#ifndef _BOARD__H__
#define _BOARD__H__

#include <stdbool.h>
#include <stdint.h>
#include <handlers.h>

#include "telemetry.h"

#define RADIO_FD 55

#define MAX_CHANNEL_COUNT   64
#define MAX_PACKET_COUNT     4

void io_init(void);
int periph_init(void);
void manifest_init(void);
void delay(uint32_t ms);
void blink_handler(unsigned int blink_int);
void telem_handler(unsigned int interval);

extern channel_manifest_t manifest;
extern channel_t channels[MAX_CHANNEL_COUNT];
extern telemetry_packet_t *packets[MAX_PACKET_COUNT];

int write_frame(frame_type_t frame_type, const char *buf, size_t count);

void command_reset_spi1_cs(void);
void command_set_spi1_cs(void);

#endif
