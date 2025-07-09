#include <sd_app.h>

FATFS fs;
FIL fnew;
UINT fnum;
FRESULT res_sd;
BYTE work[FF_MAX_SS];
u8 WriteBuf[] = {"Hello WCH!"};
u8 ReadSDBuf[11] = {0};
MKFS_PARM opt = {0};

/*********************************************************************
 * @fn      show_sdcard_info
 *
 * @brief   SD Card information.
 *
 * @return  none
 */
void show_sdcard_info(void)
{
    switch (SDCardInfo.CardType)
    {
    case SDIO_STD_CAPACITY_SD_CARD_V1_1:
        printf("Card Type:SDSC V1.1\r\n");
        break;
    case SDIO_STD_CAPACITY_SD_CARD_V2_0:
        printf("Card Type:SDSC V2.0\r\n");
        break;
    case SDIO_HIGH_CAPACITY_SD_CARD:
        printf("Card Type:SDHC V2.0\r\n");
        break;
    case SDIO_MULTIMEDIA_CARD:
        printf("Card Type:MMC Card\r\n");
        break;
    }
    printf("Card ManufacturerID:%d\r\n", SDCardInfo.SD_cid.ManufacturerID);
    printf("Card RCA:%d\r\n", SDCardInfo.RCA);
    printf("Card Capacity:%d MB\r\n", (u32)(SDCardInfo.CardCapacity >> 20));
    printf("Card BlockSize:%d\r\n\r\n", SDCardInfo.CardBlockSize);
}
/*********************************************************************
 * @fn      main
 *
 * @brief   Main program.
 *
 * @return  none
 */

void sd_app_init()
{
    opt.fmt = FM_FAT32;
    opt.n_fat = 1;
    opt.align = 0;
    opt.au_size = 0;
    while (SD_Init())
    {
        printf("SD Card Error!\r\n");
        delay_ms(100);
    }
    show_sdcard_info();
    printf("SD Card OK\r\n");

    printf("%d\r\n", SD_SelectDeselect((u32)(SDCardInfo.RCA << 16)));
    printf("%d\r\n", SD_GetState());
    printf("%d\r\n", SD_EnableWideBusOperation(1));

    res_sd = f_mount(&fs, "1:", 1); // SD mount
    if (res_sd == FR_OK)
    {
        printf("Disk mounted successfully\r\n");
    }
    if (res_sd == FR_NO_FILESYSTEM)
    {
        printf("Disk formatting\r\n");
        res_sd = f_mkfs("1:", &opt, work, sizeof(work));
        if (res_sd == FR_OK)
        {
            printf("Disk formatted successfully\r\n");
            res_sd = f_mount(&fs, "1:", 1);
            if (res_sd == FR_OK)
            {
                printf("Disk mounted successfully\r\n");
            }
            else
            {
                printf("Disk mounting failed\r\n");
                printf("error code%x\r\n", res_sd);
            }
        }
        else
        {
            printf("Disk formatting failed????§%d??\r\n", res_sd);
        }
    }
    res_sd = f_open(&fnew, (const char *)"1:/testWCH.txt", FA_CREATE_ALWAYS | FA_WRITE);
    if (res_sd != FR_OK)
    {
        printf("Create file error\r\n");
    }
    else
    {
        printf("Create file successfully\r\n");
    }
    printf("Writing......................\r\n");
    res_sd = f_write(&fnew, WriteBuf, 11, &fnum);
    for (int j = 0; j < sizeof(WriteBuf); j++)
    {
        printf("WriteBuf[%d]=%d\r\n", j, WriteBuf[j]);
    }
    if (res_sd != FR_OK)
    {
        printf("Write error\r\n");
    }
    f_close(&fnew);
    res_sd = f_open(&fnew, (const char *)"1:/testWCH.txt", FA_OPEN_EXISTING | FA_READ);
    if (res_sd != FR_OK)
    {
        printf("Open file error\r\n");
    }
    printf("Reading......................\r\n");
    res_sd = f_read(&fnew, ReadSDBuf, 11, (UINT *)&fnum);
    if (res_sd != FR_OK)
    {
        printf("Read error\r\n");
    }
    for (int i = 0; i < sizeof(ReadSDBuf); i++)
    {
        printf("ReadBuf[%d]=%d\r\n", i, ReadSDBuf[i]);
    }
}

/*********************************************************************
 * @fn      savePassengerLogToSD
 *
 * @brief   将乘客上车日志保存到SD卡的report.txt文件中
 *
 * @param   report - 日报表结构体指针
 * @param   head - 乘客链表头指针
 *
 * @return  FRESULT - 文件操作结果
 */
FRESULT savePassengerLogToSD(DailyReport *report, Passenger *head)
{
    FIL logFile;
    FRESULT res;
    UINT bytesWritten;
    char logBuffer[256];
    int logLength;

    // 打开或创建report.txt文件
    res = f_open(&logFile, "1:/report.txt", FA_CREATE_ALWAYS | FA_WRITE);
    if (res != FR_OK)
    {
        printf("创建report.txt文件失败，错误代码: %d\r\n", res);
        return res;
    }

    printf("开始保存乘客日志到SD卡...\r\n");

    // 写入文件头部信息
    logLength = sprintf(logBuffer, "=== 乘客上车日志报告 ===\r\n");
    logLength += sprintf(logBuffer + logLength, "总乘客数: %d\r\n", report->passenger_count);
    logLength += sprintf(logBuffer + logLength, "生成时间: %s\r\n", __DATE__);
    logLength += sprintf(logBuffer + logLength, "时间格式: 小时:分钟 (24小时制)\r\n");
    logLength += sprintf(logBuffer + logLength, "================================\r\n\r\n");

    res = f_write(&logFile, logBuffer, logLength, &bytesWritten);
    if (res != FR_OK)
    {
        printf("写入文件头部失败，错误代码: %d\r\n", res);
        f_close(&logFile);
        return res;
    }

    // 写入站点间流量统计
    logLength = sprintf(logBuffer, "站点间流量统计:\r\n");
    logLength += sprintf(logBuffer + logLength, "格式: [上车站点] -> [下车站点]: 乘客数量\r\n");

    for (int i = 0; i < STATIONS; i++)
    {
        for (int j = 0; j < STATIONS; j++)
        {
            if (report->count[i][j] > 0)
            {
                logLength += sprintf(logBuffer + logLength, "站点%d -> 站点%d: %d人\r\n",
                                     i, j, report->count[i][j]);
            }
        }
    }
    logLength += sprintf(logBuffer + logLength, "\r\n");

    res = f_write(&logFile, logBuffer, logLength, &bytesWritten);
    if (res != FR_OK)
    {
        printf("写入流量统计失败，错误代码: %d\r\n", res);
        f_close(&logFile);
        return res;
    }

        // 写入详细乘客记录（从链表）
    logLength = sprintf(logBuffer, "详细乘客记录:\r\n");
    logLength += sprintf(logBuffer + logLength, "格式: 时间(小时:分钟) | 上车站点 | 下车站点\r\n");
    logLength += sprintf(logBuffer + logLength, "----------------------------------------\r\n");
    
    Passenger *current = head;
    int recordCount = 0;
    
    while (current != NULL)
    {
        int hours = current->time / 60;
        int minutes = current->time % 60;
        logLength += sprintf(logBuffer + logLength, "%02d:%02d | 站点%d | 站点%d\r\n", 
                           hours+8, minutes, current->station, current->destination);
        recordCount++;
        current = current->next;

        // 如果缓冲区接近满，先写入文件
        if (logLength > 200)
        {
            res = f_write(&logFile, logBuffer, logLength, &bytesWritten);
            if (res != FR_OK)
            {
                printf("写入乘客记录失败，错误代码: %d\r\n", res);
                f_close(&logFile);
                return res;
            }
            logLength = 0;
        }
    }

    // 写入剩余的乘客记录
    if (logLength > 0)
    {
        res = f_write(&logFile, logBuffer, logLength, &bytesWritten);
        if (res != FR_OK)
        {
            printf("写入剩余乘客记录失败，错误代码: %d\r\n", res);
            f_close(&logFile);
            return res;
        }
    }

    // 写入统计信息
    logLength = sprintf(logBuffer, "\r\n统计信息:\r\n");
    logLength += sprintf(logBuffer + logLength, "总记录数: %d\r\n", recordCount);
    logLength += sprintf(logBuffer + logLength, "链表乘客数: %d\r\n", getCurrentPassengerCount());
    logLength += sprintf(logBuffer + logLength, "报表乘客数: %d\r\n", report->passenger_count);

    res = f_write(&logFile, logBuffer, logLength, &bytesWritten);
    if (res != FR_OK)
    {
        printf("写入统计信息失败，错误代码: %d\r\n", res);
        f_close(&logFile);
        return res;
    }

    // 关闭文件
    f_close(&logFile);

    printf("乘客日志已成功保存到SD卡report.txt文件\r\n");
    printf("总记录数: %d\r\n", recordCount);

    return FR_OK;
}

/*********************************************************************
 * @fn      showreport
 *
 * @brief   显示并保存乘客报告到SD卡
 *
 * @return  none
 */
void showreport()
{
    printf("正在保存乘客日志到SD卡...\r\n");
    FRESULT sd_result = savePassengerLogToSD(&report, head);
    if (sd_result == FR_OK)
    {
        printf("乘客日志保存成功！\r\n");
    }
    else
    {
        printf("乘客日志保存失败，错误代码: %d\r\n", sd_result);
    }
}
