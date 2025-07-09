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
            printf("Disk formatting failed????��%d??\r\n", res_sd);
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
 * @brief   ���˿��ϳ���־���浽SD����report.txt�ļ���
 *
 * @param   report - �ձ���ṹ��ָ��
 * @param   head - �˿�����ͷָ��
 *
 * @return  FRESULT - �ļ��������
 */
FRESULT savePassengerLogToSD(DailyReport *report, Passenger *head)
{
    FIL logFile;
    FRESULT res;
    UINT bytesWritten;
    char logBuffer[256];
    int logLength;

    // �򿪻򴴽�report.txt�ļ�
    res = f_open(&logFile, "1:/report.txt", FA_CREATE_ALWAYS | FA_WRITE);
    if (res != FR_OK)
    {
        printf("����report.txt�ļ�ʧ�ܣ��������: %d\r\n", res);
        return res;
    }

    printf("��ʼ����˿���־��SD��...\r\n");

    // д���ļ�ͷ����Ϣ
    logLength = sprintf(logBuffer, "=== �˿��ϳ���־���� ===\r\n");
    logLength += sprintf(logBuffer + logLength, "�ܳ˿���: %d\r\n", report->passenger_count);
    logLength += sprintf(logBuffer + logLength, "����ʱ��: %s\r\n", __DATE__);
    logLength += sprintf(logBuffer + logLength, "ʱ���ʽ: Сʱ:���� (24Сʱ��)\r\n");
    logLength += sprintf(logBuffer + logLength, "================================\r\n\r\n");

    res = f_write(&logFile, logBuffer, logLength, &bytesWritten);
    if (res != FR_OK)
    {
        printf("д���ļ�ͷ��ʧ�ܣ��������: %d\r\n", res);
        f_close(&logFile);
        return res;
    }

    // д��վ�������ͳ��
    logLength = sprintf(logBuffer, "վ�������ͳ��:\r\n");
    logLength += sprintf(logBuffer + logLength, "��ʽ: [�ϳ�վ��] -> [�³�վ��]: �˿�����\r\n");

    for (int i = 0; i < STATIONS; i++)
    {
        for (int j = 0; j < STATIONS; j++)
        {
            if (report->count[i][j] > 0)
            {
                logLength += sprintf(logBuffer + logLength, "վ��%d -> վ��%d: %d��\r\n",
                                     i, j, report->count[i][j]);
            }
        }
    }
    logLength += sprintf(logBuffer + logLength, "\r\n");

    res = f_write(&logFile, logBuffer, logLength, &bytesWritten);
    if (res != FR_OK)
    {
        printf("д������ͳ��ʧ�ܣ��������: %d\r\n", res);
        f_close(&logFile);
        return res;
    }

        // д����ϸ�˿ͼ�¼��������
    logLength = sprintf(logBuffer, "��ϸ�˿ͼ�¼:\r\n");
    logLength += sprintf(logBuffer + logLength, "��ʽ: ʱ��(Сʱ:����) | �ϳ�վ�� | �³�վ��\r\n");
    logLength += sprintf(logBuffer + logLength, "----------------------------------------\r\n");
    
    Passenger *current = head;
    int recordCount = 0;
    
    while (current != NULL)
    {
        int hours = current->time / 60;
        int minutes = current->time % 60;
        logLength += sprintf(logBuffer + logLength, "%02d:%02d | վ��%d | վ��%d\r\n", 
                           hours+8, minutes, current->station, current->destination);
        recordCount++;
        current = current->next;

        // ����������ӽ�������д���ļ�
        if (logLength > 200)
        {
            res = f_write(&logFile, logBuffer, logLength, &bytesWritten);
            if (res != FR_OK)
            {
                printf("д��˿ͼ�¼ʧ�ܣ��������: %d\r\n", res);
                f_close(&logFile);
                return res;
            }
            logLength = 0;
        }
    }

    // д��ʣ��ĳ˿ͼ�¼
    if (logLength > 0)
    {
        res = f_write(&logFile, logBuffer, logLength, &bytesWritten);
        if (res != FR_OK)
        {
            printf("д��ʣ��˿ͼ�¼ʧ�ܣ��������: %d\r\n", res);
            f_close(&logFile);
            return res;
        }
    }

    // д��ͳ����Ϣ
    logLength = sprintf(logBuffer, "\r\nͳ����Ϣ:\r\n");
    logLength += sprintf(logBuffer + logLength, "�ܼ�¼��: %d\r\n", recordCount);
    logLength += sprintf(logBuffer + logLength, "����˿���: %d\r\n", getCurrentPassengerCount());
    logLength += sprintf(logBuffer + logLength, "����˿���: %d\r\n", report->passenger_count);

    res = f_write(&logFile, logBuffer, logLength, &bytesWritten);
    if (res != FR_OK)
    {
        printf("д��ͳ����Ϣʧ�ܣ��������: %d\r\n", res);
        f_close(&logFile);
        return res;
    }

    // �ر��ļ�
    f_close(&logFile);

    printf("�˿���־�ѳɹ����浽SD��report.txt�ļ�\r\n");
    printf("�ܼ�¼��: %d\r\n", recordCount);

    return FR_OK;
}

/*********************************************************************
 * @fn      showreport
 *
 * @brief   ��ʾ������˿ͱ��浽SD��
 *
 * @return  none
 */
void showreport()
{
    printf("���ڱ���˿���־��SD��...\r\n");
    FRESULT sd_result = savePassengerLogToSD(&report, head);
    if (sd_result == FR_OK)
    {
        printf("�˿���־����ɹ���\r\n");
    }
    else
    {
        printf("�˿���־����ʧ�ܣ��������: %d\r\n", sd_result);
    }
}
