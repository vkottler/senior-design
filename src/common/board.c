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
void SPI1WriteRead(uint8_t *pData, uint8_t *dstData, uint16_t len)
{
 SET_BIT(SPI1->CR1, SPI_CR1_BIDIOE);                  // Tx direction, clock off
 SET_BIT(SPI1->CR1, SPI_CR1_SPE);                     // SPI On
 for (uint16_t cnt = 0; cnt < len; cnt++ )
  {
   while ( (SPI1->SR & SPI_SR_TXE) != SPI_SR_TXE)     // Control TX fifo is empty
    {}
   LL_SPI_TransmitData8(SPI1,*pData++);
  } 
 while ( SPI1->SR & SPI_SR_BSY )                      // Control the BSY flag
  {}
 CLEAR_BIT(SPI1->CR1, SPI_CR1_SPE);                   // SPI Off
/*   printf("DR 0x%lx\r\n", SPI1->DR);*/
    *dstData = (SPI1->DR)>> 8;
}

void SPI1Write(uint8_t *pData, uint16_t len)
{
 SET_BIT(SPI1->CR1, SPI_CR1_BIDIOE);                  // Tx direction, clock off
 SET_BIT(SPI1->CR1, SPI_CR1_SPE);                     // SPI On
 for (uint16_t cnt = 0; cnt < len; cnt++ )
  {
   while ( (SPI1->SR & SPI_SR_TXE) != SPI_SR_TXE)     // Control TX fifo is empty
    {}
   LL_SPI_TransmitData8(SPI1,*pData++);
  }
 while ( SPI1->SR & SPI_SR_BSY )                      // Control the BSY flag
  {}
 CLEAR_BIT(SPI1->CR1, SPI_CR1_SPE);                   // SPI Off
}
//---------------------------------------------------------------------------------------------------
void SPI1Read(uint8_t *pData, uint16_t len)
{
/* uint32_t tmp;*/
 while ( SPI1->SR & SPI_SR_RXNE )                      // Clear FIFO
  { SPI1->DR;
  }
 CLEAR_BIT(SPI1->CR1, SPI_CR1_BIDIOE);                // Rx direction
 SET_BIT(SPI1->CR2, SPI_CR2_FRXTH);                   // 1/4 Fifo = 8-bit
 SET_BIT(SPI1->CR1, SPI_CR1_SPE);                     // SPI On
 for (uint16_t cnt = 0; cnt < len; cnt++ )
  {while ( (SPI1->SR & SPI_SR_RXNE) != SPI_SR_RXNE)
    {}
   *pData++ = LL_SPI_ReceiveData8(SPI1);
  }
 CLEAR_BIT(SPI1->CR1, SPI_CR1_SPE);                   // SPI Off
 while ( SPI1->SR & SPI_SR_BSY )          
  {}
}

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

/*    uint8_t tx_buff[2];*/
/*    uint8_t rx_buff[2];*/
    
/*    tx_buff[0] = 0x8C;*/
/*    tx_buff[1] = 0x00;*/

/*    SPI1WriteRead(tx_buff, rx_buff, 2);*/
/*    SPI1Write(tx_buff, 2);*/
/*    SPI1Read(rx_buff, 2);*/

/*    printf("WHO AM I 0x%x\r\n", rx_buff[0]);*/

    gyro_config();
    printf("X: 0x%x\r\n", gyro_read_x());
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
}


