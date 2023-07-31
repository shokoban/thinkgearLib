#pragma once

/*** status of thinkgearLib protocol ***/
#define TG_OK 0x00
#define TG_NG 0xFF
#define TG_CANNOT_CONNECT 0x40

/*** code of thinkgearLib protocol ***/
#define TG_CODE_SYNC 0xAA
#define TG_CODE_EXTEND 0x55
#define TG_CODE_BATTERY 0x01
#define TG_CODE_POOR 0x02
#define TG_CODE_HEART_RATE 0x03
#define TG_CODE_ATTENTION 0x04
#define TG_CODE_MEDITATION 0x05
#define TG_CODE_RAW_WAVE_8BIT 0x06
#define TG_CODE_RAW_MARKER 0x07
#define TG_CODE_RAW_WAVE 0x80
#define TG_CODE_EEG_POWER 0x81
#define TG_CODE_ASIC_EEG_POWER 0x83
#define TG_CODE_RR_INTERVAL 0x84

/*** structures for packet of thinkgearLib protocol ***/
typedef struct
{
    unsigned int delta;
    unsigned int theta;
    unsigned int low_alpha;
    unsigned int high_alpha;
    unsigned int low_beta;
    unsigned int high_beta;
    unsigned int low_gamma;
    unsigned int mid_gamma;
} tg_asic_eeg_power_t;

typedef struct
{
    float delta;
    float theta;
    float low_alpha;
    float high_alpha;
    float low_beta;
    float high_beta;
    float low_gamma;
    float mid_gamma;
} tg_eeg_power_t;

typedef struct
{
    unsigned char battery;              // 0-127
    unsigned char poor;                 // 0-255
    unsigned char heart_rate;           // 0-255
    unsigned char attention;            // 0-100
    unsigned char meditation;           // 0-100
    unsigned char raw_wave_8bit;        // 0-255
    unsigned char raw_marker;           // 0
    int raw_wave;                       // a single big-endian 16-bit two's-compliment signed value
                                        // (high-order byte followed by low-order byte) (-32768 to 32767)
    tg_eeg_power_t eeg_power;           // eight big-endian 4-byte IEEE 754 floating point values
    tg_asic_eeg_power_t asic_eeg_power; // eight big-endian 3-byte unsigned integer values
    unsigned int rr_interval;           // two byte big-endian unsigned integer
                                        // representing the milliseconds between two R-peaks

} tg_data_t;

typedef struct
{
    unsigned char battery;
    unsigned char poor;
    unsigned char heart_rate;
    unsigned char attention;
    unsigned char meditation;
    unsigned char raw_wave_8bit;
    unsigned char raw_marker;
    unsigned char raw_wave;
    unsigned char eeg_power;
    unsigned char asic_eeg_power;
    unsigned char rr_interval;
} tg_flag_t;

typedef struct
{
    tg_data_t data;
    tg_flag_t flag;
} tg_packet_t;

void tg_reset_packet(tg_packet_t *packet);
unsigned char tg_calc_checksum(const unsigned char *payload, unsigned char pLength);
char tg_check_sync(unsigned char code);
void tg_parse_payload(const unsigned char *payload, unsigned char pLength, tg_packet_t *packet);
