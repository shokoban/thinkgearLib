#pragma once

#include "../common.h"

#ifdef __GNUC__

int tg_connect(int *socket, char *mac_addr);
void tg_disconnect(int socket);
void tg_get_packet(int sckt, tg_packet_t *packet);

#endif
