#include <stdio.h>
#include <time.h>

#ifdef _WIN32
#include <Windows.h>
#endif
#include "thinkgearLib/thinkgear.h"

int main(int argc, char **argv)
{
    // difine socket and port
#ifdef _WIN32
    handle_t sckt;
    char *port = "com3";
#elif __GNUC__
    int sckt;
    char *port = "C4:64:E3:E8:CF:DD";
#endif

    printf("Connecting to Neurosky device.\n");
    // connect to the device
    tg_packet_t packet;
    if ((tg_connect(&sckt, port)) == TG_CANNOT_CONNECT)
    {
        // if the connection fails, print the error
        printf("Error:  Could NOT Connect to Neurosky device. (code: 0x%x)\n", TG_CANNOT_CONNECT);
        return 1;
    }
    printf("Connected to Neurosky device.\n");

    // get the data for 30 seconds
    time_t start = time(NULL);
    while (time(NULL) - start < 30)
    {
        // Packet acquisition
        // Raw data is acquired every 1/512 of a second.
        // Other data is acquired every second.
        tg_get_packet(sckt, &packet);

        // Print data
        if (packet.flag.raw_wave != 0)
        {
            printf("********** raw wave **********\n");
            tg_show_raw_wave(packet);
            printf("******************************\n");
        }
        // poor signal and other data
        if (packet.flag.poor != 0)
        {
            printf("************ data ************\n");
            tg_show_poor(packet);
            tg_show_esense(packet);
            tg_show_asic_data(packet);
            printf("******************************\n");
        }
    }

    // disconnect from the device
    tg_disconnect(sckt);
    printf("Disconnected from Neurosky device.\n");

    return 0;
}