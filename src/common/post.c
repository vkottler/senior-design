#include "post.h"
#include "rcc.h"
#include "stm32f303xe.h"

#include <stdio.h>

void freq_println(uint32_t frequency)
{
    if (frequency / 1000000) printf("%lu MHz\r\n", frequency / 1000000);
    else if (frequency / 1000) printf("%lu kHz\r\n", frequency / 1000);
    else printf("%lu Hz\r\n", frequency);
}

void post(void)
{
    printf("\r\n========================================\r\n");
    printf("=      Fault Tolerant Quadcopter       =\r\n");
    printf("========================================\r\n");
    printf("SYSCLK (%s):      ", rcc_get_sys_clk_src_str());
    freq_println(SystemCoreClock);
    if (rcc_get_sys_clk_src() == SYS_CLK_PLL)
    {
        printf("PLLSRC=%s, PREDIV=%lu, MUL=%lu\r\n",
               rcc_get_pll_src_str(),
               rcc_pll_get_prediv() + 1,
               rcc_pll_get_mul() + 2);
    }
    printf("HCLK (SYSCLK/%03lu): ", rcc_get_AHB_div());
    freq_println(rcc_get_AHB());
    printf("APB1 (HCLK/%02lu):    ", rcc_get_APB1_div());
    freq_println(rcc_get_APB1());
    printf("APB2 (HCLK/%02lu):    ", rcc_get_APB2_div());
    freq_println(rcc_get_APB2());
    printf("========================================\r\n");
}
