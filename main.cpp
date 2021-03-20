#include <cstdio>
#include <ctime>

#include "thinkgearLib/thinkgear.h"
#ifdef _MSC_VER
#include <Windows.h>
#endif

int main(int argc, char **argv) {
#ifdef _MSC_VER
    char *port = "COM3";
    HANDLE hCom;
#else
    char port[18] = "XX:XX:XX:XX:XX:XX";
    int sckt;
#endif

    int time_measurement_sec = 10;
    int state;
    tg_packet_t packet;


#ifdef _MSC_VER
    if ((state = tg_connect(&hCom, port)) == TG_CANNOT_CONNECT) {
#else
    if ((state = tg_connect(&sckt, port)) == TG_CANNOT_CONNECT) {
#endif
        printf("Error: Could NOT Connect to Bluetooth device. (code: 0x%x)", state);
        return 1;
    }
    printf("Connected\n");

    clock_t start = clock();
    while (1) {
#ifdef _MSC_VER
        tg_get_packet(hCom, &packet);
#else
        tg_get_packet(sckt, &packet);
#endif
        show_data(packet);

        if (((clock() - start) / CLOCKS_PER_SEC) >= time_measurement_sec)
            break;
    }

#ifdef _MSC_VER
    tg_disconnect(hCom);
#else
    tg_disconnect(sckt);
#endif
    printf("Disconnected\n");

    return 0;
}
