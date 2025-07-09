################################################################################
# MRS Version: 1.9.2
# �Զ����ɵ��ļ�����Ҫ�༭��
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../component/diskio.c \
../component/ff.c \
../component/ffsystem.c \
../component/ffunicode.c \
../component/lcd.c \
../component/ringbuffer.c 

OBJS += \
./component/diskio.o \
./component/ff.o \
./component/ffsystem.o \
./component/ffunicode.o \
./component/lcd.o \
./component/ringbuffer.o 

C_DEPS += \
./component/diskio.d \
./component/ff.d \
./component/ffsystem.d \
./component/ffunicode.d \
./component/lcd.d \
./component/ringbuffer.d 


# Each subdirectory must supply rules for building sources it contributes
component/%.o: ../component/%.c
	@	@	riscv-none-embed-gcc -march=rv32imacxw -mabi=ilp32 -msmall-data-limit=8 -msave-restore -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -Wunused -Wuninitialized  -g -I"C:\Users\13329\Desktop\ch32v307\1\4\Debug" -I"C:\Users\13329\Desktop\ch32v307\1\4\Core" -I"C:\Users\13329\Desktop\ch32v307\1\4\User" -I"C:\Users\13329\Desktop\ch32v307\1\4\Peripheral\inc" -I"C:\Users\13329\Desktop\ch32v307\1\4\component" -I"C:\Users\13329\Desktop\ch32v307\1\4\app" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@

