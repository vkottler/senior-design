#include "esc.h"

const ESC_TypeDef ESC_TABLE[] = {
    {   .TIM = ESC_1_TIM,
        .channel = ESC_1_CH
    },
    {
        .TIM = ESC_2_TIM,
        .channel = ESC_2_CH
    },
    {
        .TIM = ESC_3_TIM,
        .channel = ESC_3_CH
    },
    {
        .TIM = ESC_4_TIM,
        .channel = ESC_4_CH
    }
};

void esc_config()
{
    TIM_PWM_Init(ESC_1_TIM, ESC_1_CH, 4000, 100);
    TIM_PWM_Init(ESC_2_TIM, ESC_2_CH, 4000, 100);
    TIM_PWM_Init(ESC_3_TIM, ESC_3_CH, 4000, 100);
    TIM_PWM_Init(ESC_4_TIM, ESC_4_CH, 4000, 100);
}

void esc_set_pulse(ESC_TypeDef ESC, uint32_t pulse)
{
    TIM_PWM_set_pulse(ESC.TIM, ESC.channel, ESC_PERIOD, pulse);
}
