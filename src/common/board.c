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

    /*
    adc_config();
    adc_activate();
    control_config();
    esc_config();*/

    spi_config();

    return ret;
}

channel_manifest_t manifest;
channel_t channels[MAX_CHANNEL_COUNT];
telemetry_packet_t *packets[MAX_PACKET_COUNT];

void manifest_sender(char *buffer, uint32_t len)
{
    write_frame(TELEM_FRAME_MANIFEST, buffer, len);
}

void manifest_init(void)
{
    manifest.channels = channels;
    manifest.count = 0;
    manifest.capacity = MAX_CHANNEL_COUNT;

    channel_add(&manifest, "gyro_x", "deg/s", TELEM_FLOAT, sizeof(float));
    channel_add(&manifest, "gyro_y", "deg/s", TELEM_FLOAT, sizeof(float));
    channel_add(&manifest, "gyro_z", "deg/s", TELEM_FLOAT, sizeof(float));

    channel_add(&manifest, "lidar_d1", "mm", TELEM_UINT16, sizeof(uint16_t));
    channel_add(&manifest, "lidar_d2", "mm", TELEM_UINT16, sizeof(uint16_t));

    channel_add(&manifest, "esc_pos_x_val", "1000 + ms", TELEM_UINT16, sizeof(uint16_t));
    channel_add(&manifest, "esc_neg_x_val", "1000 + ms", TELEM_UINT16, sizeof(uint16_t));
    channel_add(&manifest, "esc_pos_y_val", "1000 + ms", TELEM_UINT16, sizeof(uint16_t));
    channel_add(&manifest, "esc_neg_y_val", "1000 + ms", TELEM_UINT16, sizeof(uint16_t));

    channel_add(&manifest, "batt_v_cell1", "ADC counts", TELEM_FLOAT , sizeof(float));
    channel_add(&manifest, "batt_v_cell2", "ADC counts", TELEM_FLOAT, sizeof(float));
    channel_add(&manifest, "batt_v_cell3", "ADC counts", TELEM_FLOAT, sizeof(float));
    channel_add(&manifest, "batt_current", "ADC counts", TELEM_FLOAT, sizeof(float));
    channel_add(&manifest, "batt_v_total", "ADC counts", TELEM_FLOAT, sizeof(float));

    packets[0] = telemetry_packet_create(&manifest.channels[0], 3);
    packets[1] = telemetry_packet_create(&manifest.channels[3], 2);
    packets[2] = telemetry_packet_create(&manifest.channels[5], 4);
    packets[3] = telemetry_packet_create(&manifest.channels[9], 5);

    /*
    channel_manifest_print(stdout, &manifest);
    telemetry_packet_print(stdout, packets[0]);
    telemetry_packet_print(stdout, packets[1]);
    telemetry_packet_print(stdout, packets[2]);
    telemetry_packet_print(stdout, packets[3]);
    */

    channel_manifest_send(&manifest, manifest_sender);
}

/* spi chip-select managers */
void command_reset_spi1_cs(void) { gpio_resetPin(GPIOA, 4); }
void command_set_spi1_cs(void) { gpio_setPin(GPIOA, 4); }
