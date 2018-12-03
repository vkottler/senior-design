#include <stdlib.h>
#include "usart.h"
#include "lidar.h"

static lidar_t lidar1 = { .state = frame_header1, .checksum = 0, .dist = 0, .valid_dist = 0 };
static lidar_t lidar2 = { .state = frame_header1, .checksum = 0, .dist = 0, .valid_dist = 0 };

void lidar_readDist(int lidar_num)
{   
    lidar_t * lidar;
    if(lidar_num == 1) lidar = &lidar1;
    else if(lidar_num == 2) lidar = &lidar2;
    else return;
    lidar->valid_dist;
/*    printf("Lidar%d: %d \r\n", lidar_num, lidar->valid_dist);*/
}

void lidar_state_machine(lidar_t *lidar, char c)
{
    switch(lidar->state)
    {
        case frame_header1:
            lidar->checksum += c;
            if (c == TFMINT_DATA_HEAD)
                lidar->state = frame_header2;
            break;
        case frame_header2:
            lidar->checksum += c;
            if (c == TFMINT_DATA_HEAD)
                lidar->state = dist_l;
            break;
        case dist_l:
            lidar->checksum += c;
            lidar->dist = c;
            lidar->state = dist_h;
            break;
        case dist_h:
            lidar->checksum += c;
            lidar->dist |= c << 8;
            lidar->state = strength_l;
            break;
        case strength_l:
            lidar->checksum += c;
            lidar->state = strength_h;
            break;
        case strength_h:
            lidar->checksum += c;
            lidar->state = reserved;
            break;
        case reserved:
            lidar->checksum += c;
            lidar->state = raw_qual;
            break;
        case raw_qual:
            lidar->checksum += c;
            lidar->state = checksum;
            break;
        case checksum:
            if(lidar->checksum == c)
            {
                lidar->valid_dist = lidar->dist;
            }
            lidar->checksum = 0;
            lidar->state = frame_header1;
            break;
        default:
            lidar->state = frame_header1;
            lidar->checksum = 0;
            break;
    }
}

void lidar1_callback(char c)
{
    lidar_state_machine(&lidar1, c);
}

void lidar2_callback(char c)
{
    lidar_state_machine(&lidar2, c);
}

