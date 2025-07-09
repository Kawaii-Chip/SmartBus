#include "systick.h"

// 全局变量用于记录系统启动以来的毫秒数
volatile uint32_t uwticks = 0;

// 定时器 TIM3 初始化函数
void TIM3_Init(void)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

    // 定时器 TIM2 基础结构体初始化
    TIM_TimeBaseStructure.TIM_Period = SystemCoreClock / 1000 - 1; // 1ms 周期
    TIM_TimeBaseStructure.TIM_Prescaler = 0;
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

    // 使能 TIM2 中断
    TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);

    // 配置 TIM2 中断优先级
    NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    // 使能 TIM2
    TIM_Cmd(TIM3, ENABLE);
}

// 类似 HAL_GetTick 的函数
uint32_t HAL_GetTick(void)
{
    return uwticks;
}

// 简单的延时函数，单位为毫秒

// TIM2 中断处理函数
void TIM3_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast"))); // 定时器3快速中断

void TIM3_IRQHandler(void)
{
    if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)
    {
        uwticks++;
        TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
    }
}

