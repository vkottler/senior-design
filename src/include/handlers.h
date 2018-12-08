#ifndef _HANDLERS__H__
#define _HANDLERS__H__

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

void blink_handler(unsigned int blink_int);
void fault_handler(unsigned int blink_int);
void service_sensors(unsigned int interval);

void send_low_rate_telemetry(unsigned int interval);
void send_high_rate_telemetry(unsigned int interval);
void control_config();
void control_loop(float raw_input_x, float raw_input_y, float raw_input_z);

#endif
