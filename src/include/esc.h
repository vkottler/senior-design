#ifndef ESC_H
#define ESC_H

#include "stm32f303xe.h"
#include "timer.h"

#define ESC_PERIOD 4000

#define X_POS_INDEX  0
#define Y_POS_INDEX  1
#define X_NEG_INDEX  2
#define Y_NEG_INDEX  3

#define ESC_1_TIM TIM3
#define ESC_2_TIM TIM3
#define ESC_3_TIM TIM3
#define ESC_4_TIM TIM3

#define ESC_1_CH 1 
#define ESC_2_CH 2 
#define ESC_3_CH 3 
#define ESC_4_CH 4

typedef struct _ESC{
	TIM_TypeDef* TIM;
	uint8_t channel;
} ESC_TypeDef;

extern const ESC_TypeDef ESC_TABLE[];

void esc_config();
void esc_set_pulse(ESC_TypeDef ESC, uint32_t pulse);

#endif /* ESC_H*/
