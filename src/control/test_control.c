/*
 * Vaughn Kottler, 08/05/2018
 */

#include "control.h"

int main(int argc, char **argv)
{
    motion_state_t state;
    motion_configuration_t config;
    control_init(0, 0, &state, &config, 800.0f, 800.0f);
    return 0;
}
