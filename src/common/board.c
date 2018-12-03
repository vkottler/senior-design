#include "stm32f303xe.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "gpio.h"
#include "usart.h"

#include "gpio_alias.h"
#include "cli.h"
#include "board.h"
#include "board_config.h"

#include "accel.h"
#include "gyro.h"
#include "esc.h"


void delay(uint32_t ms)
{
    int prev = ticks;
    while(prev + ms  > ticks) {;}
}

int io_init(void) {

    int ret = 0;
    gpioAliasInit();
    // RADIO clear mode pins normal mode
    gpio_resetPin(GPIOB, 10);
    gpio_resetPin(GPIOB, 11);
    return ret;
}

int periph_init(void) {
    int ret = 0;

    uint32_t init_regs[3] = {0, 0, 0};

    /* USB UART */
    init_regs[0] = USART_CR1_RXNEIE;

    ret += usart_config(USART1, SYSCLK, init_regs, 115200, true);
    printf("RADIO USART1 INIT\r\n");
    ret += usart_config(USART2, SYSCLK, init_regs, 115200, true);
    printf("LIDAR1 USART2 INIT\r\n");
    ret += usart_config(USART3, SYSCLK, init_regs, 115200, true);
    printf("LIDAR2 USART3 INIT\r\n");

    spi_config();
    printf("SPI INIT\r\n");

    gyro_config();
    printf("gyro INIT\r\n");

    adc_config();
    adc_activate();
    printf("adc INIT\r\n");

    esc_config();
    printf("esc INIT\r\n");

    printf("System Core Clock: %ld\r\n", SystemCoreClock);
    printPrompt();

    return ret;
}


