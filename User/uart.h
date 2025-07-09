#ifndef UART_H
#define UART_H
#include <bsp_system.h>

extern u8 uart1_rec_string[256], uart1_rec_index;
extern u32 uart1_rec_tick;
extern u8 uart2_rec_string[256], uart2_rec_index;
extern u32 uart2_rec_tick;
extern u8 uart3_rec_string[256], uart3_rec_index;
extern u32 uart3_rec_tick;
extern u8 uart6_rec_string[256], uart6_rec_index;
extern u32 uart6_rec_tick;
extern u8 uart8_rec_string[256], uart8_rec_index;
extern u32 uart8_rec_tick;
extern u8 uart6_flag;

void Usart1_Init();
void uart1_printf(const char *format, ...);
void UART1_Printf(char *format, ...);

void Usart2_Init();
void uart2_printf(const char *format, ...);

void Usart3_Init();
void uart3_printf(const char *format, ...);

void Uart6_Init();
void uart6_printf(const char *format, ...);

void Uart8_Init();
void uart8_printf(const char *format, ...);

void string_copy(u8 *string1, u8 *string2, u8 len);
u8 string_chek(u8 *string1, u8 *string2, u8 len); // 检测两个数组前len位是否一致

#endif
