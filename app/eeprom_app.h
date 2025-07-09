#include <bsp_system.h>

extern u8 data_write_flag,data_rfid_index;

void AT24C02_Init();
void EEPROM_Write(u8 *string, u8 addr, u8 len);
void EEPROM_Read(u8 *string, u8 addr, u8 len);
void data_init();
void data_write(u8 *rfid, u8 *address);
