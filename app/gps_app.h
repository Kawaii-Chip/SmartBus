#ifndef GPS_H
#define GPS_H
#include <bsp_system.h>

extern struct GPS gps;

void gps_proc();//车辆位置更新
void gps_arrive();
void gps_read();
void show_td();


#endif

