/*
ref: Win32API(BorlandC++Compiler,VC++6.0など)
 http://0n0.fc2web.com/bcc/
ref: Win32アプリケーション開発 / シリアル通信 (Serial communication)
 https://so-zou.jp/software/tech/windows-app/win32/file/serial-communication.htm#no1
*/

#ifdef _WIN32

#include <Windows.h>

#include "../common.h"
#include "./thinkgear_win.h"

int tg_connect(HANDLE *hCom, TCHAR *port) {
    *hCom = CreateFile(
            port,
            GENERIC_READ,
            0,
            NULL,
            OPEN_EXISTING,
            FILE_ATTRIBUTE_NORMAL,
            NULL
    );
    if (*hCom == INVALID_HANDLE_VALUE)
        return TG_CANNOT_CONNECT;

    DCB dcb;
    GetCommState(*hCom, &dcb);
    dcb.BaudRate = CBR_57600;
    dcb.ByteSize = 8;
    dcb.Parity = NOPARITY;
    dcb.StopBits = ONESTOPBIT;
    SetCommState(*hCom, &dcb);

    return TG_OK;
}
void tg_disconnect(HANDLE hCom) {
    CloseHandle(hCom);
}

DWORD tg_get_payload(HANDLE hCom, unsigned char *buf, DWORD size) {
    DWORD rdCount;
    ReadFile(hCom, buf, size, &rdCount, NULL);

    return rdCount;
}

// To allocate memory area only once
static unsigned char buf[1], payload[256];
static unsigned char pLength, checksum;
void tg_get_packet(HANDLE hCom, tg_packet_t *packet) {
    tg_reset_packet(packet);

    while (1) {
        /* Synchronize on [SYNC] bytes */
        tg_get_payload(hCom, buf, 1);
        if (tg_check_sync(buf[0])) continue;
        tg_get_payload(hCom, buf, 1);
        if (tg_check_sync(buf[0])) continue;

        /* Parse [PLENGTH] byte */
        while (1) {
            tg_get_payload(hCom, buf, 1);
            pLength = buf[0];
            if (pLength != 170) break;
        }
        if (pLength > 170) continue;

        /* Collect [PAYLOAD...] bytes */
        tg_get_payload(hCom, payload, pLength);

        /* Parse [CKSUM] byte */
        tg_get_payload(hCom, buf, 1);
        /* Compute [PAYLOAD...] chksum */
        checksum = buf[0];
        /* Verify [PAYLOAD...] chksum against [CKSUM] */
        if (tg_calc_checksum(payload, pLength) != checksum) 
            continue;
        /* Since [CKSUM] is OK, parse the Data Payload */
        tg_parse_payload(payload, pLength, packet);
        break;
    }
};

#endif