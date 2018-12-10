#include "stm32f303xe.h"

#include "channels.h"
#include "radio.h"

telemetry_manager_t tm;

telemetry_group_t packet_groups[TELEM_NUM_GROUPS];

void manifest_sender(char *buffer, uint32_t len)
{ write_frame(TELEM_FRAME_MANIFEST, buffer, len); }

extern volatile uint32_t ticks;
void publish_telemetry(telemetry_manager_t *tman)
{
    uint32_t elapsed, sent_dropped, expected_handled;

    /* attempt to publish any channels needing servicing */
    elapsed = ticks - tman->t_zero;
    for (uint32_t i = 0; i < tman->packet_index; i++)
    {
        sent_dropped = tman->packets_sent[i] + tman->packets_dropped[i];

        /* also add not published packets to keep counts up to date */
        sent_dropped += tman->packets_not_published[i];
        
        expected_handled = elapsed / *(tman->packet_update_rates[i]);

        /* this condition tells us it's time to send the next packet */
        if (expected_handled > sent_dropped)
        {
            /* count this packet as not published if it's requested to not
             * be sent */
            if (!tman->publish || !(*tman->packet_auto_publish[i]))
            {
                tman->packets_not_published[i]++;
                continue;
            }

            /* if this packet can't be sent, consider it dropped */
            if (radio_transmit_state && !radio_buffer_full &&
                write_frame(TELEM_FRAME_DATA, (const char *) packets[i], 
                            telemetry_packet_size(packets[i])))
                tman->packets_sent[i]++;

            /* only drop a packet if we're now a full cycle behind,
             * otherwise allow the above condition to be checked again
             * on the next iteration */
            else if (expected_handled - sent_dropped > 1)
                tman->packets_dropped[i]++;
        }
    }
}

void dump_telemetry_manager(telemetry_manager_t *tman)
{
    printf("publish: %s, npackets: %lu, nchannels: %lu\r\n",
           (tman->publish) ? "true" : "false", tman->packet_index,
           manifest.count);
    for (uint32_t i = 0; i < tman->packet_index; i++)
    {
        printf("%s (%s): rate: %lu, sent: %lu, dropped: %lu, not published: %lu, sent: %lu\r\n",
               packet_groups[i].alias, (*(tman->packet_auto_publish[i])) ? "on" : "off",
               *(tman->packet_update_rates[i]), tman->packets_sent[i],
               tman->packets_dropped[i], tman->packets_not_published[i],
               tman->packet_last_ticks[i]);
    }
}

bool initialize_next_packet(uint32_t num_channels, uint32_t *update_rate,
                            bool *auto_publish)
{
    uint32_t start_index = manifest.count - num_channels;
    channel_t *start_channel;

    if (tm.packet_index == MAX_PACKET_COUNT)
    {
        printf("Can't create additional packets!\r\n");
        return false;
    }

    start_channel = &manifest.channels[start_index];
    packets[tm.packet_index] = telemetry_packet_create(start_channel, num_channels);
    tm.packet_update_rates[tm.packet_index] = update_rate;
    tm.packets_sent[tm.packet_index] = 0;
    tm.packets_dropped[tm.packet_index] = 0;
    tm.packets_not_published[tm.packet_index] = 0;
    tm.packet_auto_publish[tm.packet_index] = auto_publish;
    tm.packet_index++;

    return true;
}

bool manifest_init_data_channels(void)
{
    /* gyro data */
    packet_groups[0].alias   = "gyro";
    packet_groups[0].rate    = GYRO_UPDATE_RATE;
    packet_groups[0].publish = GYRO_TELEM_ON;
    channel_add(&manifest, "gyro_x", "mdeg/s", TELEM_FLOAT, sizeof(float));
    channel_add(&manifest, "gyro_y", "mdeg/s", TELEM_FLOAT, sizeof(float));
    channel_add(&manifest, "gyro_z", "mdeg/s", TELEM_FLOAT, sizeof(float));
    if (!initialize_next_packet(3, &packet_groups[0].rate,
                                &packet_groups[0].publish))
        return false;

    /* lidar data */
    packet_groups[1].alias   = "lidar";
    packet_groups[1].rate    = LIDAR_UPDATE_RATE;
    packet_groups[1].publish = LIDAR_TELEM_ON;
    channel_add(&manifest, "lidar_d1", "mm", TELEM_UINT16, sizeof(uint16_t));
    channel_add(&manifest, "lidar_d2", "mm", TELEM_UINT16, sizeof(uint16_t));
    if (!initialize_next_packet(2, &packet_groups[1].rate,
                                &packet_groups[1].publish))
        return false;

    /* battery metrics */
    packet_groups[2].alias   = "battery";
    packet_groups[2].rate    = BATTERY_UPDATE_RATE;
    packet_groups[2].publish = BATTERY_TELEM_ON;
    channel_add(&manifest, "batt_v_cell1", "ADC counts", TELEM_FLOAT , sizeof(float));
    channel_add(&manifest, "batt_v_cell2", "ADC counts", TELEM_FLOAT, sizeof(float));
    channel_add(&manifest, "batt_v_cell3", "ADC counts", TELEM_FLOAT, sizeof(float));
    channel_add(&manifest, "batt_current", "ADC counts", TELEM_FLOAT, sizeof(float));
    channel_add(&manifest, "batt_v_total", "ADC counts", TELEM_FLOAT, sizeof(float));
    if (!initialize_next_packet(5, &packet_groups[2].rate,
                                &packet_groups[2].publish))
        return false;

    return true;
}

bool manifest_init_parameter_channels(void)
{
    /* motor throttles */
    packet_groups[3].alias   = "esc";
    packet_groups[3].rate    = THROTTLE_UPDATE_RATE;
    packet_groups[3].publish = THROTTLE_TELEM_ON;
    channel_add(&manifest, "esc_pos_x_val", "1000 + ms", TELEM_UINT16, sizeof(uint16_t));
    channel_add(&manifest, "esc_neg_x_val", "1000 + ms", TELEM_UINT16, sizeof(uint16_t));
    channel_add(&manifest, "esc_pos_y_val", "1000 + ms", TELEM_UINT16, sizeof(uint16_t));
    channel_add(&manifest, "esc_neg_y_val", "1000 + ms", TELEM_UINT16, sizeof(uint16_t));
    if (!initialize_next_packet(4, &packet_groups[3].rate,
                                &packet_groups[3].publish))
        return false;

    return true;
}

void init_telemetry_manager(void)
{
    manifest.channels = tm.data_channels;
    manifest.count = 0;
    manifest.capacity = MAX_CHANNEL_COUNT;
    tm.publish = true;
}

bool manifest_init(void)
{
    init_telemetry_manager();

    if (!manifest_init_data_channels() || !manifest_init_parameter_channels())
        return false;

    channel_manifest_send(&manifest, manifest_sender);
    return true;
}
