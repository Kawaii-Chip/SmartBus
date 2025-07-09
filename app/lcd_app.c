#include <lcd_app.h>
#include <picture.h>

char *insertTwoCharArrays(char *original, int pos1, const char *insert1, int pos2, const char *insert2)
{
    // ���������Ч��
    if (original == NULL || insert1 == NULL || insert2 == NULL || pos1 < 0 || pos2 < 0)
    {
        return NULL;
    }

    int origLen = strlen(original);
    int ins1Len = strlen(insert1);
    int ins2Len = strlen(insert2);

    // ��������λ�ã�ȷ��pos1 <= pos2�Ҳ�����ԭ�ַ�������
    if (pos1 > origLen)
        pos1 = origLen;
    if (pos2 > origLen)
        pos2 = origLen;
    if (pos2 < pos1)
    {
        int temp = pos1;
        pos1 = pos2;
        pos2 = temp;
    }

    // �������ڴ�
    char *result = (char *)malloc(origLen + ins1Len + ins2Len + 1);
    if (result == NULL)
    {
        return NULL; // �ڴ����ʧ��
    }

    // ����ԭ�ַ�����pos1֮ǰ�Ĳ���
    memcpy(result, original, pos1);

    // ���Ƶ�һ�������ַ���
    memcpy(result + pos1, insert1, ins1Len);

    // ��������һ���ַ�����ԭ�ַ���ʣ�ಿ�ֵ���λ��
    int newPos2 = pos2 + ins1Len;
    int origPart2Len = pos2 - pos1;

    // ����ԭ�ַ�����pos1��pos2֮��Ĳ���
    memcpy(result + pos1 + ins1Len, original + pos1, origPart2Len);

    // ���Ƶڶ��������ַ���
    memcpy(result + newPos2, insert2, ins2Len);

    // ����ԭ�ַ���ʣ�ಿ��
    memcpy(result + newPos2 + ins2Len, original + pos2, origLen - pos2 + 1); // +1 ������ֹ��

    return result;
}

char *insertTwoInts(char *original, int pos1, int num1, int pos2, int num2)
{
    if (!original)
        return NULL;

    // ����ԭʼ�ַ�������
    int len_orig = strlen(original);

    // ȷ������λ����Ч
    if (pos1 < 0)
        pos1 = 0;
    if (pos1 > len_orig)
        pos1 = len_orig;
    if (pos2 < pos1)
        pos2 = pos1;
    if (pos2 > len_orig)
        pos2 = len_orig;

    // �������������������󻺳�����С
    char num1_buf[20], num2_buf[20];
    int len_num1 = snprintf(num1_buf, sizeof(num1_buf), "%d", num1);
    int len_num2 = snprintf(num2_buf, sizeof(num2_buf), "%d", num2);

    // �������ַ������ܳ���
    int new_len = len_orig + len_num1 + len_num2 + 1;
    char *result = (char *)malloc(new_len);
    if (!result)
        return NULL;

    // ���Ƶ�һ���֣�original[0] �� original[pos1-1]��
    memcpy(result, original, pos1);

    // �����һ������
    memcpy(result + pos1, num1_buf, len_num1);

    // ����ڶ����ֵ���ʼλ�úͳ���
    int part2_start = pos1 + len_num1;
    int part2_len = pos2 - pos1;

    // ���Ƶڶ����֣�original[pos1] �� original[pos2-1]��
    memcpy(result + part2_start, original + pos1, part2_len);

    // ����ڶ�������
    memcpy(result + part2_start + part2_len, num2_buf, len_num2);

    // ����ʣ�ಿ�֣�original[pos2] ��ĩβ��
    memcpy(result + part2_start + part2_len + len_num2, original + pos2, len_orig - pos2);

    // ����ַ���������
    result[new_len - 1] = '\0';

    return result;
}
u32 count;

void lcd_init()
{
    LCD_Init();

    LCD_Fill(0, 0, 320, 240, WHITE);


    LCD_ShowPicture(0, 0, 320, 200, gImage_1);
   // Delay_Ms(2000);
    LCD_Fill(0, 0, 320, 240, WHITE);
}

u8 lcd_mode, lcd0_flag, lcd1_flag;
u8 old,old0=10,old1=10;
void lcd_proc()
{
    switch(lcd_mode)
    {
        case 0:
            if (old0 != lcd0_flag)
            {
                old0 = lcd0_flag;
                if (lcd0_flag == 0)
                    my_pageA0();
                else
                    my_pageA1();
            }
                
            break;
        case 1:
            if (old1 != lcd1_flag)
            {
                old1 = lcd1_flag;
                if (lcd1_flag == 0)
                    my_pageB2();
                else
                    my_pageB3();
            }
            
            break;
        case 2:
            show_temperature(); // û�д�read��Ϊ�Ῠ��
            show_td();
            showgps();
            if (old != arrive_flag)
            {
                old = arrive_flag;
                LCD_Fill(0, 0, 320, 240, WHITE);
            }
                if (arrive_flag == 1)
                    my_pageB5();
                else
                    my_pageB6();
            break;
    }
    
}
