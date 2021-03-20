#ifndef THINKGEAR_TG_WIN_H
#define THINKGEAR_TG_WIN_H

#include "common.h"

#ifdef _WIN32
#include <Windows.h>

int tg_connect(HANDLE *hCom, char *port);
void tg_disconnect(HANDLE hCom);
void tg_get_packet(HANDLE hCom, tg_packet_t *packet);
#else
int tg_connect(int *socket, char *mac_addr);
void tg_disconnect(int socket);
void tg_get_packet(int sckt, tg_packet_t *packet);
#endif

#endif //THINKGEAR_TG_WIN_H
