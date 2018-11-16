#ifndef ESC_H
#define ESC_H

#include "stm32f303xe.h"
#include "timer.h"

#define ESC_PERIOD 20000

#define ESC_1_TIM TIM3
#define ESC_2_TIM TIM3
#define ESC_3_TIM TIM2
#define ESC_4_TIM TIM2

#define ESC_1_CH 1 
#define ESC_2_CH 2 
#define ESC_3_CH 3 
#define ESC_4_CH 2 

typedef struct _ESC{
	TIM_TypeDef* TIM;
	uint8_t channel;
} ESC_TypeDef;


void esc_config();
void esc_set_pulse(ESC_TypeDef ESC, uint32_t pulse);

#endif /* ESC_H*/
