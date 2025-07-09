################################################################################
# MRS Version: 1.9.2
# 自动生成的文件。不要编辑！
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../app/audio_app.c \
../app/eeprom_app.c \
../app/gps_app.c \
../app/key_app.c \
../app/lcd_app.c \
../app/lcd_test.c \
../app/nfc_app.c \
../app/onenet_app.c \
../app/onewire_app.c \
../app/page_app.c \
../app/rtc_app.c \
../app/scheduler_app.c \
../app/sd_app.c 

OBJS += \
./app/audio_app.o \
./app/eeprom_app.o \
./app/gps_app.o \
./app/key_app.o \
./app/lcd_app.o \
./app/lcd_test.o \
./app/nfc_app.o \
./app/onenet_app.o \
./app/onewire_app.o \
./app/page_app.o \
./app/rtc_app.o \
./app/scheduler_app.o \
./app/sd_app.o 

C_DEPS += \
./app/audio_app.d \
./app/eeprom_app.d \
./app/gps_app.d \
./app/key_app.d \
./app/lcd_app.d \
./app/lcd_test.d \
./app/nfc_app.d \
./app/onenet_app.d \
./app/onewire_app.d \
./app/page_app.d \
./app/rtc_app.d \
./app/scheduler_app.d \
./app/sd_app.d 


# Each subdirectory must supply rules for building sources it contributes
app/%.o: ../app/%.c
	@	@	riscv-none-embed-gcc -march=rv32imacxw -mabi=ilp32 -msmall-data-limit=8 -msave-restore -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -Wunused -Wuninitialized  -g -I"C:\Users\13329\Desktop\ch32v307\1\4\Debug" -I"C:\Users\13329\Desktop\ch32v307\1\4\Core" -I"C:\Users\13329\Desktop\ch32v307\1\4\User" -I"C:\Users\13329\Desktop\ch32v307\1\4\Peripheral\inc" -I"C:\Users\13329\Desktop\ch32v307\1\4\app" -I"C:\Users\13329\Desktop\ch32v307\1\4\component" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@

