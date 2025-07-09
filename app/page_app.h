#ifndef __PAGE__H__
#define __PAGE__H__
#include <bsp_system.h>

extern uint32_t rt_longitude;
extern uint32_t rt_latitude;
extern u8 arrive_flag; // 0:ÐÐÊ» 1£ºÍ£Ö¹

void my_pageA0(void);
void my_pageA1(void);
void my_pageB2(void);
void my_pageB3(void);
void my_pageB5(); // flag1
void my_pageB6(); // flag0
void bus_proc(void);
#endif
