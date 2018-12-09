#ifndef _HANDLERS__H__
#define _HANDLERS__H__

void fault_handler(unsigned int blink_int);
void fault_loop(void);
void blink_handler(unsigned int blink_int);
void run_critical(void);
void run_control(void);

#endif
