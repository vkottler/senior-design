#ifndef __SYSTEM_STM32F4XX_H
#define __SYSTEM_STM32F4XX_H

#include <stdint.h>

/* This variable is updated in three ways:
 * 1) by calling CMSIS function SystemCoreClockUpdate()
 * 2) by calling HAL API function HAL_RCC_GetSysClockFreq()
 * 3) each time HAL_RCC_ClockConfig() is called to configure the system clock frequency 
 *    Note: If you use this function to configure the system clock; then there
 *          is no need to call the 2 first functions listed above, since SystemCoreClock
 *          variable is updated automatically.
 */
extern uint32_t SystemCoreClock;            /* System Clock Frequency (Core Clock) */
extern volatile uint32_t ticks;             /* SysTick Timer Value */
extern const uint8_t  AHBPrescTable[16];    /* AHB prescalers table values */
extern const uint8_t  APBPrescTable[8];     /* APB prescalers table values */
extern void SystemInit(void);
extern void SystemCoreClockUpdate(void);

#endif
