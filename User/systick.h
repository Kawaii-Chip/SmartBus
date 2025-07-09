#ifndef SYSTICK_H
#define SYSTICK_H
#include <bsp_system.h>

extern volatile uint32_t uwticks;

void TIM2_Init(void);
uint32_t HAL_GetTick(void);

#endif
