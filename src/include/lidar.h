#ifndef LIDAR_H
#define LIDAR_H

#define TFMINI_DATA_Len             9
#define TFMINT_DATA_HEAD            0x59

typedef enum {
    frame_header1,
    frame_header2,
    dist_l,
    dist_h,
    strength_l,
    strength_h,
    reserved,
    raw_qual,
    checksum

} lidar_states_t;


typedef struct {
    volatile lidar_states_t state;
    volatile uint8_t checksum;
    volatile uint16_t dist;
    volatile uint16_t valid_dist;
} lidar_t;

void lidar_readDist(int lidar);
void lidar1_callback(char c);
void lidar2_callback(char c);

#endif /* LIDAR_H */
