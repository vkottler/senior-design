#include "nucleo.h"
#include "gpio.h"
#include "usart.h"
#include "rcc.h"
#include "stateful_spi.h"

#include "stm32f303xe.h"

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

void ll_spi_init(SPI_TypeDef *spi)
{
    LL_SPI_InitTypeDef spi_init;

    switch ((uint32_t) spi)
    {
        case SPI1_BASE:
            RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;
            NVIC_SetPriority(SPI1_IRQn, 3);
            NVIC_EnableIRQ(SPI1_IRQn);
            break;
        case SPI2_BASE:
            RCC->APB1ENR |= RCC_APB1ENR_SPI2EN;
            NVIC_SetPriority(SPI2_IRQn, 3);
            NVIC_EnableIRQ(SPI2_IRQn);
            break;
        default: return;
    }

    LL_SPI_StructInit(&spi_init);
    spi_init.TransferDirection = LL_SPI_FULL_DUPLEX;
    spi_init.Mode = LL_SPI_MODE_MASTER;
    spi_init.DataWidth = LL_SPI_DATAWIDTH_8BIT;
    spi_init.ClockPolarity = LL_SPI_POLARITY_LOW;
    spi_init.ClockPhase = LL_SPI_PHASE_1EDGE;
    spi_init.NSS = LL_SPI_NSS_SOFT;
    spi_init.BaudRate = LL_SPI_BAUDRATEPRESCALER_DIV64;
    spi_init.BitOrder = LL_SPI_MSB_FIRST;
    spi_init.CRCCalculation = LL_SPI_CRCCALCULATION_DISABLE;
    spi_init.CRCPoly = 7;
    LL_SPI_Init(spi, &spi_init);

    LL_SPI_SetRxFIFOThreshold(spi, LL_SPI_RX_FIFO_TH_QUARTER);
}

void spi2_init(void)
{
    gpio_setMode(GPIOB, 12, OUTPUT); // CS
    gpio_setMode(GPIOB, 13, ALT);    // SCK
    gpio_setMode(GPIOB, 14, ALT);    // MISO
    gpio_setMode(GPIOB, 15, ALT);    // MOSI
    gpio_setAlternateFunc(GPIOB, 13, 5);
    gpio_setAlternateFunc(GPIOB, 14, 5);
    gpio_setAlternateFunc(GPIOB, 15, 5);
    gpio_setPin(GPIOB, 12);
    ll_spi_init(SPI2);
    spi_init_state(&spi2_state, SPI2);
}

void nucleo_periph_init(void)
{
    uint32_t init_regs[3] = {USART_CR1_RXNEIE, 0, 0};

    if (usart_config(USART2, APBX, init_regs, 115200, true))
        nucleo_fault_handler();

    printf("0x%lx, 0x%lx\r\n", SPI2->CR1, SPI2->CR2);
}

void nucleo_io_init(void)
{
    gpio_setClock(GPIOA, true);
    gpio_setClock(GPIOB, true);

    /* LD2 */
    gpio_setMode(LED_PORT, LED_PIN, OUTPUT);

    /* USB UART */
    gpio_setMode(GPIOA, 2, ALT);
    gpio_setMode(GPIOA, 3, ALT);
    gpio_setAlternateFunc(GPIOA, 2, 7);
    gpio_setAlternateFunc(GPIOA, 3, 7);

    spi2_init();
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
