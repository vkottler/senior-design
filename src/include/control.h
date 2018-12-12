#pragma once

#include <stdint.h>
#include <stdbool.h>

#define MAX_THROTTLE   750
#define MIN_THROTTLE   50
#define START_THROTTLE 200

#define P_GAIN_Y       (00.00f)//(05.00f)
#define I_GAIN_Y       (00.00f)//(00.00f)
#define D_GAIN_Y       (00.00f)//(10.00f)

#define P_GAIN_X       (00.00f)//(05.00f)
#define I_GAIN_X       (00.00f)//(00.00f)
#define D_GAIN_X       (00.00f)//(10.00f)

#define P_GAIN_Z       (00.00f)
#define I_GAIN_Z       (00.00f)
#define D_GAIN_Z       (00.00f)

#define POS_X_PORTION  (0.4789f)
#define NEG_X_PORTION  (0.5211f)
#define POS_Y_PORTION  (0.4971f)
#define NEG_Y_PORTION  (0.5029f)

typedef struct {
    float pid_output_x, pid_output_y, pid_output_z;

    //input
    float desired_x, desired_y, desired_z;
    float throttle;

    // Difference
    float curr_dif_x, curr_dif_y, curr_dif_z;

    // Proportional
    float px_gain, py_gain, pz_gain;

    // Integral
    float ix_gain, iy_gain, iz_gain;
    float i_output_x, i_output_y, i_output_z;

    // Derivative
    float dx_gain, dy_gain, dz_gain;
    float prev_dif_x, prev_dif_y, prev_dif_z;

} control_t;

void control_config(void);

void control_loop_x(float raw_input_x, float throttle);
void control_loop_y(float raw_input_y, float throttle);

void update_y(int16_t pos_y_val, int16_t neg_y_val);
void update_x(int16_t pos_y_val, int16_t neg_y_val);

float control_math(float raw, float desired,
                   float p_gain, float i_gain, float d_gain,
                   float *i_accum, float *prev_d);

void abort(void);
void end_abort(void);

extern volatile bool abort_control;
extern control_t control;

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
