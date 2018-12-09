/*
 * Vaughn Kottler, 10/19/2018
 */

#include <stdint.h>
#include "handlers.h"

extern uint32_t __etext;
extern uint32_t __data_start__, __data_end__;
extern uint32_t __copy_table_start__, __copy_table_end__;
extern uint32_t __zero_table_start__, __zero_table_end__;
extern uint32_t __bss_start__, __bss_end__;
extern uint32_t __StackTop;

#ifndef __STACK_SIZE
#define	__STACK_SIZE	0x00000C00
#endif
static uint8_t stack[__STACK_SIZE] __attribute__((
	aligned(8), used, section(".stack")
));

#ifndef __HEAP_SIZE
#define	__HEAP_SIZE		0x00000400
#endif
static uint8_t heap[__HEAP_SIZE] __attribute__((
    aligned(8), used, section(".heap")
));

typedef void(*f_ptr)(void);

void Default_Handler(void)
{
/*    fault_loop();*/
}

volatile uint32_t ticks = 0;
void SysTick_Handler(void) { ticks++; }

/*
 * Reset Handler.
 */
extern void SystemInit(void);
extern int main(void);
void Reset_Handler(void)
{
    uint32_t *pSrc, *pDest;

    pSrc  = &__etext;
    pDest = &__data_start__;

    /* copy data section from ROM image to RAM */
    for ( ; pDest < &__data_end__ ; )
        *pDest++ = *pSrc++;

    pDest = &__bss_start__;

    /* zero BSS */
    for ( ; pDest < &__bss_end__ ; )
        *pDest++ = 0UL;

    /* Call system initialization routine */
    SystemInit();

    /* Jump to the main initialization routine. */
    main();
}

/*
 * Processor-interrupt vector table.
 */
void NonMaskableInt_Handler(void) __attribute__((weak, alias("Default_Handler")));
void HardFault_Handler(void) __attribute__((weak, alias("Default_Handler")));
void MemoryManagement_Handler(void) __attribute__((weak, alias("Default_Handler")));
void BusFault_Handler(void) __attribute__((weak, alias("Default_Handler")));
void UsageFault_Handler(void) __attribute__((weak, alias("Default_Handler")));
void SVCall_Handler(void) __attribute__((weak, alias("Default_Handler")));
void DebugMonitor_Handler(void) __attribute__((weak, alias("Default_Handler")));
void PendSV_Handler(void) __attribute__((weak, alias("Default_Handler")));
const f_ptr __core_vectors[] __attribute__((section(".vectors"))) =
{
    (f_ptr)((uint32_t)&__StackTop), /* Initial Stack Pointer */
    Reset_Handler,                  /* Reset Handler         */
    NonMaskableInt_Handler,         /* 2 Non Maskable Interrupt */
    HardFault_Handler,              /* Hard Fault Handler    */
    MemoryManagement_Handler,       /* 4 Cortex-M4 Memory Management Interrupt */
    BusFault_Handler,               /* 5 Cortex-M4 Bus Fault Interrupt */
    UsageFault_Handler,             /* 6 Cortex-M4 Usage Fault Interrupt */
    0, 0, 0, 0,
    SVCall_Handler,                 /* 11 Cortex-M4 SV Call Interrupt */
    DebugMonitor_Handler,           /* 12 Cortex-M4 Debug Monitor Interrupt */
    0,
    PendSV_Handler,                 /* 14 Cortex-M4 Pend SV Interrupt */
    SysTick_Handler,                /* 15 Cortex-M4 System Tick Interrupt */
};

/*
 * Peripheral-interrupt vector table.
 */
void WWDG_Handler(void) __attribute__((weak, alias("Default_Handler")));
void PVD_Handler(void) __attribute__((weak, alias("Default_Handler")));
void TAMP_STAMP_Handler(void) __attribute__((weak, alias("Default_Handler")));
void RTC_WKUP_Handler(void) __attribute__((weak, alias("Default_Handler")));
void FLASH_Handler(void) __attribute__((weak, alias("Default_Handler")));
void RCC_Handler(void) __attribute__((weak, alias("Default_Handler")));
void EXTI0_Handler(void) __attribute__((weak, alias("Default_Handler")));
void EXTI1_Handler(void) __attribute__((weak, alias("Default_Handler")));
void EXTI2_TSC_Handler(void) __attribute__((weak, alias("Default_Handler")));
void EXTI3_Handler(void) __attribute__((weak, alias("Default_Handler")));
void EXTI4_Handler(void) __attribute__((weak, alias("Default_Handler")));
void DMA1_Channel1_Handler(void) __attribute__((weak, alias("Default_Handler")));
void DMA1_Channel2_Handler(void) __attribute__((weak, alias("Default_Handler")));
void DMA1_Channel3_Handler(void) __attribute__((weak, alias("Default_Handler")));
void DMA1_Channel4_Handler(void) __attribute__((weak, alias("Default_Handler")));
void DMA1_Channel5_Handler(void) __attribute__((weak, alias("Default_Handler")));
void DMA1_Channel6_Handler(void) __attribute__((weak, alias("Default_Handler")));
void DMA1_Channel7_Handler(void) __attribute__((weak, alias("Default_Handler")));
void ADC1_2_Handler(void) __attribute__((weak, alias("Default_Handler")));
void USB_HP_CAN_TX_Handler(void) __attribute__((weak, alias("Default_Handler")));
void USB_LP_CAN_RX0_Handler(void) __attribute__((weak, alias("Default_Handler")));
void CAN_RX1_Handler(void) __attribute__((weak, alias("Default_Handler")));
void CAN_SCE_Handler(void) __attribute__((weak, alias("Default_Handler")));
void EXTI9_5_Handler(void) __attribute__((weak, alias("Default_Handler")));
void TIM1_BRK_TIM15_Handler(void) __attribute__((weak, alias("Default_Handler")));
void TIM1_UP_TIM16_Handler(void) __attribute__((weak, alias("Default_Handler")));
void TIM1_TRG_COM_TIM17_Handler(void) __attribute__((weak, alias("Default_Handler")));
void TIM1_CC_Handler(void) __attribute__((weak, alias("Default_Handler")));
void TIM2_Handler(void) __attribute__((weak, alias("Default_Handler")));
void TIM3_Handler(void) __attribute__((weak, alias("Default_Handler")));
void TIM4_Handler(void) __attribute__((weak, alias("Default_Handler")));
void I2C1_EV_Handler(void) __attribute__((weak, alias("Default_Handler")));
void I2C1_ER_Handler(void) __attribute__((weak, alias("Default_Handler")));
void I2C2_EV_Handler(void) __attribute__((weak, alias("Default_Handler")));
void I2C2_ER_Handler(void) __attribute__((weak, alias("Default_Handler")));
void SPI1_Handler(void) __attribute__((weak, alias("Default_Handler")));
void SPI2_Handler(void) __attribute__((weak, alias("Default_Handler")));
void USART1_Handler(void) __attribute__((weak, alias("Default_Handler")));
void USART2_Handler(void) __attribute__((weak, alias("Default_Handler")));
void USART3_Handler(void) __attribute__((weak, alias("Default_Handler")));
void EXTI15_10_Handler(void) __attribute__((weak, alias("Default_Handler")));
void RTC_Alarm_Handler(void) __attribute__((weak, alias("Default_Handler")));
void USBWakeUp_Handler(void) __attribute__((weak, alias("Default_Handler")));
void TIM8_BRK_Handler(void) __attribute__((weak, alias("Default_Handler")));
void TIM8_UP_Handler(void) __attribute__((weak, alias("Default_Handler")));
void TIM8_TRG_COM_Handler(void) __attribute__((weak, alias("Default_Handler")));
void TIM8_CC_Handler(void) __attribute__((weak, alias("Default_Handler")));
void ADC3_Handler(void) __attribute__((weak, alias("Default_Handler")));
void FMC_Handler(void) __attribute__((weak, alias("Default_Handler")));
void SPI3_Handler(void) __attribute__((weak, alias("Default_Handler")));
void UART4_Handler(void) __attribute__((weak, alias("Default_Handler")));
void UART5_Handler(void) __attribute__((weak, alias("Default_Handler")));
void TIM6_DAC_Handler(void) __attribute__((weak, alias("Default_Handler")));
void TIM7_Handler(void) __attribute__((weak, alias("Default_Handler")));
void DMA2_Channel1_Handler(void) __attribute__((weak, alias("Default_Handler")));
void DMA2_Channel2_Handler(void) __attribute__((weak, alias("Default_Handler")));
void DMA2_Channel3_Handler(void) __attribute__((weak, alias("Default_Handler")));
void DMA2_Channel4_Handler(void) __attribute__((weak, alias("Default_Handler")));
void DMA2_Channel5_Handler(void) __attribute__((weak, alias("Default_Handler")));
void ADC4_Handler(void) __attribute__((weak, alias("Default_Handler")));
void COMP1_2_3_Handler(void) __attribute__((weak, alias("Default_Handler")));
void COMP4_5_6_Handler(void) __attribute__((weak, alias("Default_Handler")));
void COMP7_Handler(void) __attribute__((weak, alias("Default_Handler")));
void I2C3_EV_Handler(void) __attribute__((weak, alias("Default_Handler")));
void I2C3_ER_Handler(void) __attribute__((weak, alias("Default_Handler")));
void USB_HP_Handler(void) __attribute__((weak, alias("Default_Handler")));
void USB_LP_Handler(void) __attribute__((weak, alias("Default_Handler")));
void USBWakeUp_RMP_Handler(void) __attribute__((weak, alias("Default_Handler")));
void TIM20_BRK_Handler(void) __attribute__((weak, alias("Default_Handler")));
void TIM20_UP_Handler(void) __attribute__((weak, alias("Default_Handler")));
void TIM20_TRG_COM_Handler(void) __attribute__((weak, alias("Default_Handler")));
void TIM20_CC_Handler(void) __attribute__((weak, alias("Default_Handler")));
void FPU_Handler(void) __attribute__((weak, alias("Default_Handler")));
void SPI4_Handler(void) __attribute__((weak, alias("Default_Handler")));
const f_ptr __dev_vectors[] __attribute__((section(".device_vectors"))) =
{
    WWDG_Handler,               /* Window WatchDog Interrupt */
    PVD_Handler,                /* PVD through EXTI Line detection Interrupt */
    TAMP_STAMP_Handler,         /* Tamper and TimeStamp interrupts through the EXTI line */
    RTC_WKUP_Handler,           /* RTC Wakeup interrupt through the EXTI line */
    FLASH_Handler,              /* FLASH global Interrupt */
    RCC_Handler,                /* RCC global Interrupt */
    EXTI0_Handler,              /* EXTI Line0 Interrupt */
    EXTI1_Handler,              /* EXTI Line1 Interrupt */
    EXTI2_TSC_Handler,              /* EXTI Line2 Interrupt */
    EXTI3_Handler,              /* EXTI Line3 Interrupt */
    EXTI4_Handler,              /* EXTI Line4 Interrupt */
    DMA1_Channel1_Handler,       /* DMA1 Stream 0 global Interrupt */
    DMA1_Channel2_Handler,       /* DMA1 Stream 1 global Interrupt */
    DMA1_Channel3_Handler,       /* DMA1 Stream 2 global Interrupt */
    DMA1_Channel4_Handler,       /* DMA1 Stream 3 global Interrupt */
    DMA1_Channel5_Handler,       /* DMA1 Stream 4 global Interrupt */
    DMA1_Channel6_Handler,       /* DMA1 Stream 5 global Interrupt */
    DMA1_Channel7_Handler,       /* DMA1 Stream 6 global Interrupt */
    ADC1_2_Handler,                /* ADC1, ADC2 and ADC3 global Interrupts */
    USB_HP_CAN_TX_Handler,
    USB_LP_CAN_RX0_Handler,
    CAN_RX1_Handler,
    CAN_SCE_Handler,
    EXTI9_5_Handler,            /* External Line[9:5] Interrupts */
    TIM1_BRK_TIM15_Handler,
    TIM1_UP_TIM16_Handler,
    TIM1_TRG_COM_TIM17_Handler,
    TIM1_CC_Handler,            /* TIM1 Capture Compare Interrupt */
    TIM2_Handler,               /* TIM2 global Interrupt */
    TIM3_Handler,               /* TIM3 global Interrupt */
    TIM4_Handler,               /* TIM4 global Interrupt */
    I2C1_EV_Handler,            /* I2C1 Event Interrupt */
    I2C1_ER_Handler,            /* I2C1 Error Interrupt */
    I2C2_EV_Handler,            /* I2C2 Event Interrupt */
    I2C2_ER_Handler,            /* I2C2 Error Interrupt */
    SPI1_Handler,               /* SPI1 global Interrupt */
    SPI2_Handler,               /* SPI2 global Interrupt */
    USART1_Handler,             /* USART1 global Interrupt */
    USART2_Handler,             /* USART2 global Interrupt */
    USART3_Handler,             /* USART3 global Interrupt */
    EXTI15_10_Handler,          /* External Line[15:10] Interrupts */
    RTC_Alarm_Handler,          /* RTC Alarm (A and B) through EXTI Line Interrupt */
    USBWakeUp_Handler,
    TIM8_BRK_Handler,     /* TIM8 Break Interrupt and TIM12 global interrupt */
    TIM8_UP_Handler,      /* TIM8 Update Interrupt and TIM13 global interrupt */
    TIM8_TRG_COM_Handler, /* TIM8 Trigger and Commutation Interrupt and TIM14 global interrupt */
    TIM8_CC_Handler,            /* TIM8 Capture Compare global interrupt */
    ADC3_Handler,
    FMC_Handler,               /* FSMC global Interrupt */
    SPI3_Handler,               /* SPI3 global Interrupt */
    UART4_Handler,              /* UART4 global Interrupt */
    UART5_Handler,              /* UART5 global Interrupt */
    TIM6_DAC_Handler,           /* TIM6 global and DAC1&2 underrun error  interrupts */
    TIM7_Handler,               /* TIM7 global interrupt */
    DMA2_Channel1_Handler,       /* DMA2 Stream 0 global Interrupt */
    DMA2_Channel2_Handler,       /* DMA2 Stream 1 global Interrupt */
    DMA2_Channel3_Handler,       /* DMA2 Stream 2 global Interrupt */
    DMA2_Channel4_Handler,       /* DMA2 Stream 3 global Interrupt */
    DMA2_Channel5_Handler,       /* DMA2 Stream 4 global Interrupt */
    ADC4_Handler,
    COMP1_2_3_Handler,          /*!< COMP1, COMP2 and COMP3 global Interrupt via EXTI Line21, 22 and 29*/
    COMP4_5_6_Handler,          /*!< COMP4, COMP5 and COMP6 global Interrupt via EXTI Line30, 31 and 32*/
    COMP7_Handler,              /*!< COMP7 global Interrupt via EXTI Line33                            */
    I2C3_EV_Handler,            /*!< I2C3 event interrupt                                              */
    I2C3_ER_Handler,            /*!< I2C3 Error Interrupt                                              */
    USB_HP_Handler,             /*!< USB High Priority global Interrupt                                */
    USB_LP_Handler,             /*!< USB Low Priority global Interrupt                                 */
    USBWakeUp_RMP_Handler,      /*!< USB Wakeup Interrupt remap                                        */
    TIM20_BRK_Handler,          /*!< TIM20 Break Interrupt                                              */
    TIM20_UP_Handler,           /*!< TIM20 Update Interrupt                                             */
    TIM20_TRG_COM_Handler,      /*!< TIM20 Trigger and Commutation Interrupt                            */
    TIM20_CC_Handler,           /*!< TIM20 Capture Compare Interrupt                                    */
    FPU_Handler,                /* FPU global interrupt */
    0,
    0,
    SPI4_Handler,               /*!< SPI4 global Interrupt                                             */   
};
