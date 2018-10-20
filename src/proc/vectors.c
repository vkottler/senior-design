/*
 * Vaughn Kottler, 10/19/2018
 */

#include <stdint.h>

extern uint32_t __etext;
extern uint32_t __data_start__, __data_end__;
extern uint32_t __copy_table_start__, __copy_table_end__;
extern uint32_t __zero_table_start__, __zero_table_end__;
extern uint32_t __bss_start__, __bss_end__;
extern uint32_t __StackTop;

#ifndef __STACK_SIZE
#define	__STACK_SIZE	0x00001000
#endif
static uint8_t stack[__STACK_SIZE] __attribute__((
	aligned(8), used, section(".stack")
));

#ifndef __HEAP_SIZE
#define	__HEAP_SIZE		0x00001000
#endif
static uint8_t heap[__HEAP_SIZE] __attribute__((
    aligned(8), used, section(".heap")
));

typedef void(*f_ptr)(void);

void Default_Handler(void)
{
    while(1) {;}
}

volatile uint32_t ticks = 0;
void SysTick_Handler(void) { ticks++; }

/*
 * Reset Handler.
 */
extern void SystemInit(void);
extern int _mainCRTStartup();
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
    _mainCRTStartup();
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
void EXTI2_Handler(void) __attribute__((weak, alias("Default_Handler")));
void EXTI3_Handler(void) __attribute__((weak, alias("Default_Handler")));
void EXTI4_Handler(void) __attribute__((weak, alias("Default_Handler")));
void DMA1_Stream0_Handler(void) __attribute__((weak, alias("Default_Handler")));
void DMA1_Stream1_Handler(void) __attribute__((weak, alias("Default_Handler")));
void DMA1_Stream2_Handler(void) __attribute__((weak, alias("Default_Handler")));
void DMA1_Stream3_Handler(void) __attribute__((weak, alias("Default_Handler")));
void DMA1_Stream4_Handler(void) __attribute__((weak, alias("Default_Handler")));
void DMA1_Stream5_Handler(void) __attribute__((weak, alias("Default_Handler")));
void DMA1_Stream6_Handler(void) __attribute__((weak, alias("Default_Handler")));
void ADC_Handler(void) __attribute__((weak, alias("Default_Handler")));
void CAN1_TX_Handler(void) __attribute__((weak, alias("Default_Handler")));
void CAN1_RX0_Handler(void) __attribute__((weak, alias("Default_Handler")));
void CAN1_RX1_Handler(void) __attribute__((weak, alias("Default_Handler")));
void CAN1_SCE_Handler(void) __attribute__((weak, alias("Default_Handler")));
void EXTI9_5_Handler(void) __attribute__((weak, alias("Default_Handler")));
void TIM1_BRK_TIM9_Handler(void) __attribute__((weak, alias("Default_Handler")));
void TIM1_UP_TIM10_Handler(void) __attribute__((weak, alias("Default_Handler")));
void TIM1_TRG_COM_TIM11_Handler(void) __attribute__((weak, alias("Default_Handler")));
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
void OTG_FS_WKUP_Handler(void) __attribute__((weak, alias("Default_Handler")));
void TIM8_BRK_TIM12_Handler(void) __attribute__((weak, alias("Default_Handler")));
void TIM8_UP_TIM13_Handler(void) __attribute__((weak, alias("Default_Handler")));
void TIM8_TRG_COM_TIM14_Handler(void) __attribute__((weak, alias("Default_Handler")));
void TIM8_CC_Handler(void) __attribute__((weak, alias("Default_Handler")));
void DMA1_Stream7_Handler(void) __attribute__((weak, alias("Default_Handler")));
void FSMC_Handler(void) __attribute__((weak, alias("Default_Handler")));
void SDIO_Handler(void) __attribute__((weak, alias("Default_Handler")));
void TIM5_Handler(void) __attribute__((weak, alias("Default_Handler")));
void SPI3_Handler(void) __attribute__((weak, alias("Default_Handler")));
void UART4_Handler(void) __attribute__((weak, alias("Default_Handler")));
void UART5_Handler(void) __attribute__((weak, alias("Default_Handler")));
void TIM6_DAC_Handler(void) __attribute__((weak, alias("Default_Handler")));
void TIM7_Handler(void) __attribute__((weak, alias("Default_Handler")));
void DMA2_Stream0_Handler(void) __attribute__((weak, alias("Default_Handler")));
void DMA2_Stream1_Handler(void) __attribute__((weak, alias("Default_Handler")));
void DMA2_Stream2_Handler(void) __attribute__((weak, alias("Default_Handler")));
void DMA2_Stream3_Handler(void) __attribute__((weak, alias("Default_Handler")));
void DMA2_Stream4_Handler(void) __attribute__((weak, alias("Default_Handler")));
void ETH_Handler(void) __attribute__((weak, alias("Default_Handler")));
void ETH_WKUP_Handler(void) __attribute__((weak, alias("Default_Handler")));
void CAN2_TX_Handler(void) __attribute__((weak, alias("Default_Handler")));
void CAN2_RX0_Handler(void) __attribute__((weak, alias("Default_Handler")));
void CAN2_RX1_Handler(void) __attribute__((weak, alias("Default_Handler")));
void CAN2_SCE_Handler(void) __attribute__((weak, alias("Default_Handler")));
void OTG_FS_Handler(void) __attribute__((weak, alias("Default_Handler")));
void DMA2_Stream5_Handler(void) __attribute__((weak, alias("Default_Handler")));
void DMA2_Stream6_Handler(void) __attribute__((weak, alias("Default_Handler")));
void DMA2_Stream7_Handler(void) __attribute__((weak, alias("Default_Handler")));
void USART6_Handler(void) __attribute__((weak, alias("Default_Handler")));
void I2C3_EV_Handler(void) __attribute__((weak, alias("Default_Handler")));
void I2C3_ER_Handler(void) __attribute__((weak, alias("Default_Handler")));
void OTG_HS_EP1_OUT_Handler(void) __attribute__((weak, alias("Default_Handler")));
void OTG_HS_EP1_IN_Handler(void) __attribute__((weak, alias("Default_Handler")));
void OTG_HS_WKUP_Handler(void) __attribute__((weak, alias("Default_Handler")));
void OTG_HS_Handler(void) __attribute__((weak, alias("Default_Handler")));
void DCMI_Handler(void) __attribute__((weak, alias("Default_Handler")));
void RNG_Handler(void) __attribute__((weak, alias("Default_Handler")));
void FPU_Handler(void) __attribute__((weak, alias("Default_Handler")));
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
    EXTI2_Handler,              /* EXTI Line2 Interrupt */
    EXTI3_Handler,              /* EXTI Line3 Interrupt */
    EXTI4_Handler,              /* EXTI Line4 Interrupt */
    DMA1_Stream0_Handler,       /* DMA1 Stream 0 global Interrupt */
    DMA1_Stream1_Handler,       /* DMA1 Stream 1 global Interrupt */
    DMA1_Stream2_Handler,       /* DMA1 Stream 2 global Interrupt */
    DMA1_Stream3_Handler,       /* DMA1 Stream 3 global Interrupt */
    DMA1_Stream4_Handler,       /* DMA1 Stream 4 global Interrupt */
    DMA1_Stream5_Handler,       /* DMA1 Stream 5 global Interrupt */
    DMA1_Stream6_Handler,       /* DMA1 Stream 6 global Interrupt */
    ADC_Handler,                /* ADC1, ADC2 and ADC3 global Interrupts */
    CAN1_TX_Handler,            /* CAN1 TX Interrupt */
    CAN1_RX0_Handler,           /* CAN1 RX0 Interrupt */
    CAN1_RX1_Handler,           /* CAN1 RX1 Interrupt */
    CAN1_SCE_Handler,           /* CAN1 SCE Interrupt */
    EXTI9_5_Handler,            /* External Line[9:5] Interrupts */
    TIM1_BRK_TIM9_Handler,      /* TIM1 Break interrupt and TIM9 global interrupt */
    TIM1_UP_TIM10_Handler,      /* TIM1 Update Interrupt and TIM10 global interrupt */
    TIM1_TRG_COM_TIM11_Handler, /* TIM1 Trigger and Commutation Interrupt and TIM11 global interrupt */
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
    OTG_FS_WKUP_Handler,        /* USB OTG FS Wakeup through EXTI line interrupt */
    TIM8_BRK_TIM12_Handler,     /* TIM8 Break Interrupt and TIM12 global interrupt */
    TIM8_UP_TIM13_Handler,      /* TIM8 Update Interrupt and TIM13 global interrupt */
    TIM8_TRG_COM_TIM14_Handler, /* TIM8 Trigger and Commutation Interrupt and TIM14 global interrupt */
    TIM8_CC_Handler,            /* TIM8 Capture Compare global interrupt */
    DMA1_Stream7_Handler,       /* DMA1 Stream7 Interrupt */
    FSMC_Handler,               /* FSMC global Interrupt */
    SDIO_Handler,               /* SDIO global Interrupt */
    TIM5_Handler,               /* TIM5 global Interrupt */
    SPI3_Handler,               /* SPI3 global Interrupt */
    UART4_Handler,              /* UART4 global Interrupt */
    UART5_Handler,              /* UART5 global Interrupt */
    TIM6_DAC_Handler,           /* TIM6 global and DAC1&2 underrun error  interrupts */
    TIM7_Handler,               /* TIM7 global interrupt */
    DMA2_Stream0_Handler,       /* DMA2 Stream 0 global Interrupt */
    DMA2_Stream1_Handler,       /* DMA2 Stream 1 global Interrupt */
    DMA2_Stream2_Handler,       /* DMA2 Stream 2 global Interrupt */
    DMA2_Stream3_Handler,       /* DMA2 Stream 3 global Interrupt */
    DMA2_Stream4_Handler,       /* DMA2 Stream 4 global Interrupt */
    ETH_Handler,                /* Ethernet global Interrupt */
    ETH_WKUP_Handler,           /* Ethernet Wakeup through EXTI line Interrupt */
    CAN2_TX_Handler,            /* CAN2 TX Interrupt */
    CAN2_RX0_Handler,           /* CAN2 RX0 Interrupt */
    CAN2_RX1_Handler,           /* CAN2 RX1 Interrupt */
    CAN2_SCE_Handler,           /* CAN2 SCE Interrupt */
    OTG_FS_Handler,             /* USB OTG FS global Interrupt */
    DMA2_Stream5_Handler,       /* DMA2 Stream 5 global interrupt */
    DMA2_Stream6_Handler,       /* DMA2 Stream 6 global interrupt */
    DMA2_Stream7_Handler,       /* DMA2 Stream 7 global interrupt */
    USART6_Handler,             /* USART6 global interrupt */
    I2C3_EV_Handler,            /* I2C3 event interrupt */
    I2C3_ER_Handler,            /* I2C3 error interrupt */
    OTG_HS_EP1_OUT_Handler,     /* USB OTG HS End Point 1 Out global interrupt */
    OTG_HS_EP1_IN_Handler,      /* USB OTG HS End Point 1 In global interrupt */
    OTG_HS_WKUP_Handler,        /* USB OTG HS Wakeup through EXTI interrupt */
    OTG_HS_Handler,             /* USB OTG HS global interrupt */
    DCMI_Handler,               /* DCMI global interrupt */
    0,
    RNG_Handler,                /* RNG global Interrupt */
    FPU_Handler,                /* FPU global interrupt */
};
