#include "stm32f303xe.h"
#include <stdio.h>
#include "board.h"
#include "gpio.h"
#include "usart.h"

int io_init(void) {

	int ret = 0;

    //blinky LED2
    gpio_setClock(GPIOA, true);
    gpio_setMode(GPIOA, 5, OUTPUT);
    gpio_setSpeed(GPIOA, 5, LOW_SPEED);
    gpio_setPin(GPIOA, 5);

    // RADIO
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
    // Radio mode 0
    gpio_setClock(GPIOB, true);
    gpio_setMode(GPIOB, 8, OUTPUT);
    gpio_setSpeed(GPIOB, 8, LOW_SPEED);
    gpio_setPin(GPIOB, 8);
    // Radio mode 1
    gpio_setClock(GPIOB, true);
    gpio_setMode(GPIOB, 9, OUTPUT);
    gpio_setSpeed(GPIOB, 9, LOW_SPEED);
    gpio_setPin(GPIOB, 9);

    //USB UART
    // USART 2 TX pin A2
    gpio_setClock(GPIOA, true);
    gpio_setMode(GPIOA, 2, ALT);
    gpio_setSpeed(GPIOA, 2, MEDIUM_SPEED);
    gpio_setAlternateFunc(GPIOA, 2, 7);

    // USART 2 RX pin A3
    gpio_setClock(GPIOA, true);
    gpio_setMode(GPIOA, 3, ALT);
    gpio_setSpeed(GPIOA, 3, MEDIUM_SPEED);
    gpio_setAlternateFunc(GPIOA, 3, 7);


	return ret;
}

int periph_init(void) {
    int ret = 0;

	uint32_t init_regs[3] = {0, 0, 0};

	/* USB UART */
	init_regs[0] = USART_CR1_RXNEIE;
	
	ret += usart_config(USB_UART, SYSCLK, init_regs, 115200, true);
    printf("USB USART2 INIT\r\n");
    
	ret += usart_config(USART1, SYSCLK, init_regs, 115200, true); 
    printf("RADIO USART1 INIT\r\n");

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
        else {
            gpio_setPin(GPIOA, 5);
            _putc(USART1, true, 0xff);
            printf("%x\r\n", 0xff);
        }
    }
	prev = curr;
}
