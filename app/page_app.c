/**************************************************************************************************
�༶�˵�����
**************************************************************************************************/
#include <bsp_system.h>

#define bus_wait_time 5000 // ���ù�������վ̨�ȴ�ʱ��(ms)



/*
1���궨��ÿ��վ�㾭γ�ȷ�Χ��������ʽ������Щֵ�ھ���վ���GPS��γ����ʾʵ���õ�
2������ͬһγ��ʱ�����Ȳ�ľ��빫ʽΪ��d=cos(γ��) * ���Ȳ� * 113190��
   ���������õľ���������ʽ��ΧΪ20�������Ȳ�0.0002��������Ԥ��վ���γ�ȷ�ΧΪ21������
3��γ�Ȳ�ľ��빫ʽΪ��d=γ�Ȳ� * 113190��
   ���������õľ���������ʽ��ΧΪ20�������Ȳ�0.0002��������Ԥ��վ���γ�ȷ�ΧΪ22.6������

��������վ�㾭γ���������õĲ�ֵΪ20��������������վ��10m�ĵط��ͻᱨվ
*/
// վ��1��õľ�γ�ȴ��Ϊ119.54505��E 39.92343��N

// �����ά���飬�洢3��վ��ľ�γ�ȷ�Χֵ��ÿ��վ�����4��32λ������
const int32_t station_ranges[3][4] = {
    // վ��1��longitude_MIN, longitude_MAX, latitude_MIN, latitude_MAX
    {11954495, 11954515, 3992333, 3992353},

    // վ��2����Ӧ�����궨���ֵ
    {11954474, 11954494, 3992358, 3992378},

    // վ��3����Ӧ�����궨���ֵ
    {11954470, 11954490, 3992223, 3992243}
};
//��ѧ�� 1
#define stop1_longitude_MIN 11954495
#define stop1_longitude_MAX 11954515
#define stop1_latitude_MIN 3992333
#define stop1_latitude_MAX 3992353

//��Զ  վ��2��õľ�γ�ȴ��Ϊ119.54484��E 39.92368��N
#define stop2_longitude_MIN 11954474
#define stop2_longitude_MAX 11954494
#define stop2_latitude_MIN 3992358
#define stop2_latitude_MAX 3992378

//���� վ��3��õľ�γ�ȴ��Ϊ119.54480��E 39.92233��N
#define stop3_longitude_MIN 11954470
#define stop3_longitude_MAX 11954490
#define stop3_latitude_MIN 3992223
#define stop3_latitude_MAX 3992243

// վ��4��õľ�γ�ȴ��Ϊ113.38990E 23.11130N
#define stop4_longitude_MIN 11338980
#define stop4_longitude_MAX 11339000
#define stop4_latitude_MIN 2311120
#define stop4_latitude_MAX 2311140

/*�����ļ�����*/
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

/******��0ҳ******/
void my_pageA0(void)
{

    lcd_show_chinese(40, 0, "���ܿͳ�����", BLUE, WHITE, 32, 0);
    lcd_show_chinese(40, 96, "�Զ���վ", BLUE, YELLOW, 32, 0);
    lcd_show_chinese(40, 128, "�ֶ���վ", BLUE, WHITE, 32, 0);

}

void my_pageA1(void)
{

    lcd_show_chinese(40, 0, "���ܿͳ�����", BLUE, WHITE, 32, 0);
    lcd_show_chinese(40, 96, "�Զ���վ", BLUE, WHITE, 32, 0);
    lcd_show_chinese(40, 128, "�ֶ���վ", BLUE, YELLOW, 32, 0);
}

/******��1ҳ******/
void my_pageB2(void)
{

    lcd_show_chinese(40, 0, "�Զ�����ѡ��", BLUE, WHITE, 32, 0);
    lcd_show_chinese(40, 96, "����", BLUE, YELLOW, 32, 0);
    lcd_show_chinese(40, 128, "����", BLUE, WHITE, 32, 0);

    
}

void my_pageB3(void)
{

    lcd_show_chinese(40, 0, "�Զ�����ѡ��", BLUE, WHITE, 32, 0);
    lcd_show_chinese(40, 96, "����", BLUE, WHITE, 32, 0);
    lcd_show_chinese(40, 128, "����", BLUE, YELLOW, 32, 0);
}


void my_pageB5()//flag1
{
    LCD_ShowChinese(50, 50, "����վ��", BLUE, WHITE, 32, 0);
    char str2[] = "��һվ��";
    LCD_ShowChinese(0, 100, str2, BLUE, WHITE, 32, 0);
    LCD_ShowIntNum(128, 100, next_station,1, BLUE, WHITE, 32);
    char str1[] = "վ";
    LCD_ShowChinese(160, 100, str1, BLUE, WHITE, 32, 0);
    // JQ8x00_Command_Data(SetVolume, 35); // ��������
    // delay_ms(10);
    // ��������01 02 03
    // JQ8x00_ZuHeBoFang(ZH_Buffer1, 3); // ��ZH�ļ���������Ϊ01 02 03��3����Ƶ�ļ����β���
}

void my_pageB6()//flag0
{
    LCD_ShowChinese(50, 50, "��ʻ��", BLUE, WHITE, 32, 0);
    char str2[] = "��һվ��";
    LCD_ShowChinese(0, 100, str2, BLUE, WHITE, 32, 0);
    LCD_ShowIntNum(128, 100, next_station, 1, BLUE, WHITE, 32);
    char str1[] = "վ";
    LCD_ShowChinese(160, 100, str1, BLUE, WHITE, 32, 0);
}


    /******��2ҳ(LCD��ʾ��/����վ��)******/

u8 arrive_flag; // 0:��ʻ 1��ֹͣ 
// �Զ���վ������
void bus_proc(void)
{
        /* GPS��õĵ�ǰλ�ã�������ʽ���ں���ɳվ���ڷ�Χ�ڣ���վ���־λΪ1 */
        if (station_ranges[next_station - 1][0] < rt_longitude && rt_longitude < station_ranges[next_station - 1][1] &&
            station_ranges[next_station - 1][2] < rt_latitude && rt_latitude < station_ranges[next_station - 1][3] && arrive_flag == 0)
        {
            arrive_flag = 1;
            gps_arrive();
            // ����ɳվ��ɫ
        }
        else if (!(station_ranges[next_station - 1][0] < rt_longitude && rt_longitude < station_ranges[next_station - 1][1] && station_ranges[next_station - 1][2] < rt_latitude && rt_latitude < station_ranges[next_station - 1][3]) && arrive_flag == 1)
        {
            arrive_flag = 0;
        }
}

// // �Զ���վ���Եµ�����ɳ����
// void my_pageC9(void)
// {
//     int32_t stop_flag = 4; // ����һ��վ���־λ����ֹ�����ظ���վ

//     Lcd_Clear(GRAY0);
//     LCD_LED_SET; // ͨ��IO���Ʊ�����
//     Gui_DrawFont_GBK16(40, 30, BLACK, GRAY0, "�Ե´���");
//     Gui_DrawFont_GBK16(40, 60, BLACK, GRAY0, "������");
//     Gui_DrawFont_GBK16(40, 90, BLACK, GRAY0, "������");
//     Gui_DrawFont_GBK16(40, 120, BLACK, GRAY0, "����ɳ");

//     while (stop_flag)
//     {
//         RT_JWD_Get(); // ��ȡʵʱʱ��;�γ��

//         /* GPS��õĵ�ǰλ�ã�������ʽ�����Ե´���վ���ڷ�Χ�ڣ���վ���־λΪ4 */
//         if (stop4_longitude_MIN < rt_longitude && rt_longitude < stop4_longitude_MAX &&
//             stop4_latitude_MIN < rt_latitude && rt_latitude < stop4_latitude_MAX && stop_flag == 4)
//         {
//             gps_arrive();
//             stop_flag = -4; // վ���־λ����Ϊ-4��׼���뿪�Ե´���վ�ı�վ
//             // �Ե´���վ��ɫ
//             Gui_DrawFont_GBK16(40, 30, BLACK, GREEN, "�Ե´���");

//             JQ8x00_Command_Data(SetVolume, 35); // ��������
//             delay_ms(10);
//             // ��������19 20 21
//             JQ8x00_ZuHeBoFang(ZH_Buffer7, 3); // ��ZH�ļ���������Ϊ19 20 21��3����Ƶ�ļ����β���
//         }

//         /* GPS��õĵ�ǰλ�ã�������ʽ���뿪�Ե´���վ���ڷ�Χ�ڣ���վ���־λΪ-4 */
//         if (!(stop4_longitude_MIN < rt_longitude && rt_longitude < stop4_longitude_MAX &&
//               stop4_latitude_MIN < rt_latitude && rt_latitude < stop4_latitude_MAX) &&
//             stop_flag == -4)
//         {
//             stop_flag = 3; // վ���־λ����Ϊ3��׼��������վ�ı�վ
//             // �Ե´���վ�ָ�ԭ������ɫ
//             Gui_DrawFont_GBK16(40, 30, BLACK, GRAY0, "�Ե´���");
//         }

//         /* GPS��õĵ�ǰλ�ã�������ʽ���ڹ�����վ���ڷ�Χ�ڣ���վ���־λΪ3 */
//         if (stop3_longitude_MIN < rt_longitude && rt_longitude < stop3_longitude_MAX &&
//             stop3_latitude_MIN < rt_latitude && rt_latitude < stop3_latitude_MAX && stop_flag == 3)
//         {
//             stop_flag = -3; // վ���־λ����Ϊ-3��׼���뿪������վ�ı�վ
//             // ������վ��ɫ
//             Gui_DrawFont_GBK16(40, 60, BLACK, GREEN, "������");

//             JQ8x00_Command_Data(SetVolume, 35); // ��������
//             delay_ms(10);
//             // ��������10 11 12
//             JQ8x00_ZuHeBoFang(ZH_Buffer4, 3); // ��ZH�ļ���������Ϊ10 11 12��3����Ƶ�ļ����β���

//             // 10 11 12������������8�룬��ֹ��û������ͱ��뿪��վ����������
//             delay_ms(8000);
//         }

//         /* GPS��õĵ�ǰλ�ã�������ʽ���뿪������վ���ڷ�Χ�ڣ���վ���־λΪ-3 */
//         if (!(stop3_longitude_MIN < rt_longitude && rt_longitude < stop3_longitude_MAX &&
//               stop3_latitude_MIN < rt_latitude && rt_latitude < stop3_latitude_MAX) &&
//             stop_flag == -3)
//         {
//             stop_flag = 2; // վ���־λ����Ϊ2��׼��������վ�ı�վ
//             // ������վ�ָ�ԭ������ɫ
//             Gui_DrawFont_GBK16(40, 60, BLACK, GRAY0, "������");
//             JQ8x00_Command_Data(SetVolume, 35); // ��������
//             delay_ms(10);
//             // ��������22 23 24
//             JQ8x00_ZuHeBoFang(ZH_Buffer8, 3); // ��ZH�ļ���������Ϊ22 23 24��3����Ƶ�ļ����β���
//         }

//         /* GPS��õĵ�ǰλ�ã�������ʽ���ں�����վ���ڷ�Χ�ڣ���վ���־λΪ2 */
//         if (stop2_longitude_MIN < rt_longitude && rt_longitude < stop2_longitude_MAX &&
//             stop2_latitude_MIN < rt_latitude && rt_latitude < stop2_latitude_MAX && stop_flag == 2)
//         {
//             stop_flag = -2; // վ���־λ����Ϊ-2��׼���뿪������վ�ı�վ
//             // ������վ��ɫ
//             Gui_DrawFont_GBK16(40, 90, BLACK, GREEN, "������");

//             JQ8x00_Command_Data(SetVolume, 35); // ��������
//             delay_ms(10);
//             // ��������04 05 06
//             JQ8x00_ZuHeBoFang(ZH_Buffer2, 3); // ��ZH�ļ���������Ϊ04 05 06��3����Ƶ�ļ����β���

//             // 04 05 06������������8�룬��ֹ��û������ͱ��뿪��վ����������
//             delay_ms(8000);
//         }

//         /* GPS��õĵ�ǰλ�ã�������ʽ���뿪������վ���ڷ�Χ�ڣ���վ���־λΪ-2 */
//         if (!(stop2_longitude_MIN < rt_longitude && rt_longitude < stop2_longitude_MAX &&
//               stop2_latitude_MIN < rt_latitude && rt_latitude < stop2_latitude_MAX) &&
//             stop_flag == -2)
//         {
//             stop_flag = 1; // վ���־λ����Ϊ1��׼������ɳ�ı�վ
//             // ������վ�ָ�ԭ������ɫ
//             Gui_DrawFont_GBK16(40, 90, BLACK, GRAY0, "������");

//             JQ8x00_Command_Data(SetVolume, 35); // ��������
//             delay_ms(10);
//             // ��������25 26 27
//             JQ8x00_ZuHeBoFang(ZH_Buffer9, 3); // ��ZH�ļ���������Ϊ25 26 27��3����Ƶ�ļ����β���
//         }

//         /* GPS��õĵ�ǰλ�ã�������ʽ���ں���ɳվ���ڷ�Χ�ڣ���վ���־λΪ1 */
//         if (stop1_longitude_MIN < rt_longitude && rt_longitude < stop1_longitude_MAX &&
//             stop1_latitude_MIN < rt_latitude && rt_latitude < stop1_latitude_MAX && stop_flag == 1)
//         {
//             stop_flag = 0; // վ���־λ����Ϊ0�������ѵ����յ�վ���������ٱ�վ
//             // ����ɳվ��ɫ
//             Gui_DrawFont_GBK16(40, 120, BLACK, GREEN, "����ɳ");
//             JQ8x00_Command_Data(SetVolume, 35); // ��������
//             delay_ms(10);
//             // ��������28 29 30
//             JQ8x00_ZuHeBoFang(ZH_Buffer10, 3); // ��ZH�ļ���������Ϊ28 29 30��3����Ƶ�ļ����β���
//         }
//     }
// }

// // �ֶ���վ���棨����ɳ���Ե·���
// void my_pageC10(void)
// {
//     Gui_DrawFont_GBK16(40, 30, BLACK, GREEN, "����ɳ");

//     JQ8x00_Command_Data(SetVolume, 35); // ��������
//     delay_ms(10);
//     JQ8x00_ZuHeBoFang(ZH_Buffer1, 3); // ��ZH�ļ���������Ϊ01 02 03��3����Ƶ�ļ����β���
// }

// void my_pageC11(void)
// {
//     Gui_DrawFont_GBK16(40, 30, BLACK, GRAY0, "����ɳ");
//     Gui_DrawFont_GBK16(40, 60, BLACK, GREEN, "������");

//     JQ8x00_Command_Data(SetVolume, 35); // ��������
//     delay_ms(10);
//     JQ8x00_ZuHeBoFang(ZH_Buffer2, 3); // ��ZH�ļ���������Ϊ04 05 06��3����Ƶ�ļ����β���
//     delay_ms(bus_wait_time + 8000);   // ͣ��bus_wait_time����ȴ��˿��ϳ�������04 05 06������������8��
//     JQ8x00_ZuHeBoFang(ZH_Buffer3, 3); // ��ZH�ļ���������Ϊ07 08 09��3����Ƶ�ļ����β���
// }

// void my_pageC12(void)
// {
//     Gui_DrawFont_GBK16(40, 60, BLACK, GRAY0, "������");
//     Gui_DrawFont_GBK16(40, 90, BLACK, GREEN, "������");

//     JQ8x00_Command_Data(SetVolume, 35); // ��������
//     delay_ms(10);
//     JQ8x00_ZuHeBoFang(ZH_Buffer4, 3); // ��ZH�ļ���������Ϊ10 11 12��3����Ƶ�ļ����β���
//     delay_ms(bus_wait_time + 8000);   // ͣ��bus_wait_time����ȴ��˿��ϳ�������10 11 12������������8��
//     JQ8x00_ZuHeBoFang(ZH_Buffer5, 3); // ��ZH�ļ���������Ϊ13 14 15��3����Ƶ�ļ����β���
// }

// void my_pageC13(void)
// {
//     Gui_DrawFont_GBK16(40, 90, BLACK, GRAY0, "������");
//     Gui_DrawFont_GBK16(40, 120, BLACK, GREEN, "�Ե´���");

//     JQ8x00_Command_Data(SetVolume, 35); // ��������
//     delay_ms(10);
//     JQ8x00_ZuHeBoFang(ZH_Buffer6, 3); // ��ZH�ļ���������Ϊ16 17 18��3����Ƶ�ļ����β���
// }

// // �ֶ���վ���棨�Եµ�����ɳ����
// void my_pageC14(void)
// {
//     Gui_DrawFont_GBK16(40, 30, BLACK, GREEN, "�Ե´���");

//     JQ8x00_Command_Data(SetVolume, 35); // ��������
//     delay_ms(10);
//     JQ8x00_ZuHeBoFang(ZH_Buffer7, 3); // ��ZH�ļ���������Ϊ19 20 21��3����Ƶ�ļ����β���
// }

// void my_pageC15(void)
// {
//     Gui_DrawFont_GBK16(40, 30, BLACK, GRAY0, "�Ե´���");
//     Gui_DrawFont_GBK16(40, 60, BLACK, GREEN, "������");

//     JQ8x00_Command_Data(SetVolume, 35); // ��������
//     delay_ms(10);
//     JQ8x00_ZuHeBoFang(ZH_Buffer4, 3); // ��ZH�ļ���������Ϊ10 11 12��3����Ƶ�ļ����β���
//     delay_ms(bus_wait_time + 8000);   // ͣ��bus_wait_time����ȴ��˿��ϳ�������10 11 12������������8��
//     JQ8x00_ZuHeBoFang(ZH_Buffer8, 3); // ��ZH�ļ���������Ϊ22 23 24��3����Ƶ�ļ����β���
// }

// void my_pageC16(void)
// {
//     Gui_DrawFont_GBK16(40, 60, BLACK, GRAY0, "������");
//     Gui_DrawFont_GBK16(40, 90, BLACK, GREEN, "������");

//     JQ8x00_Command_Data(SetVolume, 35); // ��������
//     delay_ms(10);
//     JQ8x00_ZuHeBoFang(ZH_Buffer2, 3); // ��ZH�ļ���������Ϊ04 05 06��3����Ƶ�ļ����β���
//     delay_ms(bus_wait_time + 8000);   // ͣ��bus_wait_time����ȴ��˿��ϳ�������04 05 06������������8��
//     JQ8x00_ZuHeBoFang(ZH_Buffer9, 3); // ��ZH�ļ���������Ϊ25 26 27��3����Ƶ�ļ����β���
// }

// void my_pageC17(void)
// {
//     Gui_DrawFont_GBK16(40, 90, BLACK, GRAY0, "������");
//     Gui_DrawFont_GBK16(40, 120, BLACK, GREEN, "����ɳ");

//     JQ8x00_Command_Data(SetVolume, 35); // ��������
//     delay_ms(10);
//     JQ8x00_ZuHeBoFang(ZH_Buffer10, 3); // ��ZH�ļ���������Ϊ28 29 30��3����Ƶ�ļ����β���
// }

// // �ֶ���վ��ʼ���棨����ɳ���Ե·���
// void my_pageC18(void)
// {
//     Lcd_Clear(GRAY0);
//     LCD_LED_SET; // ͨ��IO���Ʊ�����
//     Gui_DrawFont_GBK16(40, 30, BLACK, GRAY0, "����ɳ");
//     Gui_DrawFont_GBK16(40, 60, BLACK, GRAY0, "������");
//     Gui_DrawFont_GBK16(40, 90, BLACK, GRAY0, "������");
//     Gui_DrawFont_GBK16(40, 120, BLACK, GRAY0, "�Ե´���");
// }

// // �ֶ���վ��ʼ���棨�Եµ�����ɳ����
// void my_pageC19(void)
// {
//     Lcd_Clear(GRAY0);
//     LCD_LED_SET; // ͨ��IO���Ʊ�����
//     Gui_DrawFont_GBK16(40, 30, BLACK, GRAY0, "�Ե´���");
//     Gui_DrawFont_GBK16(40, 60, BLACK, GRAY0, "������");
//     Gui_DrawFont_GBK16(40, 90, BLACK, GRAY0, "������");
//     Gui_DrawFont_GBK16(40, 120, BLACK, GRAY0, "����ɳ");
// }

// /* ��GPS��;������ʱ�򣬾�Ҫ��վ���е�������λ�����ֶ���վ�������ǻ��õ��Ľ��� */
// // ����ɳ���Ե·���
// void my_pageC20(void)
// {
//     Gui_DrawFont_GBK16(40, 30, BLACK, GREEN, "����ɳ");
//     Gui_DrawFont_GBK16(40, 60, BLACK, GRAY0, "������");
//     Gui_DrawFont_GBK16(40, 120, BLACK, GRAY0, "�Ե´���");
// }

// void my_pageC21(void)
// {
//     Gui_DrawFont_GBK16(40, 30, BLACK, GRAY0, "����ɳ");
//     Gui_DrawFont_GBK16(40, 60, BLACK, GREEN, "������");
//     Gui_DrawFont_GBK16(40, 90, BLACK, GRAY0, "������");
// }

// void my_pageC22(void)
// {
//     Gui_DrawFont_GBK16(40, 60, BLACK, GRAY0, "������");
//     Gui_DrawFont_GBK16(40, 90, BLACK, GREEN, "������");
//     Gui_DrawFont_GBK16(40, 120, BLACK, GRAY0, "�Ե´���");
// }

// void my_pageC23(void)
// {
//     Gui_DrawFont_GBK16(40, 30, BLACK, GRAY0, "����ɳ");
//     Gui_DrawFont_GBK16(40, 90, BLACK, GRAY0, "������");
//     Gui_DrawFont_GBK16(40, 120, BLACK, GREEN, "�Ե´���");
// }

// // �Եµ�����ɳ����
// void my_pageC24(void)
// {
//     Gui_DrawFont_GBK16(40, 30, BLACK, GREEN, "�Ե´���");
//     Gui_DrawFont_GBK16(40, 60, BLACK, GRAY0, "������");
//     Gui_DrawFont_GBK16(40, 120, BLACK, GRAY0, "����ɳ");
// }

// void my_pageC25(void)
// {
//     Gui_DrawFont_GBK16(40, 30, BLACK, GRAY0, "�Ե´���");
//     Gui_DrawFont_GBK16(40, 60, BLACK, GREEN, "������");
//     Gui_DrawFont_GBK16(40, 90, BLACK, GRAY0, "������");
// }

// void my_pageC26(void)
// {
//     Gui_DrawFont_GBK16(40, 60, BLACK, GRAY0, "������");
//     Gui_DrawFont_GBK16(40, 90, BLACK, GREEN, "������");
//     Gui_DrawFont_GBK16(40, 120, BLACK, GRAY0, "����ɳ");
// }

// void my_pageC27(void)
// {
//     Gui_DrawFont_GBK16(40, 30, BLACK, GRAY0, "�Ե´���");
//     Gui_DrawFont_GBK16(40, 90, BLACK, GRAY0, "������");
//     Gui_DrawFont_GBK16(40, 120, BLACK, GREEN, "����ɳ");
// }
