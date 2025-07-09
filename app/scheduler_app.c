#include <scheduler_app.h>

typedef struct
{
    void (*task_func)(void);
    u32 rate_ms;
    u32 last_run;
} task_t;

task_t scheduler_task[] =
    {
            {onenet_proc,10,0}
            ,{nfc_proc,20,0}
         ,{gps_proc, 2000, 0}
         ,{lcd_proc, 1000, 0}
         ,{key_proc,50,0}
         ,{bus_proc,500,0}

};
        u8 task_num;
void scheduler_init(void)
{
    // 计算任务数组的元素个数，并将结果存储在 task_num 中
    task_num = sizeof(scheduler_task) / sizeof(task_t);


    
    gps_init();

    printf("usart1\r\n");
    DHT11_Init();
     onenet_init();
    lcd_init();
    GPIO_Init_PWM();
    GPIO_Init_PWM2();
    Touch_Key_Init();
    sd_app_init();
}

/**
 * @brief 调度器运行函数
 * 遍历任务数组，检查是否有任务需要执行。如果当前时间已经超过任务的执行周期，则执行该任务并更新上次运行时间
 */
void scheduler_run(void)
{

    // 遍历任务数组中的所有任务
    for (uint8_t i = 0; i < task_num; i++)
    {
        // 获取当前的系统时间（毫秒）
        uint32_t now_time = HAL_GetTick();

        // 检查当前时间是否达到任务的执行时间
        if (now_time > scheduler_task[i].rate_ms + scheduler_task[i].last_run)
        {
            // 更新任务的上次运行时间为当前时间
            scheduler_task[i].last_run = now_time;

            // 执行任务函数
            scheduler_task[i].task_func();
        }
    }
}
