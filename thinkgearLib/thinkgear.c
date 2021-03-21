#include "thinkgear.h"
/*
ref: ThinkGear Serial Stream Guide
http://developer.neurosky.com/docs/doku.php?id=thinkgear_communications_protocol
*/

#ifdef _MSC_VER
/*
ref: Win32API(BorlandC++Compiler,VC++6.0など)
 http://0n0.fc2web.com/bcc/
ref: Win32アプリケーション開発 / シリアル通信 (Serial communication)
 https://so-zou.jp/software/tech/windows-app/win32/file/serial-communication.htm#no1
*/

#include <Windows.h>

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
    tg_reset_data(&packet->data);
    tg_reset_flags(&packet->flag);

    while (1) {
        /* Synchronize on [SYNC] bytes */
        tg_get_payload(hCom, buf, 1);
        if (check_sync(buf[0])) continue;
        tg_get_payload(hCom, buf, 1);
        if (check_sync(buf[0])) continue;

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
        checksum = buf[0];
        /* Verify [PAYLOAD...] chksum against [CKSUM] */
        if (calc_checksum(payload, pLength) != checksum) 
            continue;
        /* Since [CKSUM] is OK, parse the Data Payload */
        parse_payload(payload, pLength, packet);
        break;
    }
};
#else
/*
ref: An Introduction to Bluetooth Programming / Chapter 4. Bluetooth programming in C with BlueZ
https://people.csail.mit.edu/albert/bluez-intro/x502.html
 */
#include <unistd.h>
#include <sys/socket.h>
#include <bluetooth/bluetooth.h>   // require: libbluetooth-dev
#include <bluetooth/rfcomm.h>   // require: libbluetooth-dev

int tg_connect(int *sckt, char *mac_addr) {
    struct sockaddr_rc addr = {0};
    int status;

    // allocate a socket
    *sckt = socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM);

    // set the connection parameters (who to connect to)
    addr.rc_family = AF_BLUETOOTH;
    addr.rc_channel = 1;
    str2ba(mac_addr, &addr.rc_bdaddr);

    // connect to server
    status = connect(*sckt, (struct sockaddr *) &addr, sizeof(addr));
    if (status != 0)
        return TG_CANNOT_CONNECT;

    return TG_OK;
}

void tg_disconnect(int s) {
    close(s);
}

int tg_get_payload(int s, unsigned char *buf, int size) {
    int rdCount;
    rdCount = read(s, buf, size);

    return rdCount;
}

// To allocate memory area only once
static unsigned char buf[1], payload[256];
static unsigned char pLength, checksum;
void tg_get_packet(int sckt, tg_packet_t *packet) {
    tg_reset_data(&(packet->data));
    tg_reset_flags(&(packet->flag));

    while (1) {
        /* Synchronize on [SYNC] bytes */
        tg_get_payload(sckt, buf, 1);
        if (check_sync(buf[0])) continue;
        tg_get_payload(sckt, buf, 1);
        if (check_sync(buf[0])) continue;

        /* Parse [PLENGTH] byte */
        while (1) {
            tg_get_payload(sckt, buf, 1);
            pLength = (int) buf[0];
            if (pLength != 170) break;
        }
        if (pLength > 170) continue;

        /* Collect [PAYLOAD...] bytes */
        tg_get_payload(sckt, payload, pLength);

        /* Parse [CKSUM] byte */
        tg_get_payload(sckt, buf, 1);
        /* Compute [PAYLOAD...] chksum */
        checksum = buf[0];
        /* Verify [PAYLOAD...] chksum against [CKSUM] */
        if (calc_checksum(payload, pLength) != checksum)
            continue;
        /* Since [CKSUM] is OK, parse the Data Payload */
        parse_payload(payload, pLength, packet);
        break;
    }
};
#endif