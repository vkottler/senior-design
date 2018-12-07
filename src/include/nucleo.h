#pragma once

// LD2 on GPIOA 5

#define LED_PORT        GPIOA
#define LED_PIN         5
#define LED_PIN_MASK    (0x1 << LED_PIN)

void nucleo_io_init(void);
void nucleo_blink_handler(unsigned int blink_int);
void nucleo_periph_init(void);
void nucleo_set_clks(void);
