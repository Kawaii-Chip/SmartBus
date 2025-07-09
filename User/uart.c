#include <uart.h>

void USART1_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast"))); // 串口1快速中断 A9 10
void USART3_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast"))); // 串口3快速中断 A2 3
//void USART2_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast"))); // 串口2快速中断 B10 11 9600
void UART6_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));  // 串口6快速中断 C0 1
void UART8_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));  // 串口8快速中断 C4 5 9600



void Usart1_Init()
{
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;

    GPIO_Init(GPIOA, &GPIO_InitStructure);
    USART_InitStructure.USART_BaudRate = 115200;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
    USART_Init(USART1, &USART_InitStructure);
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    USART_Cmd(USART1, ENABLE);
    USART_ClearFlag(USART1, USART_FLAG_TC); // 清空串口2的发送标志位
}

u8 uart1_rec_string[256], uart1_rec_index;
u32 uart1_rec_tick;

void USART1_IRQHandler(void)
{
    u8 temp;
    if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
    {
        uart1_rec_tick = HAL_GetTick();

        uart1_rec_string[uart1_rec_index] = USART_ReceiveData(USART1);
        uart1_rec_index++;
    }
    USART_ClearITPendingBit(USART1, USART_IT_RXNE);
}

void uart1_printf(const char *format, ...)
{
    char buffer[64]; // 缓冲区大小可根据需求调整
    va_list args;

    // 格式化字符串到缓冲区
    va_start(args, format);
    vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);

    // 发送缓冲区内容
    u8 i = 0;
    while (buffer[i] != '\0')
    {
        USART_SendData(USART1, buffer[i++]);
        while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == 0)
            ;
    }
}

void Usart2_Init()
{
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;

    GPIO_Init(GPIOA, &GPIO_InitStructure);
    USART_InitStructure.USART_BaudRate = 115200;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
    USART_Init(USART2, &USART_InitStructure);
    USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
    NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    USART_Cmd(USART2, ENABLE);
    USART_ClearFlag(USART2, USART_FLAG_TC); // 清空串口2的发送标志位
}

// u8 uart2_rec_string[256], uart2_rec_index;
// u32 uart2_rec_tick;

// void USART2_IRQHandler(void)
// {
//     u8 temp;
//     if (USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
//     {
//         uart2_rec_tick = 0;

//         uart2_rec_string[uart2_rec_index] = USART_ReceiveData(USART2);
//         uart2_rec_index++;
//     }
//     USART_ClearITPendingBit(USART2, USART_IT_RXNE);
// }

void uart2_printf(const char *format, ...)
{
    char buffer[64]; // 缓冲区大小可根据需求调整
    va_list args;

    // 格式化字符串到缓冲区
    va_start(args, format);
    vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);

    // 发送缓冲区内容
    u8 i = 0;
    while (buffer[i] != '\0')
    {
        USART_SendData(USART2, buffer[i++]);
        while (USART_GetFlagStatus(USART2, USART_FLAG_TC) == 0)
            ;
    }
}

void Usart3_Init() // 语音发送
{
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;

    GPIO_Init(GPIOB, &GPIO_InitStructure);
    USART_InitStructure.USART_BaudRate = 9600;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
    USART_Init(USART3, &USART_InitStructure);
    USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
    NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    USART_Cmd(USART3, ENABLE);
    USART_ClearFlag(USART3, USART_FLAG_TC); // 清空串口3的发送标志位
}

u8 uart3_rec_string[256], uart3_rec_index;
u32 uart3_rec_tick;

void USART3_IRQHandler(void)
{
    u8 temp;
    if (USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)
    {
        uart3_rec_tick = 0;

        uart3_rec_string[uart3_rec_index] = USART_ReceiveData(USART3);
        uart3_rec_index++;
    }
    USART_ClearITPendingBit(USART3, USART_IT_RXNE);
}

void uart3_printf(const char *format, ...)
{
    char buffer[64]; // 缓冲区大小可根据需求调整
    va_list args;

    // 格式化字符串到缓冲区
    va_start(args, format);
    vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);

    // 发送缓冲区内容
    u8 i = 0;
    while (buffer[i] != '\0')
    {
        USART_SendData(USART3, buffer[i++]);
        while (USART_GetFlagStatus(USART3, USART_FLAG_TC) == 0)
            ;
    }
}

void Uart6_Init()
{
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART6, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;

    GPIO_Init(GPIOC, &GPIO_InitStructure);

    USART_InitStructure.USART_BaudRate = 115200;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;

    USART_Init(UART6, &USART_InitStructure);

    USART_ITConfig(UART6, USART_IT_RXNE, ENABLE);

    NVIC_InitStructure.NVIC_IRQChannel = UART6_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    USART_Cmd(UART6, ENABLE);
    USART_ClearFlag(UART6, USART_FLAG_TC); // 清空串口6的发送标志位
    
}

void uart6_printf(const char *format, ...)
{
    char buffer[64]; // 缓冲区大小可根据需求调整
    va_list args;

    // 格式化字符串到缓冲区
    va_start(args, format);
    vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);

    // 发送缓冲区内容
    u8 i = 0;
    while (buffer[i] != '\0')
    {
        USART_SendData(UART6, buffer[i++]);
        while (USART_GetFlagStatus(UART6, USART_FLAG_TC) == 0)
            ;
    }
}


u8 uart6_rec_string[256], uart6_rec_index;
u32 uart6_rec_tick;
u8 uart6_flag;
void UART6_IRQHandler(void)
{
    u8 temp = 0;
    if (USART_GetITStatus(UART6, USART_IT_RXNE) != RESET)
    {
        uart6_rec_string[uart6_rec_index] = USART_ReceiveData(UART6);
        uart6_rec_index++;
        uart6_flag = 1;
        USART_ClearITPendingBit(UART6, USART_IT_RXNE);
    }
    
}



void Uart8_Init()
{
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART8, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;

    GPIO_Init(GPIOC, &GPIO_InitStructure);

    USART_InitStructure.USART_BaudRate = 9600;
    USART_InitStructure.USART_WordLength =USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;

    USART_Init(UART8, &USART_InitStructure);

    USART_ITConfig(UART8, USART_IT_RXNE, ENABLE);

    NVIC_InitStructure.NVIC_IRQChannel = UART8_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    USART_Cmd(UART8, ENABLE);
    USART_ClearFlag(UART8, USART_FLAG_TC); // 清空串口6的发送标志位
}

void uart8_printf(const char *format, ...)
{
    char buffer[64]; // 缓冲区大小可根据需求调整
    va_list args;

    // 格式化字符串到缓冲区
    va_start(args, format);
    vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);

    // 发送缓冲区内容
    u8 i = 0;
    while (buffer[i] != '\0')
    {
        USART_SendData(UART8, buffer[i++]);
        while (USART_GetFlagStatus(UART8, USART_FLAG_TC) == 0)
            ;
    }
}
u8 uart8_rec_string[256], uart8_rec_index;
u32 uart8_rec_tick;
void UART8_IRQHandler(void)
{
    u8 temp = 0;
    if (USART_GetITStatus(UART8, USART_IT_RXNE) != RESET)
    {
        uart8_rec_tick = HAL_GetTick();
        temp = USART_ReceiveData(UART8);
        uart8_rec_string[uart8_rec_index] = temp;
        uart8_rec_index++;
    }
    USART_ClearITPendingBit(UART8, USART_IT_RXNE);
}

void string_copy(u8 *string1, u8 *string2, u8 len)
{
    u8 i;
    for (i = 0; i < len; ++i)
    {
        string2[i] = string1[i];
    }
}
u8 string_chek(u8 *string1, u8 *string2, u8 len)
{
    u8 i;
    for (i = 0; i < len; i++)
    {
        if (string1[i] != string2[i])
        {
            return 0; // 发现不相等，立即返回0
        }
    }
    return 1; // 全部相等，返回1
}