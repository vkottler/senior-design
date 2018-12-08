#include "timer.h"

int TIM_PWM_Init(TIM_TypeDef *tim, uint8_t channel, uint32_t period, uint32_t pulse)
{
	/*enable TIM1 */
	if (tim == TIM1)
	    RCC->APB2ENR |= RCC_APB2ENR_TIM1EN;
    else if (tim == TIM2)
	    RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
	else if (tim == TIM3)
	    RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;
	else if (tim == TIM4)
	    RCC->APB1ENR |= RCC_APB1ENR_TIM4EN;
	else if (tim == TIM6)
	    RCC->APB1ENR |= RCC_APB1ENR_TIM6EN;
	else if (tim == TIM7)
	    RCC->APB1ENR |= RCC_APB1ENR_TIM7EN;

	/*	edge aligned */
	tim->CR1 &= ~0x60;
	/*	count up*/
	tim->CR1 &= ~TIM_CR1_DIR;

	/*disable ARR*/
	tim->CR1 &= ~0x80;

	/*	set period */
	tim->ARR = period;
	/*tim-> ARR = 0x8;*/

	tim->PSC = 0x1F;

	tim->EGR |= 0x1;

	/*make pwm mode 1 */
	switch(channel){
		case 1:
	tim->CCMR1 |= TIM_CCMR1_OC1M_2 | TIM_CCMR1_OC1M_1;
	break;
		case 2:
	tim->CCMR1 |= TIM_CCMR1_OC2M_2 | TIM_CCMR1_OC2M_1;
	break;
		case 3:
	tim->CCMR2 |= TIM_CCMR2_OC3M_2 | TIM_CCMR2_OC3M_1;
	break;
		case 4:
	tim->CCMR2 |= TIM_CCMR2_OC4M_2 | TIM_CCMR2_OC4M_1;
	break;
		default:
	break;
	}
	
	/*clear and set active high*/
	tim->CCER &= ~(0xf<<(4*(channel-1)));

	switch (channel){
		case 1:
			tim->CCR1 = (uint32_t)(1000 + pulse * 10);
			break;
		case 2:
			tim->CCR2 = (uint32_t)(1000 + pulse * 10);
			break;
		case 3:
			tim->CCR3 = (uint32_t)(1000 + pulse * 10);
			break;
		case 4:
			tim->CCR4 = (uint32_t)(1000 + pulse * 10);
			break;
		default:
			break;
	}
	/* output on pin */
	tim->BDTR |= 0x8000;

	/*	enable timer */
	tim->CR1 |= 0x1;
	/* enable channel */
	tim->CCER |= 0x1<<(4*(channel-1));

	return 1;
}



int TIM_PWM_set_pulse(TIM_TypeDef *tim, uint8_t channel, uint32_t period, uint32_t pulse)
{
	switch (channel){
		case 1:
			tim->CCR1 = (uint32_t)(1000 + pulse);
			break;
		case 2:
			tim->CCR2 = (uint32_t)(1000 + pulse);
			break;
		case 3:
			tim->CCR3 = (uint32_t)(1000 + pulse);
			break;
		case 4:
			tim->CCR4 = (uint32_t)(1000 + pulse);
			break;
		default:
			break;
	}
    return 0;
}


