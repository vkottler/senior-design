#include "control.h"
#include "esc.h"
#include "channels.h"

#include <string.h>

control_t control;
volatile bool abort_control = false;

static void reset_requested(void)
{
	control.desired_x = 0;
    control.desired_y = 0;
    control.desired_z = 0;
}

void end_abort(void)
{
    control.throttle = START_THROTTLE;
    reset_requested();
    update_y(control.throttle, control.throttle);
    update_x(control.throttle, control.throttle);
    abort_control = false;
}

void abort(void)
{
    abort_control = true;
    control.throttle = MIN_THROTTLE;
    reset_requested();
    update_y(control.throttle, control.throttle);
    update_x(control.throttle, control.throttle);
}

void control_loop_y(float raw_input_y, float throttle)
{
    int16_t pos_x_val, neg_x_val;
    int16_t new_pos_x_val, new_neg_x_val;
    int16_t remainder;

    control.pid_output_y = control_math(raw_input_y, control.desired_y,
                                        control.py_gain, control.iy_gain,
                                        control.dy_gain, &control.i_output_y,
                                        &control.prev_dif_y);

    pos_x_val = (int16_t) (throttle + control.pid_output_y * POS_X_PORTION);
    neg_x_val = (int16_t) (throttle - control.pid_output_y * NEG_X_PORTION);

    new_pos_x_val = pos_x_val;
    new_neg_x_val = neg_x_val;

    // Saturate
    if (pos_x_val < MIN_THROTTLE)
    {
        remainder = MIN_THROTTLE - pos_x_val;
        new_neg_x_val -= remainder;
        new_pos_x_val = MIN_THROTTLE;
    }
    else if (pos_x_val > MAX_THROTTLE)
    { 
        remainder = pos_x_val - MAX_THROTTLE;
        new_neg_x_val += remainder;
        new_pos_x_val = MAX_THROTTLE;
    }

    if (neg_x_val < MIN_THROTTLE)
    {
        remainder = MIN_THROTTLE - neg_x_val;
        new_pos_x_val -= remainder;
        new_neg_x_val = MIN_THROTTLE;
    }
    else if (neg_x_val > MAX_THROTTLE) 
    {
        remainder = neg_x_val - MAX_THROTTLE;
        new_pos_x_val += remainder;
        new_neg_x_val = MAX_THROTTLE;
    }

    if(new_neg_x_val > MAX_THROTTLE || new_pos_x_val > MAX_THROTTLE || 
       new_neg_x_val < MIN_THROTTLE || new_pos_x_val < MIN_THROTTLE ) 
    {
        abort();
        __enable_irq();
        printf("new pos x: %d new neg x: %d\r\n", new_pos_x_val, new_neg_x_val);
        printf("pos x: %d neg x: %d\r\n", pos_x_val, neg_x_val);
        return;
    }

    update_x(new_pos_x_val, new_neg_x_val);
}

void control_loop_x(float raw_input_x, float throttle)
{
    int16_t pos_y_val, neg_y_val;
    int16_t new_pos_y_val, new_neg_y_val;
    int16_t remainder;

    control.pid_output_x = control_math(raw_input_x, control.desired_x,
                                        control.px_gain, control.ix_gain,
                                        control.dx_gain, &control.i_output_x,
                                        &control.prev_dif_x);

    pos_y_val = (int16_t) (throttle - control.pid_output_x * POS_Y_PORTION);
    neg_y_val = (int16_t) (throttle + control.pid_output_x * NEG_Y_PORTION);

    new_pos_y_val = pos_y_val;
    new_neg_y_val = neg_y_val;

    // Saturate
    if (pos_y_val < MIN_THROTTLE)
    {
        remainder = MIN_THROTTLE - pos_y_val;
        new_neg_y_val -= remainder;
        new_pos_y_val = MIN_THROTTLE;
    }
    else if (pos_y_val > MAX_THROTTLE)
    { 
        remainder = pos_y_val - MAX_THROTTLE;
        new_neg_y_val += remainder;
        new_pos_y_val = MAX_THROTTLE;
    }

    if (neg_y_val < MIN_THROTTLE)
    {
        remainder = MIN_THROTTLE - neg_y_val;
        new_pos_y_val -= remainder;
        new_neg_y_val = MIN_THROTTLE;
    }
    else if (neg_y_val > MAX_THROTTLE) 
    {
        remainder = neg_y_val - MAX_THROTTLE;
        new_pos_y_val += remainder;
        new_neg_y_val = MAX_THROTTLE;
    }

    if(new_neg_y_val > MAX_THROTTLE || new_pos_y_val > MAX_THROTTLE || 
       new_neg_y_val < MIN_THROTTLE || new_pos_y_val < MIN_THROTTLE ) 
    {
        abort();
        __enable_irq();
        printf("new pos y: %d new neg y: %d\r\n", new_pos_y_val, new_neg_y_val);
        printf("pos y: %d neg y: %d\r\n", pos_y_val, neg_y_val);
        return;
    }

    update_y(new_pos_y_val, new_neg_y_val);
}

float control_math(float raw, float desired,
                   float p_gain, float i_gain, float d_gain,
                   float *i_accum, float *prev_d)
{
    float p_result, i_result, d_result, curr_diff;

    curr_diff = raw - desired;

    /* proportional: scale the current input by gain */
    p_result = curr_diff * p_gain;

    /* integral: accumulate current input by gain */
    i_result = *i_accum + (curr_diff * i_gain);
    *i_accum = i_result;

    /* derivative: accumulate difference of current input */
    d_result = (curr_diff - *prev_d) * d_gain;
    *prev_d = curr_diff;

    return p_result + i_result + d_result;
}

void update_y(int16_t pos_y_val, int16_t neg_y_val) 
{ 
    *((uint16_t *) manifest.channels[POSY_IND].data) = pos_y_val;
    *((uint16_t *) manifest.channels[NEGY_IND].data) = neg_y_val;
    esc_set_pulse(ESC_TABLE[Y_POS_INDEX], pos_y_val);
    esc_set_pulse(ESC_TABLE[Y_NEG_INDEX], neg_y_val);
}

void update_x(int16_t pos_x_val, int16_t neg_x_val) 
{ 
    *((uint16_t *) manifest.channels[POSX_IND].data) = pos_x_val;
    *((uint16_t *) manifest.channels[NEGX_IND].data) = neg_x_val;
    esc_set_pulse(ESC_TABLE[X_POS_INDEX], pos_x_val);
    esc_set_pulse(ESC_TABLE[X_NEG_INDEX], neg_x_val);
}

void control_config(void)
{
    memset(&control, 0, sizeof(control_t));
    control.throttle = START_THROTTLE;

    control.px_gain = P_GAIN_X;
    control.ix_gain = I_GAIN_X;
    control.dx_gain = D_GAIN_X;

    control.py_gain = P_GAIN_Y;
    control.iy_gain = I_GAIN_Y;
    control.dy_gain = D_GAIN_Y;

    control.pz_gain = P_GAIN_Z;
    control.iz_gain = I_GAIN_Z;
    control.dz_gain = D_GAIN_Z;
}
