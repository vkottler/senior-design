#include "stm32f407xx.h"

void blink_handler(unsigned int blink_int) {

	static unsigned int curr = 0, prev = 0;

	curr = ticks / blink_int;
	if (curr != prev)
    {
        if (curr % 2) GPIOE->BSRR |=  0x1 << 16;
        else          GPIOE->BSRR |=  0x1;
    }
	prev = curr;
}

int main(void)
{
    RCC->AHB1ENR  |= RCC_AHB1ENR_GPIOEEN;
    GPIOE->MODER  |= 0x1;
    GPIOE->OTYPER |= 0x1;
    while (1)
    {
        blink_handler(250);
    }

    return 0;
}
