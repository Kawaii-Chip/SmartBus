#include "onewire_app.h"

DHT11_data DHT11;

void DHT11_Init()
{
    //===============================【初始化】================================
    GPIO_InitTypeDef GPIO_InitStructure = {0};

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); // 使能APB2 GPIO外设时钟

    GPIO_InitStructure.GPIO_Pin = DHT11_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD; // 设置为开漏输出模式并外置上拉电阻
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(DHT11_PORT, &GPIO_InitStructure);

    DHT11_SET(1);   // 释放总线
    Delay_Ms(1500); // 越过不稳定状态
}

unsigned char DHT11_Read()
{
    //===============================【读取温湿度】================================
    // 返回值：错误代码，0表示成功
    //===========================================================================

    uint8_t i, j;
    uint8_t data[5] = {0x00};
    // 步骤二
    DHT11_SET(0);
    Delay_Ms(20);
    DHT11_SET(1); // 释放总线
    // 步骤三
    Delay_Us(30);
    if (DHT11_READ() == Bit_SET)
        return 1;
    while (DHT11_READ() != Bit_SET)
        ; // 等待高电平
    Delay_Us(90);

    for (i = 0; i < 5; i++)
    {
        for (j = 0; j < 8; j++)
        {
            data[i] <<= 1;
            while (DHT11_READ() == Bit_SET)
                ; // 如果是低电平则下一步
            while (DHT11_READ() != Bit_SET)
                ; // 等待54us的低电平过去
            Delay_Us(40);
            if (DHT11_READ() == Bit_SET)
                data[i] |= 0x01;
        }
    }

    if ((data[0] + data[1] + data[2] + data[3]) != data[4])
        return 2; // 校验

    DHT11.humidity = data[0] + data[1] / 10.0f;

    if (data[3] & 0x80)
        DHT11.temperature = -(data[2] + (data[3] & 0x7f) / 10.0f); // 负温度处理
    else
        DHT11.temperature = data[2] + data[3] / 10.0f;

    while (!DHT11_READ())
        ;     // 等待结束信号
    return 0; // 成功
}


void show_temperature()
{
     DHT11_Read();

    int T = DHT11.temperature;
    int H = DHT11.humidity;
    printf("AT+MQTTPUB=0,\"$sys/mmMJx639W9/ch32/thing/property/post\",\"{\\\"id\\\":\\\"123\\\"\\,\\\"params\\\":{\\\"DTH11\\\":{\\\"value\\\":[%d\\,%d]}}}\",0,0\r\n", T, H);
    char original[] = "T: H:";

    // 找到冒号位置
    char *temp_colon = strchr(original, ':');
    char *hum_colon = strrchr(original, ':');

    // 计算插入位置（冒号后一个位置）
    int pos1 = (temp_colon) ? (temp_colon - original + 1) : 0;
    int pos2 = (hum_colon) ? (hum_colon - original + 1) : 0;

    // 插入整数
    char *result = insertTwoInts(original, pos1, T, pos2, H);
    
    if (result)
    {
        char str2[] = "person:";
        LCD_ShowString(0, 165, str2, BROWN, WHITE, 32, 0);

        int num = getCurrentPassengerCount();
        LCD_ShowIntNum(112, 165, num,1, BLACK, WHITE, 32);
        LCD_ShowString(170, 165, result, GREEN, WHITE, 32, 0);
    }
}