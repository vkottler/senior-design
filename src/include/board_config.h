#ifndef BOARD_CONFIG_H
#define BOARD_CONFIG_H

#include "stm32f303xe.h"

#define vbatt_1s_ch  LL_ADC_CHANNEL_3
#define vbatt_1s_adc  ADC4
#define vbatt_1s_seq  LL_ADC_REG_RANK_1

#define vbatt_2s_ch  LL_ADC_CHANNEL_5
#define vbatt_2s_adc  ADC3
#define vbatt_2s_seq  LL_ADC_REG_RANK_1

#define vbatt_3s_ch  LL_ADC_CHANNEL_4
#define vbatt_3s_adc  ADC4
#define vbatt_3s_seq  LL_ADC_REG_RANK_1

#define curr_meas_ch  LL_ADC_CHANNEL_5
#define curr_meas_adc ADC4
#define curr_meas_seq  LL_ADC_REG_RANK_1

void i2c_config();
void spi_config();
void adc_config();
void adc_activate();
void adc_ConversionStart(ADC_TypeDef * adc, uint32_t ch, uint32_t seq);

#endif /* BOARD_CONFIG_H */
