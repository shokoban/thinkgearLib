#pragma once

#include "../core/common.h"

double tg_raw2uV(int raw_data);
void tg_show_poor(tg_packet_t packet);
void tg_show_raw_wave(tg_packet_t packet);
void tg_show_esense(tg_packet_t packet);
void tg_show_asic_data(tg_packet_t packet);
