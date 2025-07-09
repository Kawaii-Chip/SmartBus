#include <gps_app.h>

extern double latitude = 0; // 存储维度消息
extern float longitude = 0;     // 存储维度消息

struct GPS
{
    u8 Frame_Buffer[526];
     u8 buff_length;
     u8 buff_flag;
     char UTCTime[10];
     char GPS_Status[10];
    char GPS_lat[10];
    char GPS_ulat[10];
    char GPS_lon[10];
    char GPS_ulon[10];
    char GPS_spd[10];
    char GPS_cog[10];
    char GPS_date[10];
};
struct GPS gps;

unsigned char str1[] = "$PCAS03,0,0,0,0,1,0,0,0,0,0,,,1,0*02\r\n";

void gps_init()
{
    uart8_printf(str1);
}

void gps_arrive()
{
    current_station = next_station;
    u8 i = 30 + current_station;
    audio_play(i);
    audio_play(50);
    printf("到达%d站\r\n", current_station);
    processDropoffs(current_station, dropoffs);
    next_station = findNextStation(station_requests, dropoffs);
    i = 30 + next_station;
    printf("i:%d\r\n", i);
    audio_play(i);
}

char *point = NULL;
char *nextpoint = NULL;
uint8_t tempvar = 0;

void gps_proc()
{
     if (uart8_rec_index == 0)
         return;
    u32 now_time = HAL_GetTick();

    if (now_time >= uart8_rec_tick + 10)
    {

        if (strstr(uart8_rec_string, "GNRMC") != NULL)
        {
            for (tempvar = 0; tempvar < 10; tempvar++)
            {
                if (tempvar == 0)
                {
                    if ((point = strstr(uart8_rec_string, ",")) == NULL)
                    {
                        printf("Prase error\r\n");
                    }
                }
                else
                {
                    point++;
                    if ((nextpoint = strstr(point, ",")) != NULL)
                    {

                        switch (tempvar)
                        {

                        case 1: // 时间
                            memcpy(gps.UTCTime, point, nextpoint - point);
                            break;
                        case 2: // 时间
                            memcpy(gps.GPS_Status, point, nextpoint - point);
                            break;
                        case 3: //
                            memcpy(gps.GPS_lat, point, 10);
                            break;
                        // case 4: //
                        //     memcpy(gps.GPS_ulat, point, nextpoint - point);
                        //     break;
                        case 5: //
                            memcpy(gps.GPS_lon, point, 10);
                            break;
                        // case 6: //
                        //     memcpy(gps.GPS_ulon, point, nextpoint - point);
                        //     break;
                        case 7: //
                            memcpy(gps.GPS_spd, point, nextpoint - point);
                            break;
                        case 8: //
                            memcpy(gps.GPS_cog, point, nextpoint - point);
                            break;
                        case 9: //
                            memcpy(gps.GPS_date, point, nextpoint - point);
                            break;
                        }
                        point = nextpoint;
                    }
                    else
                    {
                        printf("Prase error\r\n");
                    }
                }
            }
            // 解析数据
            // printf("lat:%s  lon:%s\r\n", gps.GPS_lat, gps.GPS_lon);
            //printf("%s\r\n", uart8_rec_string);
            memset(uart8_rec_string, 0, uart8_rec_index);
            uart8_rec_index = 0;
            Data_Transfor();
        }
        
    }
}

void Data_Transfor(void)
{
    
    float time = 0;      // 存储时间消息
    uint16_t temp1 = 0;  // 临时变量1 存储整数
    uint16_t temp2 = 0;  // 临时变量2 存储整数
    uint16_t temp3 = 0;  // 临时变量3 存储整数

    latitude = strtod(gps.GPS_lat, NULL);
    longitude = strtod(gps.GPS_lon, NULL);
    time = strtod(gps.UTCTime, NULL);

    
    
        temp1 = (((u16)latitude / 1000) % 10) * 10 + (((u16)latitude / 100) % 10);
        latitude = latitude - (float)temp1 * 100;
        latitude = (float)temp1 + latitude / 60;

   
        // 前三位单独拿出来组成一个数
        temp1 = (((u16)longitude / 10000) % 10) * 100 + (((u16)longitude / 1000) % 10) * 10 + (((u16)longitude / 100) % 10);
        longitude = longitude - (float)temp1 * 100;
        longitude = (float)temp1 + longitude / 60;

        printf("AT+MQTTPUB=0,\"$sys/mmMJx639W9/ch32/thing/property/post\",\"{\\\"id\\\":\\\"123\\\"\\,\\\"params\\\":{\\\"address\\\":{\\\"value\\\":[%f\\,%f\\,%d]}}}\",0,0\r\n", latitude, longitude, next_station);

        rt_longitude = (u32)(longitude * 10000);
        rt_latitude = (u32)(latitude * 10000);

        
}
int time1[3];

void show_td()
{
    u8 char1 = '~';
    int date[3];
    sscanf(gps.UTCTime, "%2d%2d%2d", &time1[0], &time1[1], &time1[2]);
    sscanf(gps.GPS_date, "%2d%2d%2d", &date[2], &date[1], &date[0]);
    // time[0] = gps.UTCTime[0] * 10 + gps.UTCTime[1];
    // time[1] = gps.UTCTime[2] * 10 + gps.UTCTime[3];
    // time[2] = gps.UTCTime[4] * 10 + gps.UTCTime[5];
    // date[1] = gps.GPS_date[2] * 10 + gps.GPS_date[3];
    // date[2] = gps.GPS_date[0] * 10 + gps.GPS_date[1];

    LCD_ShowIntNum(180, 200, time1[0]+8, 2, BLACK, WHITE, 32);
    LCD_ShowChar(212, 200, char1, BLACK, WHITE, 32, 0);
    LCD_ShowIntNum(228, 200, time1[1], 2, BLACK, WHITE, 32);
    LCD_ShowChar(260, 200, char1, BLACK, WHITE, 32, 0);
    LCD_ShowIntNum(276, 200, time1[2], 2, BLACK, WHITE, 32);

    LCD_ShowIntNum(0, 200, date[1], 2, BLACK, WHITE, 32);
    LCD_ShowChinese(32, 200, "月", BLUE, WHITE, 32, 0);
    LCD_ShowIntNum(64, 200, date[2], 2, BLACK, WHITE, 32);
    LCD_ShowChinese(96, 200, "日", BLUE, WHITE, 32, 0);
}

void RTC_GetTime(unsigned char *time2)
{
    time2[0] = time1[0];
    time2[1] = time1[1];
    time2[2] = time1[2];
}