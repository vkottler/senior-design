#include <stdbool.h>
#include <stdio.h>
#include "board.h"
#include "board_config.h"
#include "battery.h"
#include "adc.h"

#define ANALOG_CONVER 0.000805664

typedef enum{
    vbatt_1s,
    vbatt_2s,
    vbatt_3s,
    curr_meas
} current_conversion_t;

static float battery_val[4] = {0, 0, 0, 0};
static current_conversion_t curr_conv = vbatt_1s;
static bool start_adc = true;

void batt_startConver()
{
    if(!start_adc) return;
    switch(curr_conv)
    {
        case vbatt_1s:
            adc_ConversionStart(vbatt_1s_adc, vbatt_1s_ch, vbatt_1s_seq);
            break;
        case vbatt_2s:
            adc_ConversionStart(vbatt_2s_adc, vbatt_2s_ch, vbatt_2s_seq);
            break;
        case vbatt_3s:
            adc_ConversionStart(vbatt_3s_adc, vbatt_3s_ch, vbatt_3s_seq);
            break;
        case curr_meas:
            adc_ConversionStart(curr_meas_adc, curr_meas_ch, curr_meas_seq);
            break;
        default: 
            curr_conv = vbatt_1s;
    }
    start_adc = false;
}


void batt_getBattery()
{ 
    uint16_t battery_val_index;
    float *channel_data;
    ADC_TypeDef * adc;
    current_conversion_t next_conv;

    switch(curr_conv)
    {
        case vbatt_1s:
            adc = vbatt_1s_adc;
            battery_val_index = 0;
            next_conv = vbatt_2s;
            break;
        case vbatt_2s:
            adc = vbatt_2s_adc;
            battery_val_index = 1;
            next_conv = vbatt_3s;
            break;
        case vbatt_3s:
            adc = vbatt_3s_adc;
            battery_val_index = 2;
            next_conv = curr_meas;
            break;
        case curr_meas:
            adc = curr_meas_adc;
            battery_val_index = 3;
            next_conv = vbatt_1s;
            break;
        default: 
            curr_conv = vbatt_1s;
            adc = vbatt_1s_adc;
            battery_val_index = 0;
            next_conv = vbatt_1s;
    }

    if (LL_ADC_IsActiveFlag_EOC(adc))
    {
        LL_ADC_ClearFlag_EOC(adc);
        battery_val[battery_val_index] = LL_ADC_REG_ReadConversionData12(adc) * ANALOG_CONVER;

        channel_data = (float*) manifest.channels[9 + battery_val_index].data;
        *channel_data = battery_val[battery_val_index];

        curr_conv = next_conv;
        start_adc = true;
    }

}


