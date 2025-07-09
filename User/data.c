#include <data.h>
#include <sd_app.h>


// ȫ�ֱ���
int current_station = 0;
int next_station = 0;
char request_handle[2] = {0, -1}; // [�Ƿ�������, �ϳ���]
int dropoffs[STATIONS] = {0};
int station_requests[STATIONS] = {0};

DailyReport report;
Passenger *head = NULL;                                   // �˿�����ͷָ��
Passenger *pending_destinations[MAX_PASSENGERS] = {NULL}; // ������Ŀ�ĵ�����

// ��ʼ���ձ���
void initReport(DailyReport *report)
{
    for (int i = 0; i < STATIONS; i++)
        for (int j = 0; j < STATIONS; j++)
            report->count[i][j] = 0;
    report->passenger_count = 0;
    request_handle[0] = 0;
}

// �����³˿ͽڵ�
Passenger *createPassenger(int station, int time, int destination)
{
    Passenger *newNode = (Passenger *)malloc(sizeof(Passenger));
    newNode->station = station;
    newNode->time = time;
    newNode->destination = destination;
    newNode->next = NULL;
    return newNode;
}



// ����˿��ϳ�������汾��
bool processBoarding(int current_time, int destination, int *dropoffs)
{
    int station = current_station;
    

    // �����³˿ͽڵ㲢��ӵ�����ͷ��
    Passenger *newPassenger = createPassenger(station, current_time, destination);
    newPassenger->next = head;
    head = newPassenger;

    // ���±�������
    report.count[station][destination]++;
    report.times[report.passenger_count] = current_time;
    report.destinations[report.passenger_count++] = destination;

    // �����³�����
    dropoffs[destination]++;

    // ����������
    request_handle[0] = 0;
    request_handle[1] = -1;
    audio_play(30);
    u8 i = 30 + destination;
    audio_play(i);
    printf("վ��%d�г˿��ϳ���Ŀ�ĵ�%d\r\n",
           station, destination);
    return true;
}

// ����˿��³�������汾��
int processDropoffs(int current_station, int *dropoffs)
{
    int dropped = 0;
    Passenger *prev = NULL;
    Passenger *curr = head;

    while (curr != NULL)
    {
        if (curr->destination == current_station)
        {
            // ����ͷ�ڵ����
            if (prev == NULL)
            {
                head = curr->next;
            }
            else
            {
                prev->next = curr->next;
            }
            // �ͷ��ڴ�
            free(curr);
            dropped++;
            // �����ӵ�ǰλ�ò��ң������������³���
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
        dropoffs[current_station] = 0; // �����³�����
        printf(" վ��%d%d���˿��³�\r\n",
               current_station, dropped);
    }
    return dropped;
}

// ������һ���������վ��
int findNextStation(int *station_requests, int *dropoffs)
{
    for (int i = 1; i < STATIONS; i++)
    {
        int candidate = (current_station + i) % STATIONS;
        if (station_requests[candidate] > 0 ||
            dropoffs[candidate] > 0)
        {
            
            
            // printf("��һվ��%d\r\n", candidate);
            return candidate;
        }
    }
    
    printf("������˳����ʻ\r\n");
    return (current_station + 1) % STATIONS; // ������ʱ��˳����ʻ
}

// �ͷ������ڴ�
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


// ���ɱ�������ԭ���߼���
void generateReport(DailyReport *report)
{

    // ����ͳ��
    // for (int i = 0; i < STATIONS; i++)
    //     for (int j = 0; j < STATIONS; j++)
    //         if (report->count[i][j] > 0)
    //         {
    //             printf("AT+MQTTPUB=0,\"$sys/mmMJx639W9/ch32/thing/property/post\",\"{\\\"id\\\":\\\"123\\\"\\,\\\"params\\\":{\\\"data\\\":{\\\"value\\\":[1\\,%d\\,%d\\,%d]}}}\",0,0\r\n", i, j, report->count[i][j]);
    //             Delay_Ms(500);
    //         }

    // ʱ��ͳ��
    // int time_bins[24] = {0};
    // for (int i = 0; i < report->passenger_count; i++)
    //     time_bins[report->times[i] / 60]++;
    // for (int i = 0; i < 24; i++)
    //     if (time_bins[i] > 0)
    //     {
    //         printf("AT+MQTTPUB=0,\"$sys/mmMJx639W9/ch32/thing/property/post\",\"{\\\"id\\\":\\\"123\\\"\\,\\\"params\\\":{\\\"data\\\":{\\\"value\\\":[2\\,%d\\,%d]}}}\",0,0\r\n", i, time_bins[i]);
    //         Delay_Ms(500);
    //     }
    
    // ����˿���־��SD��
    // printf("���ڱ���˿���־��SD��...\r\n");
    // FRESULT sd_result = savePassengerLogToSD(report, head);
    // if (sd_result == FR_OK)
    // {
    //     printf("�˿���־����ɹ���\r\n");
    // }
    // else
    // {
    //     printf("�˿���־����ʧ�ܣ��������: %d\r\n", sd_result);
    // }
           
}

int getCurrentPassengerCount(void)
{
    int count = 0;
    Passenger *curr = head;

    // ��������ͳ�ƽڵ�����
    while (curr != NULL)
    {
        count++;
        curr = curr->next;
    }

    return count;
}