#include "handlers.h"
#include "gpio.h"
#include "accel.h"
#include "gyro.h"
#include "battery.h"
#include "lidar.h"
#include "esc.h"
#include "board.h"
#include "board_config.h"
#include "i2c.h"
#include "spi.h"
#include "gpio.h"
#include "adc.h"
#include "usart.h"
#include "pcbuffer.h"
#include <unistd.h>
#include <string.h>

PC_Buffer *gyro_tx_buf[1], *gyro_rx_buf[1];
void (*fun_ptr)();

control_t control;
#define MAX_THROTTLE 500

void control_config()
{
	control.pid_output_x = 0;
	control.pid_output_y = 0;
	control.pid_output_z = 0;

    //input
	control.desired_x = 0;
    control.desired_y = 0;
    control.desired_z = 0;
	control.throttle = 0;

	// Difference
	control.curr_dif_x = 0;
	control.curr_dif_y = 0;
	control.curr_dif_z = 0;

	// Proportional
	control.p_gain = 0.1;

	// Integral
	control.i_gain = 0;
	control.i_output_x = 0;
	control.i_output_y = 0;
	control.i_output_z = 0;

	// Derivative
	control.d_gain = 0.25;
	control.prev_dif_x = 0;
	control.prev_dif_y = 0;
	control.prev_dif_z = 0;
}

void calc_pid()
{
	float p_x, i_x, d_x;
	float p_y, i_y, d_y;
	float p_z, i_z, d_z;

    p_x = (control.curr_dif_x) * control.p_gain;
    i_x = control.i_output_x + ((control.curr_dif_x) * control.i_gain);
	control.i_output_x = i_x;
	d_x = (control.curr_dif_x - control.prev_dif_x) * control.d_gain;

    p_y = (control.curr_dif_y) * control.p_gain;
    i_y = control.i_output_y + ((control.curr_dif_y) * control.i_gain);
	control.i_output_y = i_y;
	d_y = (control.curr_dif_y - control.prev_dif_y) * control.d_gain;

    p_z = (control.curr_dif_z) * control.p_gain;
    i_z = control.i_output_z + ((control.curr_dif_z) * control.i_gain);
	control.i_output_z = i_z;
	d_z = (control.curr_dif_z - control.prev_dif_z) * control.d_gain;

    control.pid_output_x = p_x + i_x + d_x;
    control.pid_output_y = p_y + i_y + d_y;
    control.pid_output_z = p_z + i_z + d_z;
}

/*****************************************************************************/
/*         		Y + (CCW)                                                     */
/*         		|                                                            */
/*         		|                                                            */
/*  (CW) -X ____|____ +x (CW)         |Y                                     */
/*         		|                     |                               		 */
/*         		|                   Z o___X                                  */
/*         		|                                                            */
/*         		-Y (CCW)                                                      */
/*****************************************************************************/

void control_loop
(
	float raw_input_x, float raw_input_y, float raw_input_z)
{
	int16_t esc_pos_x_val;
	int16_t esc_neg_x_val;
	int16_t esc_pos_y_val;
	int16_t esc_neg_y_val;
    float pos_x_val;
    float neg_x_val;
    float pos_y_val;
    float neg_y_val;

	// Calculate difference Roll, Pitch and Yaw
	control.curr_dif_x = raw_input_x - control.desired_x;
	control.curr_dif_y = raw_input_y - control.desired_y;
	control.curr_dif_z = raw_input_z - control.desired_z;

	// PID tunning
	calc_pid();

    // Calculate pulse
	pos_x_val = (int16_t)(control.throttle + control.pid_output_y
					       	 + control.pid_output_z);
	neg_x_val = (int16_t)(control.throttle - control.pid_output_y
					       	 + control.pid_output_z);
	pos_y_val = (int16_t)(control.throttle - control.pid_output_x
					       	 - control.pid_output_z);
	neg_y_val = (int16_t)(control.throttle + control.pid_output_x
					       	 - control.pid_output_z);
    // Saturate
    if (pos_x_val < 0) pos_x_val = 0;
    if (neg_x_val < 0) neg_x_val = 0;
    if (pos_y_val < 0) pos_y_val = 0;
    if (neg_y_val < 0) neg_y_val = 0;

	// Cast
	esc_pos_x_val = (int16_t)pos_x_val;
	esc_neg_x_val = (int16_t)neg_x_val;
	esc_pos_y_val = (int16_t)pos_y_val;
	esc_neg_y_val = (int16_t)neg_y_val;

    // Saturate
    if (esc_pos_x_val > MAX_THROTTLE) esc_pos_x_val = MAX_THROTTLE;
    if (esc_neg_x_val > MAX_THROTTLE) esc_neg_x_val = MAX_THROTTLE;
    if (esc_pos_y_val > MAX_THROTTLE) esc_pos_y_val = MAX_THROTTLE;
    if (esc_neg_y_val > MAX_THROTTLE) esc_neg_y_val = MAX_THROTTLE;

    *((uint16_t *) manifest.channels[5].data) = esc_pos_x_val;
    *((uint16_t *) manifest.channels[6].data) = esc_neg_x_val;
    *((uint16_t *) manifest.channels[7].data) = esc_pos_y_val;
    *((uint16_t *) manifest.channels[8].data) = esc_neg_y_val;
	// Write Motors
/*	esc_set_pulse(ESC_TABLE[X_POS_INDEX], esc_pos_x_val);*/
/*	esc_set_pulse(ESC_TABLE[X_NEG_INDEX], esc_neg_x_val);*/
/*	esc_set_pulse(ESC_TABLE[Y_POS_INDEX], esc_pos_y_val);*/
/*	esc_set_pulse(ESC_TABLE[Y_NEG_INDEX], esc_neg_y_val);*/

}

void fault_handler(unsigned int blink_int) {

    static unsigned int curr = 0, prev = 0;

    curr = ticks / blink_int;
    if (curr != prev)
    {
        if (curr % 2) gpio_resetPin(GPIOC, 3);
        else gpio_setPin(GPIOC, 3);
    }
    prev = curr;
}

void blink_handler(unsigned int blink_int) {

    static unsigned int curr = 0, prev = 0;

    curr = ticks / blink_int;
    if (curr != prev)
    {
        if (curr % 2) gpio_resetPin(GPIOA, 1);
        else gpio_setPin(GPIOA, 1);
    }
    prev = curr;
}

void send_high_rate_telemetry(unsigned int interval)
{
    static uint32_t last_tick = 0;
    if (radio_transmit_state && !(ticks % interval) && ticks != last_tick)
    {
        last_tick = ticks;
        write_frame(TELEM_FRAME_DATA, (const char *) packets[0], telemetry_packet_size(packets[0]));
        write_frame(TELEM_FRAME_DATA, (const char *) packets[1], telemetry_packet_size(packets[1]));
    }
}

void send_low_rate_telemetry(unsigned int interval)
{
    static uint32_t last_tick = 0;
    if (radio_transmit_state && !(ticks % interval) && ticks != last_tick)
    {
        last_tick = ticks;
        write_frame(TELEM_FRAME_DATA, (const char *) packets[2], telemetry_packet_size(packets[2]));
        write_frame(TELEM_FRAME_DATA, (const char *) packets[3], telemetry_packet_size(packets[3]));
    }
}

void service_sensors(unsigned int interval) {
    static uint32_t last_tick = 0;

    if (!(ticks % interval) && ticks != last_tick)
    {
        last_tick = ticks;
        getGyroXYZ();
        gyro_read_xyz();
        batt_startConver();
        batt_getBattery();
        *((uint16_t *) manifest.channels[3].data) = lidar_readDist(1);
        *((uint16_t *) manifest.channels[4].data) = lidar_readDist(2);

        float raw_input_x, raw_input_y, raw_input_z;

        memcpy(&raw_input_x, manifest.channels[0].data, sizeof(float));
        memcpy(&raw_input_y, manifest.channels[1].data, sizeof(float));
        memcpy(&raw_input_z, manifest.channels[2].data, sizeof(float));

        control_loop(raw_input_x, raw_input_y, raw_input_z);
    }
}

void  SPI1_Tx_Callback(void)
{
    if (!pc_buffer_empty(gyro_tx_buf[0]))
        pc_buffer_remove(gyro_tx_buf[0], (char *) &SPI1->DR);
    else
        LL_SPI_DisableIT_TXE(SPI1);
}

void SPI1_TransferError_Callback(void)
{
  LL_SPI_DisableIT_RXNE(SPI1);
  LL_SPI_DisableIT_TXE(SPI1);
  while (1)
  {
      blink_handler(250);
  }
}

/*
void SPI1_Handler(void)
{
    if(LL_SPI_IsActiveFlag_RXNE(SPI1))
    {
        (*fun_ptr)();
    }
    else if(LL_SPI_IsActiveFlag_TXE(SPI1))
    {
        SPI1_Tx_Callback();
    }
    else if(LL_SPI_IsActiveFlag_OVR(SPI1))
    {
        SPI1_TransferError_Callback();
    }
}
*/
