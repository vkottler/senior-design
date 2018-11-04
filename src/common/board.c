#include "stm32f303xe.h"
#include "board.h"
#include "gpio.h"

int io_init(void) {

	int ret = 0;
    gpio_setClock(GPIOA, true);
    gpio_setMode(GPIOA, 5, OUTPUT);
    gpio_setSpeed(GPIOA, 5, LOW_SPEED);
    gpio_setPin(GPIOA, 5);

    // USART 1 TX pin A9
    gpio_setClock(GPIOA, true);
    gpio_setMode(GPIOA, 9, ALT);
    gpio_setSpeed(GPIOA, 9, MEDIUM_SPEED);
    gpio_setAlternateFunc(GPIOA, 9, 7);

    // USART 1 RX pin A10
    gpio_setClock(GPIOA, true);
    gpio_setMode(GPIOA, 10, ALT);
    gpio_setSpeed(GPIOA, 10, MEDIUM_SPEED);
    gpio_setAlternateFunc(GPIOA, 10, 7);

    gpio_setPin(GPIOA, 5);

	return ret;
}

int periph_init(void) {
    int ret = 0;

	return ret;
}


void fault(void) {

	while (1) {
		blink_handler(50);
	}
}

void blink_handler(unsigned int blink_int) {

	static unsigned int curr = 0, prev = 0;

	curr = ticks / blink_int;
	if (curr != prev)
    {
        if (curr % 2) gpio_resetPin(GPIOA, 5);
        else          gpio_setPin(GPIOA, 5);
    }
	prev = curr;
}
