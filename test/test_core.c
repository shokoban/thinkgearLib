#include <stdio.h>
#include "test_core.h"

/// @brief this is a test function for _reset_data()
/// @param
void test_common_reset_data(void)
{
    // After executing _reset_data, success if poor is 255, otherwise failure.
    printf("test_common_reset_data: ");

    tg_data_t data;
    _reset_data(&data);
    if (data.poor == 255)
    {
        printf("success\n");
    }
    else
    {
        printf("failure\n");
    }
}

/// @brief this is a test function for _reset_flag()
/// @param
void test_common_reset_flag(void)
{
    // After executing _reset_flag, success if poor is 0, otherwise failure.
    printf("test_common_reset_flag: ");

    tg_flag_t flag;
    _reset_flags(&flag);
    if (flag.poor == 0)
    {
        printf("success\n");
    }
    else
    {
        printf("failure\n");
    }
}

/// @brief this is a test function for tg_reset_packet()
/// @param
void test_tg_reset_packet(void)
{
    // After executing tg_reset_packet, success if poor is 255, otherwise failure.
    printf("test_tg_reset_packet: ");

    tg_packet_t packet;
    tg_reset_packet(&packet);
    if (packet.data.poor == 255)
    {
        printf("success\n");
    }
    else
    {
        printf("failure\n");
    }
}

// this is test function for tg_show_data
void test_tg_show_data()
{
    tg_packet_t packet;
    tg_reset_packet(&packet);
    tg_show_data(packet);
}

// this is test function for tg_check_sync. (normal system)
void test_tg_check_sync()
{
    unsigned char code = TG_CODE_SYNC;
    char result = tg_check_sync(code);
    // if result is TG_OK, show success.
    // else, show fail.
    if (result == TG_OK)
    {
        printf("success\n");
    }
    else
    {
        printf("fail\n");
    }
}

// this is test function for tg_check_sync. (abnormal system)
void test_tg_check_sync_abnormal()
{
    unsigned char code = TG_CANNOT_CONNECT;
    char result = tg_check_sync(code);
    printf("result: %d\n", result);
    // if result is TG_NG, show success.
    // else, show fail.
    if (result == (char)TG_NG)
    {
        printf("success\n");
    }
    else
    {
        printf("fail\n");
    }
}