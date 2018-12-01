#include "stm32f303xe.h"
#include <stdio.h>
#include "board.h"
#include "gpio.h"
#include "usart.h"
#include "i2c.h"
#include "spi.h"
#include "timer.h"
#include "accel.h"
#include "gyro.h"
#include "esc.h"
#include "cli.h"
#include "gpio_alias.h"
#include <string.h>
#include <unistd.h>

void i2c_config();
void spi_config();

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

    spi_config();
    printf("SPI INIT\r\n");


    gyro_config();
/*    printf("X: 0x%x\r\n", gyro_read_x());*/
    printf("System Core Clock: %ld\r\n", SystemCoreClock);
    printPrompt();

    return ret;
}


void i2c_config()
{

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
        return;
    }

  LL_I2C_Enable(I2C1);
}

void spi_config()
{
    NVIC_SetPriority(SPI1_IRQn, 0);
    NVIC_EnableIRQ(SPI1_IRQn);

    LL_SPI_InitTypeDef SPI_InitStruct;

    RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;

    SPI_InitStruct.TransferDirection = LL_SPI_FULL_DUPLEX;
    SPI_InitStruct.Mode = LL_SPI_MODE_MASTER;
    SPI_InitStruct.DataWidth = LL_SPI_DATAWIDTH_8BIT;
    SPI_InitStruct.ClockPolarity = LL_SPI_POLARITY_LOW;
    SPI_InitStruct.ClockPhase = LL_SPI_PHASE_1EDGE;
    SPI_InitStruct.NSS = LL_SPI_NSS_HARD_OUTPUT;
    SPI_InitStruct.BaudRate = LL_SPI_BAUDRATEPRESCALER_DIV8;
    SPI_InitStruct.BitOrder = LL_SPI_MSB_FIRST;
    SPI_InitStruct.CRCCalculation = LL_SPI_CRCCALCULATION_DISABLE;
    SPI_InitStruct.CRCPoly = 7;
    LL_SPI_Init(SPI1, &SPI_InitStruct);
    LL_SPI_EnableIT_RXNE(SPI1);
    LL_SPI_EnableIT_TXE(SPI1);
    LL_SPI_EnableIT_ERR(SPI1);
}


