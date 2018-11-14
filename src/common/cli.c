#include "stm32f303xe.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "pcbuffer.h"
#include "cli.h"
#include "accel.h"
#include "timer.h"

char buffer[BUFSIZ];
/* how to print a backspace to the console */
char backspace[3] = {0x08, ' ', 0x08};

void process_input(char *buffer) {
    printf("%s\r\n", buffer);
    if(strcmp(buffer, "accel") == 0)
    {
        printf("ACCEL\r\n");
        printf("x: %d\r\n", accel_read_x());
        printf("y: %d\r\n", accel_read_y());
        printf("z: %d\r\n", accel_read_z());
    }
    else if(buffer[0] == 'p' && buffer[1] == 'w' && buffer[2] == 'm')
    {   uint32_t pulse = atoi(&buffer[4]);
        TIM_PWM_set_pulse(TIM3, 1, 20000, pulse);
        TIM_PWM_set_pulse(TIM3, 2, 20000, pulse);
        TIM_PWM_set_pulse(TIM2, 3, 20000, pulse);
        TIM_PWM_set_pulse(TIM2, 2, 20000, pulse);
    }
}

inline void printPrompt(void) {
    printf("=> ");
    fflush(stdout);
}

void check_input(void) {
    if (pc_buffer_messageAvailable(rx_buf[0])) {
        pc_buffer_getMessage(rx_buf[0], buffer, 128);
        if (buffer[0] != '\0') {
            process_input(buffer);
        }
        printPrompt();
    }
}

