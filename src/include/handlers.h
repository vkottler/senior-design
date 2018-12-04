#ifndef _HANDLERS__H__
#define _HANDLERS__H__

void blink_handler(unsigned int blink_int);
void fault_handler(unsigned int blink_int);
void service_sensors(unsigned int interval);

void send_low_rate_telemetry(unsigned int interval);
void send_high_rate_telemetry(unsigned int interval);

#endif
