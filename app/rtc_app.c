#include "rtc_app.h"

// 定义舵机的PWM周期和占空比
#define PWM_PERIOD 20000 // PWM周期为20ms (20000us)
#define PWM_MIN 500      // 最小脉宽为0.5ms (500us)
#define PWM_MAX 2500     // 最大脉宽为2.5ms (2500us)

// 初始化TIM2以生成PWM信号
void TIM2_PWM_Init(void)
{
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); // 使能TIM2时钟

    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_OCInitTypeDef TIM_OCInitStructure;

    // 计算参数（修改：频率从250Hz调整为400Hz）
    uint16_t PrescalerValue = 96 - 1; // 预分频器，96MHz/96 = 1MHz (1us)
    uint16_t PeriodValue = 25000 - 1; // 周期 = 1MHz/400Hz = 2500，因此Period为2499

    // 配置TIM2的基本参数
    TIM_TimeBaseStructure.TIM_Period = PeriodValue;       // 设置自动重装值
    TIM_TimeBaseStructure.TIM_Prescaler = PrescalerValue; // 设置预分频器
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

    // 配置TIM2的PWM模式（修改：Pulse值适配新周期以保持50%占空比）
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse = 12500; // 占空比50% (25000 × 50% = 12500)
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;

    // 配置TIM2通道1
    TIM_OC1Init(TIM2, &TIM_OCInitStructure);          // 使用TIM2的通道1
    TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Enable); // 使能TIM2通道1的预装载寄存器

    TIM_OC2Init(TIM2, &TIM_OCInitStructure);          // 使用TIM2的通道2（修正注释）
    TIM_OC2PreloadConfig(TIM2, TIM_OCPreload_Enable); // 使能TIM2通道2的预装载寄存器

    TIM_ARRPreloadConfig(TIM2, ENABLE); // 使能TIM2的自动重装寄存器预装载

    TIM_Cmd(TIM2, ENABLE); // 使能TIM2
}
// 初始化GPIO以输出PWM信号
void GPIO_Init_PWM(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); // 使能GPIOA时钟

    GPIO_InitTypeDef GPIO_InitStructure;

    // 配置PA0为复用推挽输出（TIM2通道1）
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);


}

void TIM4_PWM_Init(void)
{
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); // 使能TIM4时钟（APB1总线）

    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_OCInitTypeDef TIM_OCInitStructure;

    // 计算参数（保持与原TIM2相同的频率）
    uint16_t PrescalerValue = 96 - 1; // 预分频器，96MHz/96 = 1MHz (1us)
    uint16_t PeriodValue = 4000 - 1;  // 周期 = 1MHz/250Hz = 4000，因此Period为3999

    // 配置TIM4的基本参数
    TIM_TimeBaseStructure.TIM_Period = PeriodValue;       // 设置自动重装值
    TIM_TimeBaseStructure.TIM_Prescaler = PrescalerValue; // 设置预分频器
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);

    // 配置TIM4的PWM模式
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse = 2000; // 占空比50% (2000/4000)
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;

    // 配置TIM4通道1（仅保留CH1配置，移除CH2）
    TIM_OC1Init(TIM4, &TIM_OCInitStructure);          // 使用TIM4的通道1
    TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable); // 使能TIM4通道1的预装载寄存器

    TIM_ARRPreloadConfig(TIM4, ENABLE); // 使能TIM4的自动重装寄存器预装载

    TIM_Cmd(TIM4, ENABLE); // 使能TIM4
}

// 初始化GPIO以输出PWM信号（需根据实际引脚修改）
void GPIO_Init_PWM2(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); // 使能GPIOB时钟

    GPIO_InitTypeDef GPIO_InitStructure;

    // 配置PB6为通用推挽输出（非复用功能）
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; // 修改：通用推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    // 设置PB6为低电平
    GPIO_ResetBits(GPIOB, GPIO_Pin_6); // 新增：明确设置低电平
}

void fan(u8 enable)
{
    
    if(enable)
    {
        uart6_printf("fan:1");
        audio_play(72);
    }
    else
    {
        uart6_printf("fan:0");

        audio_play(71);
    }
}

void led(u8 enable)
{

    if (enable)
    {
        uart6_printf("led:0");
        audio_play(74);
    }
    else
    {
        audio_play(73);
        uart6_printf("led:1");
    }
}
