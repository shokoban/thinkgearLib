
# ThinkgearLib for Mindwave

This code is for get data from Mindwave that is machine made by NeuroSky.

## 
なんか適当に作った


## Requirements
### linux
* libbluetooth-dev

## CMakeLists.txt
#### For windows.
``` CMake
cmake_minimum_required(VERSION 3.10)
project(thinkgear)

set(CMAKE_C_STANDARD 11)

file(GLOB THINKGEAR_LIB
    thinkgear/common.*
    thinkgear/thinkgear.*
)
add_executable(thinkgear main.c ${THINKGEAR_LIB})
```
#### For Ubuntu.
``` CMake
cmake_minimum_required(VERSION 3.10)
project(thinkgear)

set(CMAKE_C_STANDARD 11)

file(GLOB THINKGEAR_LIB
    thinkgear/common.*
    thinkgear/thinkgear.*
)
add_executable(thinkgear main.c ${THINKGEAR_LIB})
target_link_libraries(thinkgear bluetooth)
```

## main.c
```c
#include <stdio.h>
#include <time.h>

#include "thinkgearLib/thinkgear.h"
#ifdef _MSC_VER
#include <Windows.h>
#endif

int main(int argc, char **argv) {
#ifdef _MSC_VER
    char *port = "com3";
    handle hcom;
#else
    char port[18] = "XX:XX:XX:XX:XX:XX";
    int sckt;
#endif

    int time_measurement_sec = 10;
    tg_packet_t packet;

#ifdef _MSC_VER
    if (tg_connect(&hCom, port) == TG_CANNOT_CONNECT) {
#else
    if (tg_connect(&sckt, port) == TG_CANNOT_CONNECT) {
#endif
        printf("Error: Could NOT Connect to Bluetooth device. (code: 0x%x)", TG_CANNOT_CONNECT);
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

```
