#include <nfc_app.h>

u8 flag;

void USART2_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast"))); // 串口2快速中断 B10 11 9600

u8 rfid_index, rfid_temp[4], rfid_password_index;
u8 rfid[10][4] = {};
u8 address[10]={3,0,0,0,0,0,0,0,0,0};
u8 rfid_chek()
{
    u8 i;
    for (i = 0; i < rfid_password_index; ++i)
    {
        if (string_chek(rfid_temp, rfid[i], 4))
            return 1;
    }
    return 0;
}
void USART2_IRQHandler(void)
{
    u8 temp;
    if (USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
    {
        temp = USART_ReceiveData(USART2);
        switch (rfid_index)
        {
        case 0:
            if (temp == 0x04)
                rfid_index++;
            break;
        case 1:
            if (temp == 0x0c)
                rfid_index++;
            else
                rfid_index = 0;
            break;
        case 2:
            if (temp == 0x02)
                rfid_index++;
            else
                rfid_index = 0;
            break;
        case 3:
            if (temp == 0x30)
                rfid_index++;
            else
                rfid_index = 0;
            break;
        case 4:
            if (temp == 0x00)
                rfid_index++;
            else
                rfid_index = 0;
            break;
        case 5:
            if (temp == 0x04)
                rfid_index++;
            else
                rfid_index = 0;
            break;
        case 6:
            if (temp == 0x00)
                rfid_index++;
            else
                rfid_index = 0;
            break;
        case 7:
            rfid_temp[0] = temp;
            rfid_index++;

            break;
        case 8:
            rfid_temp[1] = temp;
            rfid_index++;

            break;
        case 9:
            rfid_temp[2] = temp;
            rfid_index++;

            break;
        case 10:
            rfid_temp[3] = temp;
            rfid_index = 0;
            if(flag==2)
                flag = 3;
            else
            flag = 1;
            
            break;
        }
    }
    USART_ClearITPendingBit(USART2, USART_IT_RXNE);
}

// 模拟乘客上车
void simulate_passenger_boarding()
{
    for (u8 i = 0; i < rfid_password_index; ++i)
    {
        if (string_chek(rfid_temp, rfid[i], 4))
        {
            u8 time[3];
            RTC_GetTime(time);
            int current_time = time[0] * 60 + time[1] + (time[2] >= 30 ? 1 : 0);
            processBoarding(current_time, address[i], dropoffs);
            return;
        }
    }
    printf("未能识别通勤卡\r\n");
    audio_play(61);
}

void init()
{
    flag = 2;
}

void add_card()
{
    string_copy(rfid_temp, rfid[rfid_password_index], 4);
    printf("添加成功，目的地%d\r\n", address[rfid_password_index]);
    rfid_password_index++;
    flag = 0;
    audio_play(62);
    u8 i = 30 + address[rfid_password_index-1];
    printf("i:%d\r\n", i);

    audio_play(i);
}

void nfc_proc()
{
    if (flag==1)
    {
        flag = 0;
        simulate_passenger_boarding();
        next_station = findNextStation(station_requests, dropoffs);
    }
    else if(flag==3)
    {
        flag = 0;
        add_card();
    }
}


