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

PC_Buffer *gyro_tx_buf[1], *gyro_rx_buf[1];
void (*fun_ptr)();

typedef struct {
	float pid_output_x;
	float pid_output_y;
	float pid_output_z;
	
	// Difference
	float curr_dif_x;
	float curr_dif_y;
	float curr_dif_z;

	// Proportional
	float p_gain;

	// Integral
	float i_gain;
	float i_output_x;
	float i_output_y;
	float i_output_z;

	// Derivative
	float d_gain;
	float prev_dif_x;
	float prev_dif_y;
	float prev_dif_z;

} control_t;

control_t control;

void control_config()
{
	control.pid_output_x = 0;
	control.pid_output_y = 0;
	control.pid_output_z = 0;
	
	// Difference
	control.curr_dif_x = 0;
	control.curr_dif_y = 0;
	control.curr_dif_z = 0;

	// Proportional
	control.p_gain = 1;

	// Integral
	control.i_gain = 1;
	control.i_output_x = 0;
	control.i_output_y = 0;
	control.i_output_z = 0;

	// Derivative
	control.d_gain = 1;
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
/*  (CW) -X ____|____ +x (CW)         |Y+                                    */
/*         		|                     |                               		 */
/*         		|                  Z+ o___X+                                 */
/*         		|                                                            */
/*         		-Y (CCW)                                                      */
/*****************************************************************************/

void control_loop
(
	float raw_input_x, float raw_input_y, float raw_input_z, 
	float desired_x, float desired_y, float desired_z,
	float throttle
) 
{
	float esc_pos_x_val;
	float esc_neg_x_val;
	float esc_pos_y_val;
	float esc_neg_y_val;

	// Calculate difference Roll, Pitch and Yaw
	control.curr_dif_x = raw_input_x - desired_x;
	control.curr_dif_y = raw_input_y - desired_y;
	control.curr_dif_z = raw_input_z - desired_z;

	// PID tunning
	calc_pid();

	// Calculate pulse
	esc_pos_x_val = throttle + control.pid_output_x 
					       	 - control.pid_output_z;

	esc_neg_x_val = throttle - control.pid_output_x 
					       	 - control.pid_output_z;

	esc_pos_y_val = throttle + control.pid_output_y
					       	 + control.pid_output_z;

	esc_neg_y_val = throttle - control.pid_output_y
					       	 + control.pid_output_z;

	// Write Motors
	esc_set_pulse(ESC_TABLE[0], esc_pos_x_val);
	esc_set_pulse(ESC_TABLE[1], esc_neg_x_val);
	esc_set_pulse(ESC_TABLE[2], esc_pos_y_val);
	esc_set_pulse(ESC_TABLE[3], esc_neg_y_val);

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
  blink_handler(50);
}

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
