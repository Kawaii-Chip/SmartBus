#include <data.h>
#include <sd_app.h>


// 全局变量
int current_station = 0;
int next_station = 0;
char request_handle[2] = {0, -1}; // [是否有请求, 上车点]
int dropoffs[STATIONS] = {0};
int station_requests[STATIONS] = {0};

DailyReport report;
Passenger *head = NULL;                                   // 乘客链表头指针
Passenger *pending_destinations[MAX_PASSENGERS] = {NULL}; // 待处理目的地链表

// 初始化日报表
void initReport(DailyReport *report)
{
    for (int i = 0; i < STATIONS; i++)
        for (int j = 0; j < STATIONS; j++)
            report->count[i][j] = 0;
    report->passenger_count = 0;
    request_handle[0] = 0;
}

// 创建新乘客节点
Passenger *createPassenger(int station, int time, int destination)
{
    Passenger *newNode = (Passenger *)malloc(sizeof(Passenger));
    newNode->station = station;
    newNode->time = time;
    newNode->destination = destination;
    newNode->next = NULL;
    return newNode;
}



// 处理乘客上车（链表版本）
bool processBoarding(int current_time, int destination, int *dropoffs)
{
    int station = current_station;
    

    // 创建新乘客节点并添加到链表头部
    Passenger *newPassenger = createPassenger(station, current_time, destination);
    newPassenger->next = head;
    head = newPassenger;

    // 更新报表数据
    report.count[station][destination]++;
    report.times[report.passenger_count] = current_time;
    report.destinations[report.passenger_count++] = destination;

    // 更新下车计数
    dropoffs[destination]++;

    // 重置请求句柄
    request_handle[0] = 0;
    request_handle[1] = -1;
    audio_play(30);
    u8 i = 30 + destination;
    audio_play(i);
    printf("站点%d有乘客上车，目的地%d\r\n",
           station, destination);
    return true;
}

// 处理乘客下车（链表版本）
int processDropoffs(int current_station, int *dropoffs)
{
    int dropped = 0;
    Passenger *prev = NULL;
    Passenger *curr = head;

    while (curr != NULL)
    {
        if (curr->destination == current_station)
        {
            // 处理头节点情况
            if (prev == NULL)
            {
                head = curr->next;
            }
            else
            {
                prev->next = curr->next;
            }
            // 释放内存
            free(curr);
            dropped++;
            // 继续从当前位置查找（可能有连续下车）
            curr = prev ? prev->next : head;
        }
        else
        {
            prev = curr;
            curr = curr->next;
        }
    }

    if (dropped > 0)
    {
        dropoffs[current_station] = 0; // 重置下车计数
        printf(" 站点%d%d名乘客下车\r\n",
               current_station, dropped);
    }
    return dropped;
}

// 查找下一个有任务的站点
int findNextStation(int *station_requests, int *dropoffs)
{
    for (int i = 1; i < STATIONS; i++)
    {
        int candidate = (current_station + i) % STATIONS;
        if (station_requests[candidate] > 0 ||
            dropoffs[candidate] > 0)
        {
            
            
            // printf("下一站是%d\r\n", candidate);
            return candidate;
        }
    }
    
    printf("无任务按顺序行驶\r\n");
    return (current_station + 1) % STATIONS; // 无任务时按顺序行驶
}

// 释放链表内存
void freePassengers()
{
    Passenger *curr = head;
    while (curr != NULL)
    {
        Passenger *next = curr->next;
        free(curr);
        curr = next;
    }
    head = NULL;
}


// 生成报表（保持原有逻辑）
void generateReport(DailyReport *report)
{

    // 流量统计
    // for (int i = 0; i < STATIONS; i++)
    //     for (int j = 0; j < STATIONS; j++)
    //         if (report->count[i][j] > 0)
    //         {
    //             printf("AT+MQTTPUB=0,\"$sys/mmMJx639W9/ch32/thing/property/post\",\"{\\\"id\\\":\\\"123\\\"\\,\\\"params\\\":{\\\"data\\\":{\\\"value\\\":[1\\,%d\\,%d\\,%d]}}}\",0,0\r\n", i, j, report->count[i][j]);
    //             Delay_Ms(500);
    //         }

    // 时间统计
    // int time_bins[24] = {0};
    // for (int i = 0; i < report->passenger_count; i++)
    //     time_bins[report->times[i] / 60]++;
    // for (int i = 0; i < 24; i++)
    //     if (time_bins[i] > 0)
    //     {
    //         printf("AT+MQTTPUB=0,\"$sys/mmMJx639W9/ch32/thing/property/post\",\"{\\\"id\\\":\\\"123\\\"\\,\\\"params\\\":{\\\"data\\\":{\\\"value\\\":[2\\,%d\\,%d]}}}\",0,0\r\n", i, time_bins[i]);
    //         Delay_Ms(500);
    //     }
    
    // 保存乘客日志到SD卡
    // printf("正在保存乘客日志到SD卡...\r\n");
    // FRESULT sd_result = savePassengerLogToSD(report, head);
    // if (sd_result == FR_OK)
    // {
    //     printf("乘客日志保存成功！\r\n");
    // }
    // else
    // {
    //     printf("乘客日志保存失败，错误代码: %d\r\n", sd_result);
    // }
           
}

int getCurrentPassengerCount(void)
{
    int count = 0;
    Passenger *curr = head;

    // 遍历链表，统计节点数量
    while (curr != NULL)
    {
        count++;
        curr = curr->next;
    }

    return count;
}