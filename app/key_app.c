#include <key_app.h>

#define TOUCHKEY1_PIN GPIO_Pin_1 // PA1
#define TOUCHKEY1_CHANNEL ADC_Channel_1
#define TOUCHKEY2_PIN GPIO_Pin_4 // PA4
#define TOUCHKEY2_CHANNEL ADC_Channel_4

/*********************************************************************
 * @fn      Touch_Key_Init
 *
 * @brief   Initializes Touch Key collection.
 *
 * @return  none
 */
void Touch_Key_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure = {0};
    ADC_InitTypeDef ADC_InitStructure = {0};

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
    RCC_ADCCLKConfig(RCC_PCLK2_Div8);

    // 初始化PA1和PA4为模拟输入
    GPIO_InitStructure.GPIO_Pin = TOUCHKEY1_PIN | TOUCHKEY2_PIN; // PA1, PA4
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
    ADC_InitStructure.ADC_ScanConvMode = DISABLE;
    ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
    ADC_InitStructure.ADC_NbrOfChannel = 1;
    ADC_Init(ADC1, &ADC_InitStructure);

    ADC_Cmd(ADC1, ENABLE);
    TKey1->CTLR1 |= (1 << 26) | (1 << 24); // Enable TouchKey and Buffer
}

/*********************************************************************
 * @fn      Touch_Key_Adc
 *
 * @brief   Returns ADCx conversion result data.
 *
 * @param   ch - ADC channel.
 *            ADC_Channel_0 - ADC Channel0 selected.
 *            ADC_Channel_1 - ADC Channel1 selected.
 *            ADC_Channel_2 - ADC Channel2 selected.
 *            ADC_Channel_3 - ADC Channel3 selected.
 *            ADC_Channel_4 - ADC Channel4 selected.
 *            ADC_Channel_5 - ADC Channel5 selected.
 *            ADC_Channel_6 - ADC Channel6 selected.
 *            ADC_Channel_7 - ADC Channel7 selected.
 *            ADC_Channel_8 - ADC Channel8 selected.
 *            ADC_Channel_9 - ADC Channel9 selected.
 *            ADC_Channel_10 - ADC Channel10 selected.
 *            ADC_Channel_11 - ADC Channel11 selected.
 *            ADC_Channel_12 - ADC Channel12 selected.
 *            ADC_Channel_13 - ADC Channel13 selected.
 *            ADC_Channel_14 - ADC Channel14 selected.
 *            ADC_Channel_15 - ADC Channel15 selected.
 *            ADC_Channel_16 - ADC Channel16 selected.
 *            ADC_Channel_17 - ADC Channel17 selected.
 *
 * @return  val - The Data conversion value.
 */
u16 Touch_Key_Adc(u8 ch)
{
    ADC_RegularChannelConfig(ADC1, ch, 1, ADC_SampleTime_7Cycles5);
    TKey1->IDATAR1 = 0x10; // Charging Time
    TKey1->RDATAR = 0x8;   // Discharging Time
    while (!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC))
        ;
    return (uint16_t)TKey1->RDATAR;
}

u16 ADC_val1, ADC_val2;
const u16 threshold = 2000; // 触摸阈值，根据实际情况调整
u8 key1_pressed = 0, key2_pressed = 0;

void key_proc()
{
    ADC_val1 = Touch_Key_Adc(TOUCHKEY1_CHANNEL);
    ADC_val2 = Touch_Key_Adc(TOUCHKEY2_CHANNEL);

    // 检测key1
    if (ADC_val1 < threshold && !key1_pressed)
    {
        if (lcd_mode == 0)
            lcd0_flag ^= 1;
        else if (lcd_mode == 1)
            lcd1_flag ^= 1;
        printf("key1\r\n");
        key1_pressed = 1;
    }
    else if (ADC_val1 >= threshold && key1_pressed)
    {
        key1_pressed = 0;
    }

    // 检测key2
    if (ADC_val2 < threshold && !key2_pressed)
    {
        printf("key2\r\n");
        key2_pressed = 1;
        lcd_mode++;
        LCD_Fill(0, 0, 320, 240, WHITE);
    }
    else if (ADC_val2 >= threshold && key2_pressed)
    {
        key2_pressed = 0;
    }

    // u32 now_time = HAL_GetTick();
    
    // if (now_time >= uart6_rec_tick + 10)
    // {
    //     if (uart6_rec_string[0]=='1')
    //     {
    //         if (lcd_mode==0)
    //             lcd0_flag ^= 1;
    //         else if (lcd_mode==1)
    //             lcd1_flag ^= 1;
    //     }
    //     else if (uart6_rec_string[0] == '2')
    //     {
    //         lcd_mode++;
    //         LCD_Fill(0, 0, 320, 240, WHITE);
    //     }
    //     memset(uart6_rec_string, 0, uart6_rec_index);
    //     uart6_rec_index = 0;
    //     }
}