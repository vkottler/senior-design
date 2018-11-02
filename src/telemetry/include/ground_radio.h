#ifndef GROUND_RADIO_H
#define GROUND_RADIO_H

typedef enum{
 NORMAL,
 WAKE_UP,
 POWER_SAVING,
 SLEEP
}radio_mode_type;

typedef enum{
 SLOW = 0x18, // 9600
 FAST = 0x38 // 115200
}radio_baud_type;

#define mode_0  19
#define mode_1  13 

int radio_config();
int radio_print_parm();
int radio_print_ver();
int radio_set_mode(radio_mode_type mode);

#endif /* GROUND_RADIO_H */
