#include <stdio.h>
#include "utils.h"

/*
 * https://www.neurosky.jp/faq/
 * ( RawData * (1.8 / 4096) ) / 0.002 [uV]
 */
double tg_raw2uV(int raw_data)
{
    return (raw_data * (1.8 / 4096)) / 0.002;
}

void tg_show_poor(tg_packet_t packet)
{
    printf("poor\t\t: %d\n", packet.data.poor);
}

void tg_show_raw_wave(tg_packet_t packet)
{
    printf("raw\t\t: %d\n", packet.data.raw_wave);
    printf("raw[uV]\t\t: %lf\n", tg_raw2uV(packet.data.raw_wave));
}

void tg_show_esense(tg_packet_t packet)
{
    printf("attention\t: %d\n", packet.data.attention);
    printf("meditation\t: %d\n", packet.data.meditation);
}

void tg_show_asic_data(tg_packet_t packet)
{
    printf("delta\t\t: %d\n", packet.data.asic_eeg_power.delta);
    printf("theta\t\t: %d\n", packet.data.asic_eeg_power.theta);
    printf("low_alpha\t: %d\n", packet.data.asic_eeg_power.low_alpha);
    printf("high_alpha\t: %d\n", packet.data.asic_eeg_power.high_alpha);
    printf("low_beta\t: %d\n", packet.data.asic_eeg_power.low_beta);
    printf("high_beta\t: %d\n", packet.data.asic_eeg_power.high_beta);
    printf("low_gamma\t: %d\n", packet.data.asic_eeg_power.low_gamma);
    printf("mid_gamma\t: %d\n", packet.data.asic_eeg_power.mid_gamma);
}
