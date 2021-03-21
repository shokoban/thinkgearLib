/*
 * ref: Neurosky / ThinkGear Serial Stream Guide
 * http://developer.neurosky.com/docs/doku.php?id=thinkgear_communications_protocol
 */

#include <stdio.h>
#include <string.h>
#include "common.h"

void tg_reset_data(tg_data_t *data) {
    data->battery       = 0;
    data->poor          = 255;
    data->heart_rate    = 0;
    data->attention     = 0;
    data->meditation    = 0;
    data->raw_wave_8bit = 0;
    data->raw_marker    = 255;
    data->raw_wave      = 0;
    memset(&data->eeg_power,      0, sizeof(data->eeg_power)     );
    memset(&data->asic_eeg_power, 0, sizeof(data->asic_eeg_power));
    data->rr_interval   = 0;
}

void tg_reset_flags(tg_flag_t *flag) {
    memset(flag, 0, sizeof(*flag));
}

unsigned char calc_checksum(const unsigned char *payload, unsigned char pLength) {
    int chechsum = 0;
    for (int i = 0; i < pLength; i++)
        chechsum += payload[i];
    chechsum &= 0xFF;
    chechsum = ~chechsum & 0xFF;

    return chechsum;
}

char check_sync(unsigned char code){
    return code == SYNC_CODE? TG_OK: TG_NG;
}

//void parse_payload(const unsigned char *payload, unsigned char pLength, tg_data_t *packet, tg_recv_flag_t *flags) {
void parse_payload(const unsigned char *payload, unsigned char pLength, tg_packet_t *packet) {
    for (int i = 0; i < pLength; ++i) {
        switch (payload[i]) {
            case BATTERY_CODE:
                packet->data.battery = payload[++i];
                packet->flag.battery = 1;
                break;
            case POOR_CODE:
                packet->data.poor = payload[++i];
                packet->flag.poor = 1;
                break;
            case HEART_RATE_CODE:
                packet->data.heart_rate = payload[++i];
                packet->flag.heart_rate = 1;
                break;
            case ATTENTION_CODE:
                packet->data.attention = payload[++i];
                packet->flag.attention = 1;
                break;
            case MEDITATION_CODE:
                packet->data.meditation = payload[++i];
                packet->flag.meditation = 1;
                break;
            case RAW_WAVE_8BIT_CODE:
                packet->data.raw_wave_8bit = payload[++i];
                packet->flag.raw_wave_8bit = 1;
                break;
            case RAW_MARKER_CODE:
                // TODO: check payload. maybe, correct?
                packet->data.raw_marker = payload[++i];
                packet->flag.raw_marker = 1;
                break;
            case RAW_WAVE_CODE:
                i++; // length
                packet->data.raw_wave = payload[++i] << 8;
                packet->data.raw_wave += payload[++i];
                if (packet->data.raw_wave >= 32768)
                    packet->data.raw_wave -= 65536;
                packet->flag.raw_wave = 1;
                break;
            case EEG_POWER_CODE:
                // TODO: implement
                i++; // length
                i += 32;
                // flags->eeg_power = 1;
                break;
            case ASIC_EEG_POWER_CODE:
                i++; // length
                packet->data.asic_eeg_power.delta = (payload[++i] << 16);
                packet->data.asic_eeg_power.delta += (payload[++i] << 8);
                packet->data.asic_eeg_power.delta += payload[++i];
                packet->data.asic_eeg_power.theta = (payload[++i] << 16);
                packet->data.asic_eeg_power.theta += (payload[++i] << 8);
                packet->data.asic_eeg_power.theta += payload[++i];
                packet->data.asic_eeg_power.low_alpha = (payload[++i] << 16);
                packet->data.asic_eeg_power.low_alpha += (payload[++i] << 8);
                packet->data.asic_eeg_power.low_alpha += payload[++i];
                packet->data.asic_eeg_power.high_alpha = (payload[++i] << 16);
                packet->data.asic_eeg_power.high_alpha += (payload[++i] << 8);
                packet->data.asic_eeg_power.high_alpha += payload[++i];
                packet->data.asic_eeg_power.low_beta = (payload[++i] << 16);
                packet->data.asic_eeg_power.low_beta += (payload[++i] << 8);
                packet->data.asic_eeg_power.low_beta += payload[++i];
                packet->data.asic_eeg_power.high_beta = (payload[++i] << 16);
                packet->data.asic_eeg_power.high_beta += (payload[++i] << 8);
                packet->data.asic_eeg_power.high_beta += payload[++i];
                packet->data.asic_eeg_power.low_gamma = (payload[++i] << 16);
                packet->data.asic_eeg_power.low_gamma += (payload[++i] << 8);
                packet->data.asic_eeg_power.low_gamma += payload[++i];
                packet->data.asic_eeg_power.mid_gamma = (payload[++i] << 16);
                packet->data.asic_eeg_power.mid_gamma += (payload[++i] << 8);
                packet->data.asic_eeg_power.mid_gamma += payload[++i];
                packet->flag.asic_eeg_power = 1;
                break;
            case RR_INTERVAL_CODE:
                // TODO: check payload. maybe, correct?
                i++; // length
                packet->data.rr_interval = payload[++i] << 8;
                packet->data.rr_interval += payload[++i];
                packet->flag.rr_interval = 1;
                break;
//            case 0x00: // unknown codes
//            case 0xba:
//            case 0xbc:
            default:
                i++;
                break;
        }
    }

}

void show_data(tg_packet_t packet) {
    if (packet.flag.poor) {
        printf("poor : %d, ",         packet.data.poor);
        printf("\n");
        printf("attention : %d, ",    packet.data.attention);
        printf("meditation : %d, ",   packet.data.meditation);
        printf("\n");
        printf("delta: %d, ",         packet.data.asic_eeg_power.delta);
        printf("theta: %d, ",         packet.data.asic_eeg_power.theta);
        printf("\n");
        printf("low_alpha: %d, ",     packet.data.asic_eeg_power.low_alpha);
        printf("high_alpha: %d, ",    packet.data.asic_eeg_power.high_alpha);
        printf("low_beta: %d, ",      packet.data.asic_eeg_power.low_beta);
        printf("high_beta: %d, ",     packet.data.asic_eeg_power.high_beta);
        printf("low_gamma: %d, ",     packet.data.asic_eeg_power.low_gamma);
        printf("mid_gamma: %d, ",     packet.data.asic_eeg_power.mid_gamma);
        printf("\n");
    }
//    if (packet.flag.raw_wave)
//        printf("raw : %d\n",          packet.data.raw_wave);
}