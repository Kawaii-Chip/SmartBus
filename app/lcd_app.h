#ifndef LCD
#define LCD

#include <bsp_system.h>

void lcd_proc();
char *insertTwoCharArrays(char *original, int pos1, const char *insert1, int pos2, const char *insert2);
char *insertTwoInts(char *original, int pos1, int num1, int pos2, int num2);
extern u8 lcd_mode, lcd0_flag, lcd1_flag;

void lcd_init();


#endif