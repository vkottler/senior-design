#pragma once

#include <stdint.h>
#include <stdbool.h>

typedef enum sys_clk_src {
    SYS_CLK_HSI = 0,
    SYS_CLK_HSE = 1,
    SYS_CLK_PLL = 2,
} sys_clk_src_t;

typedef enum pclk_div {
    PCLK_DIV_1  = 0x0,
    PCLK_DIV_2  = 0x4,
    PCLK_DIV_4  = 0x5,
    PCLK_DIV_8  = 0x6,
    PCLK_DIV_16 = 0x7
} pclk_div_t;

typedef enum hclk_div {
    HCLK_DIV_1   = 0x0,
    HCLK_DIV_2   = 0x8,
    HCLK_DIV_4   = 0x9,
    HCLK_DIV_8   = 0xa,
    HCLK_DIV_16  = 0xb,
    HCLK_DIV_64  = 0xc,
    HCLK_DIV_128 = 0xd,
    HCLK_DIV_256 = 0xe,
    HCLK_DIV_512 = 0xf,
} hclk_div_t;

typedef enum pll_src {
    RCC_PLL_HSI_2 = 0,
    RCC_PLL_HSI   = 1,
    RCC_PLL_HSE   = 2,
} pll_src_t;

typedef enum pll_mul {
    RCC_PLL_MUL_2  = 0x0,
    RCC_PLL_MUL_3  = 0x1,
    RCC_PLL_MUL_4  = 0x2,
    RCC_PLL_MUL_5  = 0x3,
    RCC_PLL_MUL_6  = 0x4,
    RCC_PLL_MUL_7  = 0x5,
    RCC_PLL_MUL_8  = 0x6,
    RCC_PLL_MUL_9  = 0x7,
    RCC_PLL_MUL_10 = 0x8,
    RCC_PLL_MUL_11 = 0x9,
    RCC_PLL_MUL_12 = 0xa,
    RCC_PLL_MUL_13 = 0xb,
    RCC_PLL_MUL_14 = 0xc,
    RCC_PLL_MUL_15 = 0xd,
    RCC_PLL_MUL_16 = 0xe,
} pll_mul_t;

typedef enum pll_prediv {
    RCC_PLL_PREDIV_1  = 0x0,
    RCC_PLL_PREDIV_2  = 0x1,
    RCC_PLL_PREDIV_3  = 0x2,
    RCC_PLL_PREDIV_4  = 0x3,
    RCC_PLL_PREDIV_5  = 0x4,
    RCC_PLL_PREDIV_6  = 0x5,
    RCC_PLL_PREDIV_7  = 0x6,
    RCC_PLL_PREDIV_8  = 0x7,
    RCC_PLL_PREDIV_9  = 0x8,
    RCC_PLL_PREDIV_10 = 0x9,
    RCC_PLL_PREDIV_11 = 0xa,
    RCC_PLL_PREDIV_12 = 0xb,
    RCC_PLL_PREDIV_13 = 0xc,
    RCC_PLL_PREDIV_14 = 0xd,
    RCC_PLL_PREDIV_15 = 0xe,
    RCC_PLL_PREDIV_16 = 0xf
} pll_prediv_t;

uint32_t rcc_get_AHB_div(void);
uint32_t rcc_get_AHB(void);
uint32_t rcc_get_APB1_div(void);
uint32_t rcc_get_APB1(void);
uint32_t rcc_get_APB2_div(void);
uint32_t rcc_get_APB2(void);

void rcc_set_AHB_div(hclk_div_t value);
void rcc_set_APB1_div(pclk_div_t value);
void rcc_set_APB2_div(pclk_div_t value);

sys_clk_src_t rcc_get_sys_clk_src(void);
const char *rcc_get_sys_clk_src_str();

uint32_t rcc_sys_clk_src_state(sys_clk_src_t src);
bool rcc_set_sys_clk_src_state(sys_clk_src_t src, bool state, uint32_t timeout);
bool rcc_set_sys_clk_src(sys_clk_src_t src, uint32_t timeout);

pll_src_t rcc_get_pll_src(void);
const char *rcc_get_pll_src_str(void);
uint32_t rcc_pll_get_prediv(void);
uint32_t rcc_pll_get_mul(void);

bool rcc_set_pll(pll_src_t src, pll_prediv_t predif, pll_mul_t mul,
                 uint32_t timeout);
