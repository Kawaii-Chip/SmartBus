/**************************************************************************************************
多级菜单界面
**************************************************************************************************/
#include <bsp_system.h>

#define bus_wait_time 5000 // 设置公交车的站台等待时间(ms)



/*
1、宏定义每个站点经纬度范围（整数形式），这些值在具体站点的GPS经纬度显示实验测得的
2、关于同一纬度时，经度差的距离公式为：d=cos(纬度) * 经度差 * 113190米
   这里我设置的经度整数形式范围为20，即经度差0.0002，计算器预估站点的纬度范围为21米左右
3、纬度差的距离公式为：d=纬度差 * 113190米
   这里我设置的经度整数形式范围为20，即经度差0.0002，计算器预估站点的纬度范围为22.6米左右

所以下面站点经纬度整数设置的差值为20合理，理论上在离站点10m的地方就会报站
*/
// 站点1测得的经纬度大概为119.54505°E 39.92343°N

// 定义二维数组，存储3个站点的经纬度范围值（每个站点包含4个32位整数）
const int32_t station_ranges[3][4] = {
    // 站点1：longitude_MIN, longitude_MAX, latitude_MIN, latitude_MAX
    {11954495, 11954515, 3992333, 3992353},

    // 站点2：对应上述宏定义的值
    {11954474, 11954494, 3992358, 3992378},

    // 站点3：对应上述宏定义的值
    {11954470, 11954490, 3992223, 3992243}
};
//工学馆 1
#define stop1_longitude_MIN 11954495
#define stop1_longitude_MAX 11954515
#define stop1_latitude_MIN 3992333
#define stop1_latitude_MAX 3992353

//鹏远  站点2测得的经纬度大概为119.54484°E 39.92368°N
#define stop2_longitude_MIN 11954474
#define stop2_longitude_MAX 11954494
#define stop2_latitude_MIN 3992358
#define stop2_latitude_MAX 3992378

//西操 站点3测得的经纬度大概为119.54480°E 39.92233°N
#define stop3_longitude_MIN 11954470
#define stop3_longitude_MAX 11954490
#define stop3_latitude_MIN 3992223
#define stop3_latitude_MAX 3992243

// 站点4测得的经纬度大概为113.38990E 23.11130N
#define stop4_longitude_MIN 11338980
#define stop4_longitude_MAX 11339000
#define stop4_latitude_MIN 2311120
#define stop4_latitude_MAX 2311140

/*语音文件数组*/
uint8_t ZH_Buffer1[3] = {1, 2, 3};
uint8_t ZH_Buffer2[3] = {4, 5, 6};
uint8_t ZH_Buffer3[3] = {7, 8, 9};
uint8_t ZH_Buffer4[3] = {10, 11, 12};
uint8_t ZH_Buffer5[3] = {13, 14, 15};
uint8_t ZH_Buffer6[3] = {16, 17, 18};

uint8_t ZH_Buffer7[3] = {19, 20, 21};
uint8_t ZH_Buffer8[3] = {22, 23, 24};
uint8_t ZH_Buffer9[3] = {25, 26, 27};
uint8_t ZH_Buffer10[3] = {28, 29, 30};


 uint32_t rt_longitude;
 uint32_t rt_latitude;

void showgps()
{
    LCD_ShowIntNum(0, 0, rt_longitude, BLUE, WHITE, 16, 0);
    LCD_ShowIntNum(100,0 , rt_latitude, BLUE, WHITE, 16, 0);
}

/******第0页******/
void my_pageA0(void)
{

    lcd_show_chinese(40, 0, "智能客车助手", BLUE, WHITE, 32, 0);
    lcd_show_chinese(40, 96, "自动报站", BLUE, YELLOW, 32, 0);
    lcd_show_chinese(40, 128, "手动报站", BLUE, WHITE, 32, 0);

}

void my_pageA1(void)
{

    lcd_show_chinese(40, 0, "智能客车助手", BLUE, WHITE, 32, 0);
    lcd_show_chinese(40, 96, "自动报站", BLUE, WHITE, 32, 0);
    lcd_show_chinese(40, 128, "手动报站", BLUE, YELLOW, 32, 0);
}

/******第1页******/
void my_pageB2(void)
{

    lcd_show_chinese(40, 0, "自动方向选择", BLUE, WHITE, 32, 0);
    lcd_show_chinese(40, 96, "正向", BLUE, YELLOW, 32, 0);
    lcd_show_chinese(40, 128, "反向", BLUE, WHITE, 32, 0);

    
}

void my_pageB3(void)
{

    lcd_show_chinese(40, 0, "自动方向选择", BLUE, WHITE, 32, 0);
    lcd_show_chinese(40, 96, "正向", BLUE, WHITE, 32, 0);
    lcd_show_chinese(40, 128, "反向", BLUE, YELLOW, 32, 0);
}


void my_pageB5()//flag1
{
    LCD_ShowChinese(50, 50, "到达站点", BLUE, WHITE, 32, 0);
    char str2[] = "下一站是";
    LCD_ShowChinese(0, 100, str2, BLUE, WHITE, 32, 0);
    LCD_ShowIntNum(128, 100, next_station,1, BLUE, WHITE, 32);
    char str1[] = "站";
    LCD_ShowChinese(160, 100, str1, BLUE, WHITE, 32, 0);
    // JQ8x00_Command_Data(SetVolume, 35); // 设置音量
    // delay_ms(10);
    // 语音播报01 02 03
    // JQ8x00_ZuHeBoFang(ZH_Buffer1, 3); // 将ZH文件夹下名字为01 02 03的3个音频文件依次播放
}

void my_pageB6()//flag0
{
    LCD_ShowChinese(50, 50, "行驶中", BLUE, WHITE, 32, 0);
    char str2[] = "下一站是";
    LCD_ShowChinese(0, 100, str2, BLUE, WHITE, 32, 0);
    LCD_ShowIntNum(128, 100, next_station, 1, BLUE, WHITE, 32);
    char str1[] = "站";
    LCD_ShowChinese(160, 100, str1, BLUE, WHITE, 32, 0);
}


    /******第2页(LCD显示的/公交站点)******/

u8 arrive_flag; // 0:行驶 1：停止 
// 自动报站（正向）
void bus_proc(void)
{
        /* GPS获得的当前位置（整数形式）在海心沙站所在范围内，且站点标志位为1 */
        if (station_ranges[next_station - 1][0] < rt_longitude && rt_longitude < station_ranges[next_station - 1][1] &&
            station_ranges[next_station - 1][2] < rt_latitude && rt_latitude < station_ranges[next_station - 1][3] && arrive_flag == 0)
        {
            arrive_flag = 1;
            gps_arrive();
            // 海心沙站绿色
        }
        else if (!(station_ranges[next_station - 1][0] < rt_longitude && rt_longitude < station_ranges[next_station - 1][1] && station_ranges[next_station - 1][2] < rt_latitude && rt_latitude < station_ranges[next_station - 1][3]) && arrive_flag == 1)
        {
            arrive_flag = 0;
        }
}

// // 自动报站（猎德到海心沙方向）
// void my_pageC9(void)
// {
//     int32_t stop_flag = 4; // 定义一个站点标志位，防止公交重复报站

//     Lcd_Clear(GRAY0);
//     LCD_LED_SET; // 通过IO控制背光亮
//     Gui_DrawFont_GBK16(40, 30, BLACK, GRAY0, "猎德大桥");
//     Gui_DrawFont_GBK16(40, 60, BLACK, GRAY0, "广州塔");
//     Gui_DrawFont_GBK16(40, 90, BLACK, GRAY0, "海心桥");
//     Gui_DrawFont_GBK16(40, 120, BLACK, GRAY0, "海心沙");

//     while (stop_flag)
//     {
//         RT_JWD_Get(); // 获取实时时间和经纬度

//         /* GPS获得的当前位置（整数形式）在猎德大桥站所在范围内，且站点标志位为4 */
//         if (stop4_longitude_MIN < rt_longitude && rt_longitude < stop4_longitude_MAX &&
//             stop4_latitude_MIN < rt_latitude && rt_latitude < stop4_latitude_MAX && stop_flag == 4)
//         {
//             gps_arrive();
//             stop_flag = -4; // 站点标志位更换为-4，准备离开猎德大桥站的报站
//             // 猎德大桥站绿色
//             Gui_DrawFont_GBK16(40, 30, BLACK, GREEN, "猎德大桥");

//             JQ8x00_Command_Data(SetVolume, 35); // 设置音量
//             delay_ms(10);
//             // 语音播报19 20 21
//             JQ8x00_ZuHeBoFang(ZH_Buffer7, 3); // 将ZH文件夹下名字为19 20 21的3个音频文件依次播放
//         }

//         /* GPS获得的当前位置（整数形式）离开猎德大桥站所在范围内，且站点标志位为-4 */
//         if (!(stop4_longitude_MIN < rt_longitude && rt_longitude < stop4_longitude_MAX &&
//               stop4_latitude_MIN < rt_latitude && rt_latitude < stop4_latitude_MAX) &&
//             stop_flag == -4)
//         {
//             stop_flag = 3; // 站点标志位更换为3，准备广州塔站的报站
//             // 猎德大桥站恢复原来的颜色
//             Gui_DrawFont_GBK16(40, 30, BLACK, GRAY0, "猎德大桥");
//         }

//         /* GPS获得的当前位置（整数形式）在广州塔站所在范围内，且站点标志位为3 */
//         if (stop3_longitude_MIN < rt_longitude && rt_longitude < stop3_longitude_MAX &&
//             stop3_latitude_MIN < rt_latitude && rt_latitude < stop3_latitude_MAX && stop_flag == 3)
//         {
//             stop_flag = -3; // 站点标志位更换为-3，准备离开广州塔站的报站
//             // 广州塔站绿色
//             Gui_DrawFont_GBK16(40, 60, BLACK, GREEN, "广州塔");

//             JQ8x00_Command_Data(SetVolume, 35); // 设置音量
//             delay_ms(10);
//             // 语音播报10 11 12
//             JQ8x00_ZuHeBoFang(ZH_Buffer4, 3); // 将ZH文件夹下名字为10 11 12的3个音频文件依次播放

//             // 10 11 12语音播报共花8秒，防止还没播报完就被离开报站的语音抢报
//             delay_ms(8000);
//         }

//         /* GPS获得的当前位置（整数形式）离开广州塔站所在范围内，且站点标志位为-3 */
//         if (!(stop3_longitude_MIN < rt_longitude && rt_longitude < stop3_longitude_MAX &&
//               stop3_latitude_MIN < rt_latitude && rt_latitude < stop3_latitude_MAX) &&
//             stop_flag == -3)
//         {
//             stop_flag = 2; // 站点标志位更换为2，准备海心桥站的报站
//             // 广州塔站恢复原来的颜色
//             Gui_DrawFont_GBK16(40, 60, BLACK, GRAY0, "广州塔");
//             JQ8x00_Command_Data(SetVolume, 35); // 设置音量
//             delay_ms(10);
//             // 语音播报22 23 24
//             JQ8x00_ZuHeBoFang(ZH_Buffer8, 3); // 将ZH文件夹下名字为22 23 24的3个音频文件依次播放
//         }

//         /* GPS获得的当前位置（整数形式）在海心桥站所在范围内，且站点标志位为2 */
//         if (stop2_longitude_MIN < rt_longitude && rt_longitude < stop2_longitude_MAX &&
//             stop2_latitude_MIN < rt_latitude && rt_latitude < stop2_latitude_MAX && stop_flag == 2)
//         {
//             stop_flag = -2; // 站点标志位更换为-2，准备离开海心桥站的报站
//             // 海心桥站绿色
//             Gui_DrawFont_GBK16(40, 90, BLACK, GREEN, "海心桥");

//             JQ8x00_Command_Data(SetVolume, 35); // 设置音量
//             delay_ms(10);
//             // 语音播报04 05 06
//             JQ8x00_ZuHeBoFang(ZH_Buffer2, 3); // 将ZH文件夹下名字为04 05 06的3个音频文件依次播放

//             // 04 05 06语音播报共花8秒，防止还没播报完就被离开报站的语音抢报
//             delay_ms(8000);
//         }

//         /* GPS获得的当前位置（整数形式）离开海心桥站所在范围内，且站点标志位为-2 */
//         if (!(stop2_longitude_MIN < rt_longitude && rt_longitude < stop2_longitude_MAX &&
//               stop2_latitude_MIN < rt_latitude && rt_latitude < stop2_latitude_MAX) &&
//             stop_flag == -2)
//         {
//             stop_flag = 1; // 站点标志位更换为1，准备海心沙的报站
//             // 海心桥站恢复原来的颜色
//             Gui_DrawFont_GBK16(40, 90, BLACK, GRAY0, "海心桥");

//             JQ8x00_Command_Data(SetVolume, 35); // 设置音量
//             delay_ms(10);
//             // 语音播报25 26 27
//             JQ8x00_ZuHeBoFang(ZH_Buffer9, 3); // 将ZH文件夹下名字为25 26 27的3个音频文件依次播放
//         }

//         /* GPS获得的当前位置（整数形式）在海心沙站所在范围内，且站点标志位为1 */
//         if (stop1_longitude_MIN < rt_longitude && rt_longitude < stop1_longitude_MAX &&
//             stop1_latitude_MIN < rt_latitude && rt_latitude < stop1_latitude_MAX && stop_flag == 1)
//         {
//             stop_flag = 0; // 站点标志位更换为0，代表已到达终点站，公交不再报站
//             // 海心沙站绿色
//             Gui_DrawFont_GBK16(40, 120, BLACK, GREEN, "海心沙");
//             JQ8x00_Command_Data(SetVolume, 35); // 设置音量
//             delay_ms(10);
//             // 语音播报28 29 30
//             JQ8x00_ZuHeBoFang(ZH_Buffer10, 3); // 将ZH文件夹下名字为28 29 30的3个音频文件依次播放
//         }
//     }
// }

// // 手动报站界面（海心沙到猎德方向）
// void my_pageC10(void)
// {
//     Gui_DrawFont_GBK16(40, 30, BLACK, GREEN, "海心沙");

//     JQ8x00_Command_Data(SetVolume, 35); // 设置音量
//     delay_ms(10);
//     JQ8x00_ZuHeBoFang(ZH_Buffer1, 3); // 将ZH文件夹下名字为01 02 03的3个音频文件依次播放
// }

// void my_pageC11(void)
// {
//     Gui_DrawFont_GBK16(40, 30, BLACK, GRAY0, "海心沙");
//     Gui_DrawFont_GBK16(40, 60, BLACK, GREEN, "海心桥");

//     JQ8x00_Command_Data(SetVolume, 35); // 设置音量
//     delay_ms(10);
//     JQ8x00_ZuHeBoFang(ZH_Buffer2, 3); // 将ZH文件夹下名字为04 05 06的3个音频文件依次播放
//     delay_ms(bus_wait_time + 8000);   // 停车bus_wait_time毫秒等待乘客上车，其中04 05 06语音播报共花8秒
//     JQ8x00_ZuHeBoFang(ZH_Buffer3, 3); // 将ZH文件夹下名字为07 08 09的3个音频文件依次播放
// }

// void my_pageC12(void)
// {
//     Gui_DrawFont_GBK16(40, 60, BLACK, GRAY0, "海心桥");
//     Gui_DrawFont_GBK16(40, 90, BLACK, GREEN, "广州塔");

//     JQ8x00_Command_Data(SetVolume, 35); // 设置音量
//     delay_ms(10);
//     JQ8x00_ZuHeBoFang(ZH_Buffer4, 3); // 将ZH文件夹下名字为10 11 12的3个音频文件依次播放
//     delay_ms(bus_wait_time + 8000);   // 停车bus_wait_time毫秒等待乘客上车，其中10 11 12语音播报共花8秒
//     JQ8x00_ZuHeBoFang(ZH_Buffer5, 3); // 将ZH文件夹下名字为13 14 15的3个音频文件依次播放
// }

// void my_pageC13(void)
// {
//     Gui_DrawFont_GBK16(40, 90, BLACK, GRAY0, "广州塔");
//     Gui_DrawFont_GBK16(40, 120, BLACK, GREEN, "猎德大桥");

//     JQ8x00_Command_Data(SetVolume, 35); // 设置音量
//     delay_ms(10);
//     JQ8x00_ZuHeBoFang(ZH_Buffer6, 3); // 将ZH文件夹下名字为16 17 18的3个音频文件依次播放
// }

// // 手动报站界面（猎德到海心沙方向）
// void my_pageC14(void)
// {
//     Gui_DrawFont_GBK16(40, 30, BLACK, GREEN, "猎德大桥");

//     JQ8x00_Command_Data(SetVolume, 35); // 设置音量
//     delay_ms(10);
//     JQ8x00_ZuHeBoFang(ZH_Buffer7, 3); // 将ZH文件夹下名字为19 20 21的3个音频文件依次播放
// }

// void my_pageC15(void)
// {
//     Gui_DrawFont_GBK16(40, 30, BLACK, GRAY0, "猎德大桥");
//     Gui_DrawFont_GBK16(40, 60, BLACK, GREEN, "广州塔");

//     JQ8x00_Command_Data(SetVolume, 35); // 设置音量
//     delay_ms(10);
//     JQ8x00_ZuHeBoFang(ZH_Buffer4, 3); // 将ZH文件夹下名字为10 11 12的3个音频文件依次播放
//     delay_ms(bus_wait_time + 8000);   // 停车bus_wait_time毫秒等待乘客上车，其中10 11 12语音播报共花8秒
//     JQ8x00_ZuHeBoFang(ZH_Buffer8, 3); // 将ZH文件夹下名字为22 23 24的3个音频文件依次播放
// }

// void my_pageC16(void)
// {
//     Gui_DrawFont_GBK16(40, 60, BLACK, GRAY0, "广州塔");
//     Gui_DrawFont_GBK16(40, 90, BLACK, GREEN, "海心桥");

//     JQ8x00_Command_Data(SetVolume, 35); // 设置音量
//     delay_ms(10);
//     JQ8x00_ZuHeBoFang(ZH_Buffer2, 3); // 将ZH文件夹下名字为04 05 06的3个音频文件依次播放
//     delay_ms(bus_wait_time + 8000);   // 停车bus_wait_time毫秒等待乘客上车，其中04 05 06语音播报共花8秒
//     JQ8x00_ZuHeBoFang(ZH_Buffer9, 3); // 将ZH文件夹下名字为25 26 27的3个音频文件依次播放
// }

// void my_pageC17(void)
// {
//     Gui_DrawFont_GBK16(40, 90, BLACK, GRAY0, "海心桥");
//     Gui_DrawFont_GBK16(40, 120, BLACK, GREEN, "海心沙");

//     JQ8x00_Command_Data(SetVolume, 35); // 设置音量
//     delay_ms(10);
//     JQ8x00_ZuHeBoFang(ZH_Buffer10, 3); // 将ZH文件夹下名字为28 29 30的3个音频文件依次播放
// }

// // 手动报站初始界面（海心沙到猎德方向）
// void my_pageC18(void)
// {
//     Lcd_Clear(GRAY0);
//     LCD_LED_SET; // 通过IO控制背光亮
//     Gui_DrawFont_GBK16(40, 30, BLACK, GRAY0, "海心沙");
//     Gui_DrawFont_GBK16(40, 60, BLACK, GRAY0, "海心桥");
//     Gui_DrawFont_GBK16(40, 90, BLACK, GRAY0, "广州塔");
//     Gui_DrawFont_GBK16(40, 120, BLACK, GRAY0, "猎德大桥");
// }

// // 手动报站初始界面（猎德到海心沙方向）
// void my_pageC19(void)
// {
//     Lcd_Clear(GRAY0);
//     LCD_LED_SET; // 通过IO控制背光亮
//     Gui_DrawFont_GBK16(40, 30, BLACK, GRAY0, "猎德大桥");
//     Gui_DrawFont_GBK16(40, 60, BLACK, GRAY0, "广州塔");
//     Gui_DrawFont_GBK16(40, 90, BLACK, GRAY0, "海心桥");
//     Gui_DrawFont_GBK16(40, 120, BLACK, GRAY0, "海心沙");
// }

// /* 当GPS中途坏掉的时候，就要把站点切到坏掉的位置再手动报站，以下是会用到的界面 */
// // 海心沙到猎德方向
// void my_pageC20(void)
// {
//     Gui_DrawFont_GBK16(40, 30, BLACK, GREEN, "海心沙");
//     Gui_DrawFont_GBK16(40, 60, BLACK, GRAY0, "海心桥");
//     Gui_DrawFont_GBK16(40, 120, BLACK, GRAY0, "猎德大桥");
// }

// void my_pageC21(void)
// {
//     Gui_DrawFont_GBK16(40, 30, BLACK, GRAY0, "海心沙");
//     Gui_DrawFont_GBK16(40, 60, BLACK, GREEN, "海心桥");
//     Gui_DrawFont_GBK16(40, 90, BLACK, GRAY0, "广州塔");
// }

// void my_pageC22(void)
// {
//     Gui_DrawFont_GBK16(40, 60, BLACK, GRAY0, "海心桥");
//     Gui_DrawFont_GBK16(40, 90, BLACK, GREEN, "广州塔");
//     Gui_DrawFont_GBK16(40, 120, BLACK, GRAY0, "猎德大桥");
// }

// void my_pageC23(void)
// {
//     Gui_DrawFont_GBK16(40, 30, BLACK, GRAY0, "海心沙");
//     Gui_DrawFont_GBK16(40, 90, BLACK, GRAY0, "广州塔");
//     Gui_DrawFont_GBK16(40, 120, BLACK, GREEN, "猎德大桥");
// }

// // 猎德到海心沙方向
// void my_pageC24(void)
// {
//     Gui_DrawFont_GBK16(40, 30, BLACK, GREEN, "猎德大桥");
//     Gui_DrawFont_GBK16(40, 60, BLACK, GRAY0, "广州塔");
//     Gui_DrawFont_GBK16(40, 120, BLACK, GRAY0, "海心沙");
// }

// void my_pageC25(void)
// {
//     Gui_DrawFont_GBK16(40, 30, BLACK, GRAY0, "猎德大桥");
//     Gui_DrawFont_GBK16(40, 60, BLACK, GREEN, "广州塔");
//     Gui_DrawFont_GBK16(40, 90, BLACK, GRAY0, "海心桥");
// }

// void my_pageC26(void)
// {
//     Gui_DrawFont_GBK16(40, 60, BLACK, GRAY0, "广州塔");
//     Gui_DrawFont_GBK16(40, 90, BLACK, GREEN, "海心桥");
//     Gui_DrawFont_GBK16(40, 120, BLACK, GRAY0, "海心沙");
// }

// void my_pageC27(void)
// {
//     Gui_DrawFont_GBK16(40, 30, BLACK, GRAY0, "猎德大桥");
//     Gui_DrawFont_GBK16(40, 90, BLACK, GRAY0, "海心桥");
//     Gui_DrawFont_GBK16(40, 120, BLACK, GREEN, "海心沙");
// }
