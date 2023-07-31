#include <string.h>
#include "./common.h"

void _reset_data(tg_data_t *data)
{
    data->battery = 0;
    data->poor = 255;
    data->heart_rate = 0;
    data->attention = 0;
    data->meditation = 0;
    data->raw_wave_8bit = 0;
    data->raw_marker = 255;
    data->raw_wave = 0;
    memset(&(data->eeg_power), 0, sizeof(data->eeg_power));
    memset(&(data->asic_eeg_power), 0, sizeof(data->asic_eeg_power));
    data->rr_interval = 0;
}

void _reset_flags(tg_flag_t *flag)
{
    memset(flag, 0, sizeof(*flag));
}

void tg_reset_packet(tg_packet_t *packet)
{

    _reset_data(&(packet->data));
    _reset_flags(&(packet->flag));
}

unsigned char tg_calc_checksum(const unsigned char *payload, unsigned char pLength)
{
    int chechsum = 0;
    for (int i = 0; i < pLength; i++)
        chechsum += payload[i];
    chechsum &= 0xFF;
    chechsum = ~chechsum & 0xFF;

    return chechsum;
}

char tg_check_sync(unsigned char code)
{
    return code == TG_CODE_SYNC ? TG_OK : TG_NG;
}

void tg_parse_payload(const unsigned char *payload, unsigned char pLength, tg_packet_t *packet)
{
    for (int i = 0; i < pLength; ++i)
    {
        switch (payload[i])
        {
        case TG_CODE_BATTERY:
            packet->data.battery = payload[++i];
            packet->flag.battery = 1;
            break;
        case TG_CODE_POOR:
            packet->data.poor = payload[++i];
            packet->flag.poor = 1;
            break;
        case TG_CODE_HEART_RATE:
            packet->data.heart_rate = payload[++i];
            packet->flag.heart_rate = 1;
            break;
        case TG_CODE_ATTENTION:
            packet->data.attention = payload[++i];
            packet->flag.attention = 1;
            break;
        case TG_CODE_MEDITATION:
            packet->data.meditation = payload[++i];
            packet->flag.meditation = 1;
            break;
        case TG_CODE_RAW_WAVE_8BIT:
            packet->data.raw_wave_8bit = payload[++i];
            packet->flag.raw_wave_8bit = 1;
            break;
        case TG_CODE_RAW_MARKER:
            // TODO: check payload. maybe, correct?
            packet->data.raw_marker = payload[++i];
            packet->flag.raw_marker = 1;
            break;
        case TG_CODE_RAW_WAVE:
            i++; // length
            packet->data.raw_wave = payload[++i] << 8;
            packet->data.raw_wave += payload[++i];
            if (packet->data.raw_wave >= 32768)
                packet->data.raw_wave -= 65536;
            packet->flag.raw_wave = 1;
            break;
        case TG_CODE_EEG_POWER:
            // TODO: implement
            i++; // length
            i += 32;
            // flags->eeg_power = 1;
            break;
        case TG_CODE_ASIC_EEG_POWER:
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
        case TG_CODE_RR_INTERVAL:
            // TODO: check payload. maybe, correct?
            i++; // length
            packet->data.rr_interval = payload[++i] << 8;
            packet->data.rr_interval += payload[++i];
            packet->flag.rr_interval = 1;
            break;
        // case 0x00: // unknown codes
        //     i++;
        //     break;
        // case 0xba: // unknown codes
        //     i++;
        //     break;
        // case 0xbc: // unknown codes
        //     i++;
        //     break;
        default:
            i++;
            break;
        }
    }
}
