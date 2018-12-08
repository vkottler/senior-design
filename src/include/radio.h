#pragma once

#define RADIO_SILENCE_BYTE  0xAA
#define RADIO_SILENCE_TICKS 100

extern volatile uint32_t radio_resume;
extern volatile bool radio_transmit_state;
extern volatile bool radio_buffer_full;

void update_radio_state(void);
