/*
 * ref: Neurosky / ThinkGear Serial Stream Guide
 * http://developer.neurosky.com/docs/doku.php?id=thinkgear_communications_protocol
 */

#ifndef THINKGEAR_COMMON_H
#define THINKGEAR_COMMON_H


#define TG_OK               0x00
#define TG_NG               0xFF
#define TG_CANNOT_CONNECT   0x40

/*** code of thinkgearLib protocol ***/
#define SYNC_CODE           0xAA
#define EXTEND_CODE         0x55
#define BATTERY_CODE        0x01
#define POOR_CODE           0x02
#define HEART_RATE_CODE     0x03
#define ATTENTION_CODE      0x04
#define MEDITATION_CODE     0x05
#define RAW_WAVE_8BIT_CODE  0x06
#define RAW_MARKER_CODE     0x07
#define RAW_WAVE_CODE       0x80
#define EEG_POWER_CODE      0x81
#define ASIC_EEG_POWER_CODE 0x83
#define RR_INTERVAL_CODE    0x84

/*** structures for packet of thinkgearLib protocol ***/
typedef struct tg_eeg_power_t
{
    float delta = 0;
    float theta = 0;
    float low_alpha = 0;
    float high_alpha = 0;
    float low_beta = 0;
    float high_beta = 0;
    float low_gamma = 0;
    float mid_gamma = 0;
} tg_eeg_power_t;

typedef struct tg_asic_eeg_power_t
{
    unsigned int delta = 0;
    unsigned int theta = 0;
    unsigned int low_alpha = 0;
    unsigned int high_alpha = 0;
    unsigned int low_beta = 0;
    unsigned int high_beta = 0;
    unsigned int low_gamma = 0;
    unsigned int mid_gamma = 0;
} tg_asic_eeg_power_t;

typedef struct tg_data_t
{
    unsigned char battery = 0;          // 0-127
    unsigned char poor = 255;           // 0-255
    unsigned char heart_rate = 0;       // 0-255
    unsigned char attention = 0;        // 0-100
    unsigned char meditation = 0;       // 0-100
    unsigned char raw_wave_8bit = 0;    // 0-255
    unsigned char raw_marker = 255;     // 0
    int raw_wave = 0;                   /* a single big-endian 16-bit two's-compliment signed value
                                             (high-order byte followed by low-order byte) (-32768 to 32767) */
    tg_eeg_power_t eeg_power;           // eight big-endian 4-byte IEEE 754 floating point values
    tg_asic_eeg_power_t asic_eeg_power; // eight big-endian 3-byte unsigned integer values
    unsigned int rr_interval = 0;       /* two byte big-endian unsigned integer
                                           representing the milliseconds between two R-peaks */
} tg_data_t;

typedef struct tg_flag_t
{
    unsigned char battery = 0;
    unsigned char poor = 0;
    unsigned char heart_rate = 0;
    unsigned char attention = 0;
    unsigned char meditation = 0;
    unsigned char raw_wave_8bit = 0;
    unsigned char raw_marker = 0;
    unsigned char raw_wave = 0;
    unsigned char eeg_power = 0;
    unsigned char asic_eeg_power = 0;
    unsigned char rr_interval = 0;
} tg_flag_t;

typedef struct tg_packet_t
{
    tg_data_t data;
    tg_flag_t flag;
} tg_packet_t;


/*** definitions of prototype ***/
void tg_reset_data(tg_data_t *data);
void tg_reset_flags(tg_flag_t *flag);

unsigned char calc_checksum(const unsigned char *payload, unsigned char pLength);
void parse_payload(const unsigned char *payload, unsigned char pLength, tg_packet_t *packet);

char check_sync(unsigned char code);
void show_data(tg_packet_t packet);


#endif //THINKGEAR_COMMON_H
