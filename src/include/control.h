#ifndef _CONTROL__H__
#define _CONTROL__H__
#include <stdint.h>

#define MAX_THROTTLE 350
#define MIN_THROTTLE 100
#define START_THROTTLE 200

typedef struct {
    float pid_output_x;
    float pid_output_y;
    float pid_output_z;

    //input
    float desired_x;
    float desired_y;
    float desired_z;
    float throttle;

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

void control_config();
void control_loop(float raw_input_x, float raw_input_y, float raw_input_z);
void control_loop_x(float raw_input_x,float throttle);
void update_y(int16_t pos_y_val, int16_t neg_y_val);
void update_x(int16_t pos_y_val, int16_t neg_y_val);

#endif /* CONTROL */
