#ifndef _TIMER__H__
#define _TIMER__H__

#include <stdint.h>
#include "stm32f303xe.h"

int TIM_PWM_Init(TIM_TypeDef *tim, uint8_t channel, uint32_t period, uint32_t duty);
int TIM_PWM_set_pulse(TIM_TypeDef *tim, uint8_t channel, uint32_t period, uint32_t duty);

#endif /* _TIMER__H__ */
