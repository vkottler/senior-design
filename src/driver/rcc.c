#include "rcc.h"

#include "stm32f303xe.h"

/* SYSCLK sources */
const char *sys_clk_src_str[] = { "HSI", "HSE", "PLL"};
sys_clk_src_t rcc_get_sys_clk_src(void)
{ return (sys_clk_src_t) (RCC->CFGR & RCC_CFGR_SW_Msk) >> RCC_CFGR_SW_Pos; }
const char *rcc_get_sys_clk_src_str(void)
{ return sys_clk_src_str[rcc_get_sys_clk_src()]; }
uint32_t rcc_sys_clk_src_state(sys_clk_src_t src)
{
    uint32_t mask = 0, position = 0;
    switch (src)
    {
        case SYS_CLK_HSI:
            mask = RCC_CR_HSION_Msk;
            position = RCC_CR_HSION_Pos;
            break;
        case SYS_CLK_HSE:
            mask = RCC_CR_HSEON_Msk;
            position = RCC_CR_HSEON_Pos;
            break;
        case SYS_CLK_PLL:
            mask = RCC_CR_PLLON_Msk;
            position = RCC_CR_PLLON_Pos;
            break;
    }
    return (RCC->CR & mask) >> position;
}
bool rcc_set_sys_clk_src_state(sys_clk_src_t src, bool state, uint32_t timeout)
{
    uint32_t rdy_msk = 0, state_msk = 0, timeout_time;

    /* check if anything needs to be done */
    if ((rcc_sys_clk_src_state(src) && state) ||
        (!rcc_sys_clk_src_state(src) && !state))
        return true;

    switch (src)
    {
        case SYS_CLK_HSI:
            state_msk = RCC_CR_HSION_Msk;
            rdy_msk = RCC_CR_HSIRDY_Msk;
            break;
        case SYS_CLK_HSE:
            state_msk = RCC_CR_HSEON_Msk;
            rdy_msk = RCC_CR_HSERDY_Msk;
            break;
        case SYS_CLK_PLL:
            state_msk = RCC_CR_PLLON_Msk;
            rdy_msk = RCC_CR_PLLRDY_Msk;
            break;
    }

    timeout_time = ticks + timeout;
    if (state)
    {
        RCC->CR |= state_msk;
        while (ticks < timeout_time && !(RCC->CR & rdy_msk)) {;}
    }
    else
    {
        while (ticks < timeout_time && (RCC->CR & rdy_msk)) {;}
    }

    return ticks < timeout_time;
}
bool rcc_set_sys_clk_src(sys_clk_src_t src, uint32_t timeout)
{
    uint32_t status_expected = 0, to_set, timeout_time;

    /* don't handle enabling clocks first */
    if (!rcc_sys_clk_src_state(src))
        return false;

    to_set = RCC->CFGR & ~(RCC_CFGR_SW_Msk);
    switch (src)
    {
        case SYS_CLK_HSI:
            to_set |= RCC_CFGR_SW_HSI;
            status_expected = RCC_CFGR_SWS_HSI;
            break;
        case SYS_CLK_HSE:
            to_set |= RCC_CFGR_SW_HSE;
            status_expected = RCC_CFGR_SWS_HSE;
            break;
        case SYS_CLK_PLL:
            to_set |= RCC_CFGR_SW_PLL;
            status_expected = RCC_CFGR_SWS_PLL;
            break;
         default: return false;
    }

    /* set the source request and wait for it to take effect */
    timeout_time = ticks + timeout;
    RCC->CFGR = to_set;
    while (ticks < timeout_time)
    {
        if ((RCC->CFGR & RCC_CFGR_SWS_Msk) == status_expected)
            break;
    }
    return ticks < timeout_time;
}

/* APB1 */
uint32_t rcc_get_APB1_shift(void)
{ return APBPrescTable[(RCC->CFGR & RCC_CFGR_PPRE1) >> RCC_CFGR_PPRE1_Pos]; }
uint32_t rcc_get_APB1_div(void) { return 0x1 << rcc_get_APB1_shift(); }
uint32_t rcc_get_APB1(void) { return rcc_get_AHB() >> rcc_get_APB1_shift(); }
void rcc_set_APB1_div(pclk_div_t value)
{
	uint32_t curr;
	curr = RCC->CFGR;
	curr &= ~RCC_CFGR_PPRE1;
	curr |= value << RCC_CFGR_PPRE1_Pos;
	RCC->CFGR = curr;
}

/* APB2 */
uint32_t rcc_get_APB2_shift(void)
{ return APBPrescTable[(RCC->CFGR & RCC_CFGR_PPRE2) >> RCC_CFGR_PPRE2_Pos]; }
uint32_t rcc_get_APB2_div(void) { return 0x1 << rcc_get_APB2_shift(); }
uint32_t rcc_get_APB2(void) { return rcc_get_AHB() >> rcc_get_APB2_shift(); }
void rcc_set_APB2_div(pclk_div_t value)
{
	uint32_t curr;
	curr = RCC->CFGR;
	curr &= ~RCC_CFGR_PPRE2;
	curr |= value << RCC_CFGR_PPRE2_Pos;
	RCC->CFGR = curr;
}

/* AHB (HCLK) */
uint32_t rcc_get_AHB_shift(void)
{ return AHBPrescTable[(RCC->CFGR & RCC_CFGR_HPRE) >> RCC_CFGR_HPRE_Pos]; }
uint32_t rcc_get_AHB_div(void) { return 0x1 << rcc_get_AHB_shift(); }
uint32_t rcc_get_AHB(void) { return SystemCoreClock >> rcc_get_AHB_shift(); }
void rcc_set_AHB_div(hclk_div_t value)
{
	uint32_t curr;
	curr = RCC->CFGR;
	curr &= ~RCC_CFGR_HPRE;
	curr |= value << RCC_CFGR_HPRE_Pos;
	RCC->CFGR = curr;
}

/* PLL */
const char *pll_clk_src_str[] = { "HSI_2", "HSI", "HSE"};
pll_src_t rcc_get_pll_src(void)
{ return (RCC->CFGR & RCC_CFGR_PLLSRC_Msk) >> RCC_CFGR_PLLSRC_Pos; }
const char *rcc_get_pll_src_str(void)
{ return pll_clk_src_str[rcc_get_pll_src()]; }

uint32_t rcc_pll_get_prediv(void)
{ return (RCC->CFGR2 & RCC_CFGR2_PREDIV_Msk) >> RCC_CFGR2_PREDIV_Pos; }
uint32_t rcc_pll_get_mul(void)
{ return (RCC->CFGR & RCC_CFGR_PLLMUL_Msk) >> RCC_CFGR_PLLMUL_Pos; }

bool rcc_set_pll(pll_src_t src, pll_prediv_t prediv, pll_mul_t mul,
                 uint32_t timeout)
{
    uint32_t value_to_set = RCC->CFGR, temp;

    /* can't set PLL parameters when it's the system clock source */
    if (rcc_get_sys_clk_src() == SYS_CLK_PLL)
        return false;

    /* disable the PLL */
    if (!rcc_set_sys_clk_src_state(SYS_CLK_PLL, false, timeout))
        return false;

    /* clear current parameter values that we want to update */
    value_to_set &= ~(RCC_CFGR_PLLMUL_Msk | RCC_CFGR_PLLSRC_Msk);

    /* set prediv register */
    temp = RCC->CFGR2 & ~RCC_CFGR2_PREDIV_Msk;
    temp |= prediv << RCC_CFGR2_PREDIV_Pos;
    RCC->CFGR2 = temp;
    value_to_set |= (prediv & 0x1) << RCC_CFGR_PLLXTPRE_Pos;

    /* set the requested source */
    value_to_set |= src << RCC_CFGR_PLLSRC_Pos;

    /* set the requested multiplier */
    value_to_set |= mul << RCC_CFGR_PLLMUL_Pos;

    /* verify that the new requested state would be functional */
    // TODO

    /* update the PLL configurations */
    RCC->CFGR = value_to_set;
    return true;
}
