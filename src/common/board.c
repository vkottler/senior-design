#include "stm32f303xe.h"
#include "post.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "gpio.h"
#include "usart.h"
#include "rcc.h"

#include "gpio_alias.h"
#include "cli.h"
#include "board.h"
#include "board_config.h"

#include "accel.h"
#include "gyro.h"
#include "esc.h"

void delay(uint32_t ms)
{
    int prev = ticks;
    while(prev + ms  > ticks) {;}
}

void set_clks(void)
{
    rcc_set_AHB_div(HCLK_DIV_1);
    rcc_set_APB1_div(PCLK_DIV_2);
    rcc_set_APB2_div(PCLK_DIV_1);

    /* turn HSE on */
    rcc_set_sys_clk_src_state(SYS_CLK_HSE, true, 10);

    /* set PLL parameters */
    rcc_set_pll(RCC_PLL_HSE, RCC_PLL_PREDIV_4, RCC_PLL_MUL_16, 10);

    /* turn PLL on */
    rcc_set_sys_clk_src_state(SYS_CLK_PLL, true, 10);

    /* set SYSCLK to PLL */
    rcc_set_sys_clk_src(SYS_CLK_PLL, 10);

    /* turn HSI off */
    rcc_set_sys_clk_src_state(SYS_CLK_HSI, false, 10);

    SystemCoreClockUpdate();
}

void io_init(void)
{
    gpioAliasInit();

    set_clks();

    // RADIO clear mode pins normal mode
    gpio_resetPin(GPIOB, 10);
    gpio_resetPin(GPIOB, 11);
}

int periph_init(void) {
    int ret = 0;

    uint32_t init_regs[3] = {USART_CR1_RXNEIE, 0, 0};

    /* radio usart */
    ret += usart_config(USART1, APBX, init_regs, 115200, true);
    /* lidar usart */
    ret += usart_config(USART2, APBX, init_regs, 115200, true);
    /* lidar usart */
    ret += usart_config(USART3, APBX, init_regs, 115200, true);

    post();
    printPrompt();

    adc_config();
    adc_activate();
    /*
    control_config();
    esc_config();
    */

    spi_config();

    return ret;
}

/* spi chip-select managers */
void command_reset_spi1_cs(void) { gpio_resetPin(GPIOA, 4); }
void command_set_spi1_cs(void) { gpio_setPin(GPIOA, 4); }
