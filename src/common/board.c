#include "stm32f303xe.h"
#include <stdio.h>
#include "board.h"
#include "gpio.h"
#include "usart.h"
#include "i2c.h"
#include "accel.h"

void delay(uint32_t ms)
{
    int prev = ticks;
    while(prev + ms  > ticks) {;}
}

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
    gpio_setClock(GPIOC, true);
    gpio_setMode(GPIOC, 4, OUTPUT);
    gpio_setSpeed(GPIOC, 4, LOW_SPEED);
    gpio_resetPin(GPIOC, 4);
    // Radio mode 1
    gpio_setClock(GPIOB, true);
    gpio_setMode(GPIOB, 1, OUTPUT);
    gpio_setSpeed(GPIOB, 1, LOW_SPEED);
    gpio_resetPin(GPIOB, 1);

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

    // i2c 
    gpio_setClock(GPIOB, true);
    gpio_setMode(GPIOB, 8, ALT);
    gpio_setSpeed(GPIOB, 8, MEDIUM_SPEED);
    gpio_setAlternateFunc(GPIOB, 8, 4);

    gpio_setClock(GPIOB, true);
    gpio_setMode(GPIOB, 8, ALT);
    gpio_setSpeed(GPIOB, 8, MEDIUM_SPEED);
    gpio_setAlternateFunc(GPIOB, 8, 4);
    gpio_openDrainState(GPIOB, 8, true);
    gpio_setPullupState(GPIOB, 8, PULL_UP);

    gpio_setClock(GPIOB, true);
    gpio_setMode(GPIOB, 9, ALT);
    gpio_setSpeed(GPIOB, 9, MEDIUM_SPEED);
    gpio_setAlternateFunc(GPIOB, 9, 4);
    gpio_openDrainState(GPIOB, 9, true);
    gpio_setPullupState(GPIOB, 9, PULL_UP);

    return ret;
}

int periph_init(void) {
    int ret = 0;

    uint32_t init_regs[3] = {0, 0, 0};

    /* USB UART */
    init_regs[0] = USART_CR1_RXNEIE;

/*    ret += usart_config(USB_UART, SYSCLK, init_regs, 115200, true);*/
/*    printf("USB USART2 INIT\r\n");*/

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
    printf("0x%x\r\n", accel_who_am_i(CTRL_REG1_8652));
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
/*            _putc(USART1, true, c);*/
            printf("ACCEL\r\n");
            printf("x: %d\r\n", accel_read_x());
            printf("y: %d\r\n", accel_read_y());
            printf("z: %d\n\r\n", accel_read_z());
        }
    }
    prev = curr;
}
