#include <stdbool.h>
#include <stdio.h>
#include "board.h"
#include "board_config.h"
#include "battery.h"
#include "adc.h"
#include <string.h> 

const VOLT_DIVIDER_T V_DIVIDERS[3] = {{.R1 = 100.0f, .R2 = 100.0f},
                                      {.R1 = 300.0f, .R2 = 100.0f},
                                      {.R1 = 750.0f, .R2 = 150.0f}};


float battery_val[4] = {0, 0, 0, 0};
static current_conversion_t curr_conv = vbatt_1s;

float voltageDivider(float r1, float r2)
{ return ((r1+r2)/r2); }

void batt_startConver(void)
{
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
        default: return;
    }
}

void batt_convertUnits(ADC_TypeDef * adc, float vdivider,
                       uint8_t battery_val_index, float subtractor)
{
    int i;
    uint16_t analogRead = LL_ADC_REG_ReadConversionData12(adc);
    float volts = analogRead * ANALOG_CONVER * vdivider;

    battery_val[battery_val_index] = volts - subtractor;

    memcpy(manifest.channels[BATTERY_IND + battery_val_index].data, 
           (void *) &battery_val[battery_val_index], sizeof(float));

    /* if we just updated the third cell, sum all of the cell values */
    if (battery_val_index == vbatt_3s)
    {
        volts = 0.0f;
        for (i = 0; i < 3; i++)
            volts += battery_val[i];
        memcpy(manifest.channels[BATTERY_IND + 4].data, 
                (void *) &volts, sizeof(float));
    }
}

void batt_getBattery()
{ 
    uint16_t battery_val_index;
    float vdivider = 1.0f;
    float subtractor = 0.0f;
    ADC_TypeDef * adc;
    current_conversion_t next_conv;

    switch(curr_conv)
    {
        case vbatt_1s:
            adc = vbatt_1s_adc;
            battery_val_index = 0;
            vdivider = voltageDivider(
                    V_DIVIDERS[battery_val_index].R1,
                    V_DIVIDERS[battery_val_index].R2);
            next_conv = vbatt_2s;
            break;
        case vbatt_2s:
            adc = vbatt_2s_adc;
            battery_val_index = 1;
            vdivider = voltageDivider(
                    V_DIVIDERS[battery_val_index].R1,
                    V_DIVIDERS[battery_val_index].R2);
            subtractor = battery_val[vbatt_1s];
            next_conv = vbatt_3s;
            break;
        case vbatt_3s:
            adc = vbatt_3s_adc;
            battery_val_index = 2;
            vdivider = voltageDivider(
                    V_DIVIDERS[battery_val_index].R1,
                    V_DIVIDERS[battery_val_index].R2);
            subtractor = battery_val[vbatt_1s] + battery_val[vbatt_2s];
            next_conv = curr_meas;
            break;
        case curr_meas:
            adc = curr_meas_adc;
            battery_val_index = 3;
            vdivider = 10.0f; 
            next_conv = vbatt_1s;
            break;
        default: return;
    }

    /* handle sample if it's finished and start a new one */
    if (LL_ADC_IsActiveFlag_EOC(adc))
    {
        LL_ADC_ClearFlag_EOC(adc);
        batt_convertUnits(adc, vdivider, battery_val_index, subtractor);
        curr_conv = next_conv;
        batt_startConver();
    }
}
