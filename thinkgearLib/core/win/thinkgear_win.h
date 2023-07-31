#pragma once

#include "../common.h"

#ifdef _WIN32

int tg_connect(HANDLE *hCom, TCHAR *port);
void tg_disconnect(HANDLE hCom);
void tg_get_packet(HANDLE hCom, tg_packet_t *packet);

#endif
