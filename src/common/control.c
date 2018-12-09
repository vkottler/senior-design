#include "control.h"
#include "esc.h"
#include "channels.h"

control_t control;
extern bool abort_control;
void control_config()
{
	control.pid_output_x = 0;
	control.pid_output_y = 0;
	control.pid_output_z = 0;

    //input
	control.desired_x = 0;
    control.desired_y = 0;
    control.desired_z = 0;
	control.throttle = START_THROTTLE;

	// Difference
	control.curr_dif_x = 0;
	control.curr_dif_y = 0;
	control.curr_dif_z = 0;

	// Proportional
	control.p_gain = 10.0f;

	// Integral
	control.i_gain = 0.25f;
	control.i_output_x = 0;
	control.i_output_y = 0;
	control.i_output_z = 0;

	// Derivative
	control.d_gain = 25.0f;
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
void update_y(int16_t pos_y_val, int16_t neg_y_val) 
{ 
    *((uint16_t *) manifest.channels[12].data) = pos_y_val;
    *((uint16_t *) manifest.channels[13].data) = neg_y_val;
	esc_set_pulse(ESC_TABLE[Y_POS_INDEX], pos_y_val);
	esc_set_pulse(ESC_TABLE[Y_NEG_INDEX], neg_y_val);
}

void update_x(int16_t pos_x_val, int16_t neg_x_val) 
{ 
    *((uint16_t *) manifest.channels[10].data) = pos_x_val;
    *((uint16_t *) manifest.channels[11].data) = neg_x_val;
	esc_set_pulse(ESC_TABLE[X_POS_INDEX], pos_x_val);
	esc_set_pulse(ESC_TABLE[X_NEG_INDEX], neg_x_val);
}

void control_loop_y(float raw_input_y, float throttle)
{
	float p_y, i_y, d_y;
    int16_t pos_x_val;
    int16_t neg_x_val;
    int16_t new_pos_x_val;
    int16_t new_neg_x_val;
    int16_t remainder;

	control.curr_dif_y = raw_input_y - control.desired_y;
    
    // Proportional Difference times gain
    p_y = (control.curr_dif_y) * control.p_gain;

    // Integral acumulate plus difference time gain
    i_y = control.i_output_y + ((control.curr_dif_y) * control.i_gain);
	control.i_output_y = i_y;

    // Derivatie change in difference time gain
	d_y = (control.curr_dif_y - control.prev_dif_y) * control.d_gain;
    control.prev_dif_y = control.curr_dif_y;

    // Compute overall effect
    control.pid_output_y = p_y + i_y + d_y;

	pos_x_val = (int16_t)(throttle - control.pid_output_x/2.0f);
	neg_x_val = (int16_t)(throttle + control.pid_output_x/2.0f);

    new_pos_x_val = pos_x_val;
    new_neg_x_val = neg_x_val;

    // Saturate
    if (pos_x_val < MIN_THROTTLE)
    {
        remainder = MIN_THROTTLE - pos_x_val;
        new_neg_x_val -= remainder;
        pos_x_val = MIN_THROTTLE;
    }
    else if (pos_x_val > MAX_THROTTLE)
    { 
        remainder = pos_x_val - MAX_THROTTLE;
        new_neg_x_val += remainder;
        pos_x_val = MAX_THROTTLE;
    }

    if (neg_x_val < MIN_THROTTLE)
    {
        remainder = MIN_THROTTLE - neg_x_val;
        new_pos_x_val -= remainder;
        neg_x_val = MIN_THROTTLE;
    }
    else if (neg_x_val > MAX_THROTTLE) 
    {
        remainder = neg_x_val - MAX_THROTTLE;
        new_pos_x_val += remainder;
        neg_x_val = MAX_THROTTLE;
    }

    if(new_neg_x_val > MAX_THROTTLE || new_pos_x_val > MAX_THROTTLE || 
       new_neg_x_val < MIN_THROTTLE || new_pos_x_val < MIN_THROTTLE ) 
    {
        printf("new pos x: %d new neg x: %d\r\n", new_pos_x_val, new_neg_x_val);
        printf("pos x: %d neg x: %d\r\n", pos_x_val, neg_x_val);
        abort_control = true;
        new_pos_x_val = 0;
        new_neg_x_val = 0;
    }

    update_x(new_pos_x_val, new_neg_x_val);

}
void control_loop_x(float raw_input_x, float throttle)
{
	float p_x, i_x, d_x;
    int16_t pos_y_val;
    int16_t neg_y_val;
    int16_t new_pos_y_val;
    int16_t new_neg_y_val;
    int16_t remainder;

	control.curr_dif_x = raw_input_x - control.desired_x;
    
    // Proportional Difference times gain
    p_x = (control.curr_dif_x) * control.p_gain;

    // Integral acumulate plus difference time gain
    i_x = control.i_output_x + ((control.curr_dif_x) * control.i_gain);
	control.i_output_x = i_x;

    // Derivatie change in difference time gain
	d_x = (control.curr_dif_x - control.prev_dif_x) * control.d_gain;
    control.prev_dif_x = control.curr_dif_x;

    // Compute overall effect
    control.pid_output_x = p_x + i_x + d_x;

	pos_y_val = (int16_t)(throttle - control.pid_output_x/2.0f);
	neg_y_val = (int16_t)(throttle + control.pid_output_x/2.0f);

    new_pos_y_val = pos_y_val;
    new_neg_y_val = neg_y_val;

    // Saturate
    if (pos_y_val < MIN_THROTTLE)
    {
        remainder = MIN_THROTTLE - pos_y_val;
        new_neg_y_val -= remainder;
        pos_y_val = MIN_THROTTLE;
    }
    else if (pos_y_val > MAX_THROTTLE)
    { 
        remainder = pos_y_val - MAX_THROTTLE;
        new_neg_y_val += remainder;
        pos_y_val = MAX_THROTTLE;
    }

    if (neg_y_val < MIN_THROTTLE)
    {
        remainder = MIN_THROTTLE - neg_y_val;
        new_pos_y_val -= remainder;
        neg_y_val = MIN_THROTTLE;
    }
    else if (neg_y_val > MAX_THROTTLE) 
    {
        remainder = neg_y_val - MAX_THROTTLE;
        new_pos_y_val += remainder;
        neg_y_val = MAX_THROTTLE;
    }

    if(new_neg_y_val > MAX_THROTTLE || new_pos_y_val > MAX_THROTTLE || 
       new_neg_y_val < MIN_THROTTLE || new_pos_y_val < MIN_THROTTLE ) 
    {
        printf("new pos y: %d new neg y: %d\r\n", new_pos_y_val, new_neg_y_val);
        printf("pos y: %d neg y: %d\r\n", pos_y_val, neg_y_val);
        abort_control = true;
        new_pos_y_val = 0;
        new_neg_y_val = 0;
    }

    update_y(new_pos_y_val, new_neg_y_val);

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

