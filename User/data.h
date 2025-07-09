#ifndef BUS_SIM_H
#define BUS_SIM_H
#include <bsp_system.h>
#include <stdbool.h>

// 宏定义
#define STATIONS 10         // 站点总数
#define DAY_MINUTES 1440    // 一天总分钟数
#define MAX_PASSENGERS 1000 // 报表数组最大乘客数（实际链表无固定限制）

// 乘客结构体（链表节点）
typedef struct Passenger
{
    int station;            // 上车站点
    int time;               // 上车时间（分钟）
    int destination;        // 下车站点
    struct Passenger *next; // 指向下一乘客的指针
} Passenger;

// 日报表结构体
typedef struct
{
    int count[STATIONS][STATIONS];    // 站点间流量统计
    int times[MAX_PASSENGERS];        // 乘客上车时间数组
    int destinations[MAX_PASSENGERS]; // 乘客目的地数组
    int passenger_count;              // 总乘客数
} DailyReport;

// 全局变量声明（如需在多文件中使用，需通过 extern 声明）
extern int current_station;   // 当前站点
extern int next_station;      // 下一站站点
extern char request_handle[2]; // 上车请求句柄：[是否有请求, 上车点]
extern DailyReport report;    // 日报表结构体
extern Passenger *head;       // 乘客链表头指针
extern Passenger *pending_destinations[MAX_PASSENGERS]; // 待处理目的地链表
extern int dropoffs[STATIONS];
extern int station_requests[STATIONS];

// 初始化函数
void initReport(DailyReport *report);

// 乘客操作函数
void passengerRequest(int station);                                     // 记录乘客请求（仅上车点）
bool processBoarding(int current_time, int destination, int *dropoffs); // 处理乘客上车
int processDropoffs(int current_station, int *dropoffs);                // 处理乘客下车

// 辅助函数
int findNextStation(int *station_requests, int *dropoffs); // 查找下一站
void freePassengers();                                     // 释放乘客链表内存
void generateReport(DailyReport *report);                  // 生成日报表
int getCurrentPassengerCount(void);

#endif // BUS_SIM_H