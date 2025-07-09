#include "onenet_app.h"

u8 string1[] = "AT+RST\r\n";
u8 string2[] = "AT+CWMODE=1\r\n";
u8 string3[] = "AT+CWDHCP=1,1\r\n";
u8 string4[] = "AT+CWJAP=\"firstlearnt\",\"2004092827b\"\r\n";
u8 string5[] = "AT+MQTTUSERCFG=0,1,\"ch32\",\"mmMJx639W9\",\"version=2018-10-31&res=products%%2FmmMJx639W9%%2Fdevices%%2Fch32&et=9999999999&method=md5&sign=dG31R2Gk%%2F2soadnYdqG7og%%3D%%3D\",0,0,\"\"\r\n";

//"AT+MQTTUSERCFG=0,1,\"ch32\",\"56HeNPU4AK\",\"version=2018-10-31&res=products%2F56HeNPU4AK%2Fdevices%2Fch32&et=1745065821&method=md5&sign=eMHITyfOpd8di%2FQQjks%2Byg%3D%3D\",0,0,\"\"\r\n";
// AT+MQTTUSERCFG=0,1,"ch32","mmMJx639W9","version=2018-10-31&res=products%2FmmMJx639W9%2Fdevices%2Fch32&et=9999999999&method=md5&sign=dG31R2Gk%2F2soadnYdqG7og%3D%3D",0,0,""

u8 string6[] = "AT+MQTTCONN=0,\"mqtts.heclouds.com\",1883,1\r\n";
u8 string7[] = "AT+MQTTSUB=0,\"$sys/mmMJx639W9/ch32/thing/property/set\",1\r\n";
u8 string8[] = "AT+MQTTPUB=0,\"$sys/mmMJx639W9/ch32/thing/property/post\",\"{\\\"id\\\":\\\"123\\\"\\,\\\"params\\\":{\\\"appkey\\\":{\\\"value\\\":11}}}\",0,0\r\n";
void esp8266_init()
{
    Uart6_Init();
}

void onenet_init()
{
    printf(string1);
    fflush(stdout);
    Delay_Ms(1000);
    printf(string2);
    fflush(stdout);
    Delay_Ms(1000);
    printf(string3);
    fflush(stdout);
    Delay_Ms(1000);
    printf(string4);
    fflush(stdout);
    Delay_Ms(1000);
    Delay_Ms(1000);
    printf(string5);
    fflush(stdout);
    Delay_Ms(1000);
    printf(string6);
    fflush(stdout);
    Delay_Ms(1000);
    Delay_Ms(1000);
    printf(string7);
    fflush(stdout);
    Delay_Ms(1000);
    Delay_Ms(1000);
}

u8 index_old;
void onenet_proc()
{
    if (uart1_rec_index == 0)
        return;

    u32 now_time = HAL_GetTick();
    if (now_time >= uart1_rec_tick + 10)
    {

        char *add1 = strstr(uart1_rec_string, "appkey");
        char *add2;
        int i = 0;
        int appkey = 0;
        sscanf(add1, "appkey\":%d", &appkey);

        switch (appkey)
        {
        case 1:
            add2 = strstr(uart1_rec_string, "passenger");
            sscanf(add2, "passenger\":\"%c", &address[rfid_password_index]);
            address[rfid_password_index] -= 48;
            printf("正在添加乘客\r\n");
            audio_play(60);
            flag = 2;
            break;
        // case 2:
        //     gps_read();

        //         break;
        
        case 3:
            showreport();
            break;
        // case 4:
        //     read_temperature();
        //     break;
        case 5:
            add2 = strstr(uart1_rec_string, "quest");
            sscanf(add2, "quest\":\"%c%c", &request_handle[0], &request_handle[1]);
            if (request_handle[0] == '1')
            {
                station_requests[request_handle[1]-48] = 1;
                u8 i = request_handle[1]-48+30;
                printf("i:%d\r\n", i);
                audio_play(i);
                audio_play(40);

                 printf("%c站有乘客等待上车\r\n", request_handle[1]);
                request_handle[0] = 0;
                next_station = findNextStation(station_requests, dropoffs);
            }
            break;
        case 6:
            add2 = strstr(uart1_rec_string, "advice");
            u8 advice=0;
            sscanf(add2, "advice\":\"%c", &advice);
            printf("advice:%c\r\n", advice);
            if (advice=='1')
            {
                printf("fan:1\r\n");
                fan(0);
            }
            else if (advice == '2')
                fan(1);
            else if (advice == '3')
                led(0);
            else if (advice == '4')
                led(1);
            else if(advice=='5')
                audio_play(75);
            else if (advice == '6')
                audio_play(76);
            break;
       
        case 10:
            gps_arrive();

            break;
        }
        
        memset(uart1_rec_string, 0, uart1_rec_index);
        uart1_rec_index = 0;
    }
}
