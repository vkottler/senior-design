#pragma once

#define ANALOG_CONVER 0.000805664f

typedef struct {
    float R1, R2;
} VOLT_DIVIDER_T;

typedef enum {
    vbatt_1s = 0,
    vbatt_2s = 1,
    vbatt_3s = 2,
    curr_meas = 3
} current_conversion_t;

void batt_startConver(void);
void batt_getBattery(void);
