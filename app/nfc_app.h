#include <bsp_system.h>
void nfc_proc();
extern u8 flag;
extern u8 rfid[10][4], rfid_password_index; // 卡号
extern u8 address[10] ;//卡号对应的目的地
void init();