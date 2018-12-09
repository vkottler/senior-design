#include "esc.h"
#include "board.h"
#include "gpio.h"

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

void esc_reset()
{
    gpio_setClock(GPIOB, true);
    gpio_setMode(GPIOB, 4, OUTPUT);
    gpio_setSpeed(GPIOB, 4, LOW_SPEED);
    gpio_writePin(GPIOB, 4, 0);

    gpio_setMode(GPIOB, 5, OUTPUT);
    gpio_setSpeed(GPIOB, 5, LOW_SPEED);
    gpio_writePin(GPIOB, 5, 0);

    gpio_setMode(GPIOB, 0, OUTPUT);
    gpio_setSpeed(GPIOB, 0, LOW_SPEED);
    gpio_writePin(GPIOB, 0, 0);

    gpio_setMode(GPIOB, 1, OUTPUT);
    gpio_setSpeed(GPIOB, 1, LOW_SPEED);
    gpio_writePin(GPIOB, 1, 0);

    delay(2000);
}

void esc_config()
{
    TIM_PWM_Init(ESC_1_TIM, ESC_1_CH, ESC_PERIOD, 1000);
    TIM_PWM_Init(ESC_2_TIM, ESC_2_CH, ESC_PERIOD, 1000);
    TIM_PWM_Init(ESC_3_TIM, ESC_3_CH, ESC_PERIOD, 1000);
    TIM_PWM_Init(ESC_4_TIM, ESC_4_CH, ESC_PERIOD, 1000);

    // Wait for ESC to latch
    delay(7000);

    esc_set_pulse(ESC_TABLE[X_POS_INDEX], 0);
    esc_set_pulse(ESC_TABLE[X_NEG_INDEX], 0);
    esc_set_pulse(ESC_TABLE[Y_POS_INDEX], 0);
    esc_set_pulse(ESC_TABLE[Y_NEG_INDEX], 0);

    delay(7000);

}

void esc_set_pulse(ESC_TypeDef ESC, uint32_t pulse)
{
    TIM_PWM_set_pulse(ESC.TIM, ESC.channel, ESC_PERIOD, pulse);
}
