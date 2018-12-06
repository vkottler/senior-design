#include "stm32f303xe.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "gpio.h"
#include "usart.h"

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

int io_init(void) {

    int ret = 0;
    gpioAliasInit();
    // RADIO clear mode pins normal mode
    gpio_resetPin(GPIOB, 10);
    gpio_resetPin(GPIOB, 11);
    return ret;
}

int periph_init(void) {
    int ret = 0;

    uint32_t init_regs[3] = {0, 0, 0};

    /* USB UART */
    init_regs[0] = USART_CR1_RXNEIE;

    ret += usart_config(USART1, SYSCLK, init_regs, 115200, true);
    printf("RADIO USART1 INIT\r\n");
    ret += usart_config(USART2, SYSCLK, init_regs, 115200, true);
    printf("LIDAR1 USART2 INIT\r\n");
    ret += usart_config(USART3, SYSCLK, init_regs, 115200, true);
    printf("LIDAR2 USART3 INIT\r\n");

    spi_config();
    printf("SPI INIT\r\n");

    gyro_config();
    printf("gyro INIT\r\n");

    adc_config();
    adc_activate();
    printf("adc INIT\r\n");

    esc_config();
    printf("esc INIT\r\n");

    control_config();
    printf("control INIT\r\n");

    printf("System Core Clock: %ld\r\n", SystemCoreClock);
    printPrompt();

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
