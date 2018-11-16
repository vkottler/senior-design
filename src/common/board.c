#include "stm32f303xe.h"
#include <stdio.h>
#include "board.h"
#include "gpio.h"
#include "usart.h"
#include "i2c.h"
#include "timer.h"
#include "accel.h"
#include "esc.h"
#include "cli.h"
#include "gpio_alias.h"
#include <unistd.h>

void delay(uint32_t ms)
{
    int prev = ticks;
    while(prev + ms  > ticks) {;}
}

int io_init(void) {

    int ret = 0;
    gpioAliasInit();
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

    RCC->CFGR3 |= RCC_CFGR3_I2C1SW; // SYSCLK
    RCC->APB1ENR |= RCC_APB1ENR_I2C1EN;
    uint8_t prescaler = 1;
    uint8_t setup_time = 0x4;
    uint8_t hold_time = 0x2;
    uint8_t clk_high_period = 0xf;
    uint8_t clk_low_period = 0x13;
    uint32_t i2c_timing = __LL_I2C_CONVERT_TIMINGS(
            prescaler,
            setup_time,
            hold_time,
            clk_high_period,
            clk_low_period);


    LL_I2C_Disable(I2C1);
    LL_I2C_InitTypeDef handle;
    handle.AnalogFilter = LL_I2C_ANALOGFILTER_ENABLE;
    handle.DigitalFilter = 0x00;
    handle.Timing = i2c_timing;
    handle.OwnAddress1 = 0x02;
    handle.OwnAddrSize = LL_I2C_OWNADDRESS1_7BIT;
    handle.PeripheralMode = LL_I2C_MODE_I2C;
    handle.TypeAcknowledge = LL_I2C_NACK; //slave only 

    if(LL_I2C_Init(I2C1, &handle) != SUCCESS)
    {
        /* Initialization Error */
        blink_handler(50);
        printf("Error I2C init\r\n");
        return -1;
    }
    LL_I2C_Enable(I2C1);
    printf("I2C INIT\r\n");

    accel_config();
    printf("WHO AM I 0x%x\r\n", accel_who_am_i());

    esc_config();
    printf("PWM INIT\r\n");
    printf("System Core Clock: %ld\r\n", SystemCoreClock);
    printPrompt();
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
        else gpio_setPin(GPIOA, 5);
    }
    prev = curr;
}

void telem_handler(unsigned int interval) {
    static uint32_t last_tick = 0;

    if (!(ticks % interval) && ticks != last_tick)
    {
        last_tick = ticks;
        printf("ACCEL\r\n");
        printf("x: %d\r\n", accel_read_x());
        printf("y: %d\r\n", accel_read_y());
        printf("z: %d\r\n", accel_read_z());
/*        uint16_t accel_data[3];*/
/*        accel_data[0] = accel_read_x();*/
/*        accel_data[1] = accel_read_y();*/
/*        accel_data[2] = accel_read_z();*/
/*        write(RADIO_FD, (void*)accel_data, 6);*/
    }
}
