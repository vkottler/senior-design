#include "nucleo.h"

#include "gpio.h"
#include "usart.h"
#include "rcc.h"

void nucleo_fault_handler(void)
{
    while (1)
    {
        nucleo_blink_handler(100);
    }
}

void nucleo_set_clks(void)
{
    rcc_set_AHB_div(HCLK_DIV_1);
    rcc_set_APB1_div(PCLK_DIV_2);
    rcc_set_APB2_div(PCLK_DIV_1);

    /* set PLL parameters */
    if (!rcc_set_pll(RCC_PLL_HSI, RCC_PLL_PREDIV_2, RCC_PLL_MUL_10, 10))
        nucleo_fault_handler();
    
    /* turn PLL on */
    if (!rcc_set_sys_clk_src_state(SYS_CLK_PLL, true, 10))
        nucleo_fault_handler();

    /* set SYSCLK to PLL */
    if (!rcc_set_sys_clk_src(SYS_CLK_PLL, 10))
        nucleo_fault_handler();

    SystemCoreClockUpdate();
}

void nucleo_periph_init(void)
{
    /* USB UART */
    gpio_setMode(GPIOA, 2, ALT);
    gpio_setMode(GPIOA, 3, ALT);
    gpio_setAlternateFunc(GPIOA, 2, 7);
    gpio_setAlternateFunc(GPIOA, 3, 7);

    uint32_t init_regs[3] = {USART_CR1_RXNEIE, 0, 0};

    if (usart_config(USART2, APBX, init_regs, 115200, true))
        nucleo_fault_handler();
}

void nucleo_io_init(void)
{
    /* LD2 */
    gpio_setClock(LED_PORT, true);
    gpio_setMode(LED_PORT, LED_PIN, OUTPUT);
}

void nucleo_blink_handler(unsigned int blink_int)
{
    static unsigned int curr = 0, prev = 0;

    curr = ticks / blink_int;

    if (curr != prev)
    {
        if (curr % 2) LED_PORT->BSRR |= LED_PIN_MASK << 16;
        else          LED_PORT->BSRR |= LED_PIN_MASK;
    }
    prev = curr;
}
