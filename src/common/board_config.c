#include <stdio.h>
#include "stm32f303xe.h"
#include "board_config.h"
#include "board.h"
#include "spi.h"
#include "adc.h"
#include "i2c.h"

void adc_config()
{
    RCC->AHBENR |= RCC_AHBENR_ADC34EN;

    LL_ADC_CommonInitTypeDef handle_common;
    handle_common.CommonClock = LL_ADC_CLOCK_SYNC_PCLK_DIV2;

    LL_ADC_InitTypeDef handle;
    handle.Resolution = LL_ADC_RESOLUTION_12B;
    handle.DataAlignment = LL_ADC_DATA_ALIGN_RIGHT;
    handle.LowPowerMode = LL_ADC_LP_MODE_NONE;

    LL_ADC_REG_InitTypeDef handle_regular;
    handle_regular.TriggerSource = LL_ADC_REG_TRIG_SOFTWARE;
    handle_regular.SequencerLength = LL_ADC_REG_SEQ_SCAN_DISABLE;
    handle_regular.ContinuousMode = LL_ADC_REG_CONV_SINGLE;
    handle_regular.DMATransfer =  LL_ADC_REG_DMA_TRANSFER_NONE;
    handle_regular.Overrun = LL_ADC_REG_OVR_DATA_OVERWRITTEN;

    //ADC 3
    LL_ADC_CommonInit(ADC34_COMMON, &handle_common);
    LL_ADC_Init(ADC3, &handle);
    LL_ADC_REG_Init(ADC3, &handle_regular);

    LL_ADC_REG_SetSequencerRanks(ADC3, LL_ADC_REG_RANK_1, LL_ADC_CHANNEL_5);
    LL_ADC_SetChannelSamplingTime(ADC3, LL_ADC_CHANNEL_5, LL_ADC_SAMPLINGTIME_19CYCLES_5);
    //ADC 4
    LL_ADC_Init(ADC4, &handle);
    LL_ADC_REG_Init(ADC4, &handle_regular);

    LL_ADC_REG_SetSequencerRanks(ADC4, LL_ADC_REG_RANK_1, LL_ADC_CHANNEL_5);
    LL_ADC_SetChannelSamplingTime(ADC4, LL_ADC_CHANNEL_5, LL_ADC_SAMPLINGTIME_19CYCLES_5);
}

void adc_activate()
{
    if (LL_ADC_IsEnabled(ADC3) == 0)
    {
        LL_ADC_EnableInternalRegulator(ADC3);
        LL_ADC_StartCalibration(ADC3, LL_ADC_SINGLE_ENDED);

        while (LL_ADC_IsCalibrationOnGoing(ADC3) != 0)
        {
        }

        delay(100);
        LL_ADC_Enable(ADC3);

        while (LL_ADC_IsActiveFlag_ADRDY(ADC3) == 0)
        {
        }
    }

    if (LL_ADC_IsEnabled(ADC4) == 0)
    {
        LL_ADC_EnableInternalRegulator(ADC4);
        LL_ADC_StartCalibration(ADC4, LL_ADC_SINGLE_ENDED);

        while (LL_ADC_IsCalibrationOnGoing(ADC4) != 0)
        {
        }

        delay(100);
        LL_ADC_Enable(ADC4);

        while (LL_ADC_IsActiveFlag_ADRDY(ADC4) == 0)
        {
        }
    }
}


void adc_ConversionStart(ADC_TypeDef * adc, uint32_t ch, uint32_t seq)
{
    LL_ADC_REG_SetSequencerRanks(adc, seq, ch);
    LL_ADC_SetChannelSamplingTime(adc, ch,
            LL_ADC_SAMPLINGTIME_19CYCLES_5);
    LL_ADC_SetOffset(adc, LL_ADC_OFFSET_1, ch, 0x000);

    if ((LL_ADC_IsEnabled(adc) == 1)               &&
            (LL_ADC_IsDisableOngoing(adc) == 0)        &&
            (LL_ADC_REG_IsConversionOngoing(adc) == 0)   )
    {
        LL_ADC_REG_StartConversion(adc);
    }
    else
    {
    }
/*    while (LL_ADC_IsActiveFlag_EOC(adc) == 0)*/
/*    {*/
/*    }*/

/*    LL_ADC_ClearFlag_EOC(adc);*/
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


