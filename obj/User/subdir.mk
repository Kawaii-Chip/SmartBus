################################################################################
# MRS Version: 1.9.2
# 自动生成的文件。不要编辑！
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../User/ch32v30x_it.c \
../User/data.c \
../User/gpio.c \
../User/main.c \
../User/sdio.c \
../User/system_ch32v30x.c \
../User/systick.c \
../User/uart.c 

OBJS += \
./User/ch32v30x_it.o \
./User/data.o \
./User/gpio.o \
./User/main.o \
./User/sdio.o \
./User/system_ch32v30x.o \
./User/systick.o \
./User/uart.o 

C_DEPS += \
./User/ch32v30x_it.d \
./User/data.d \
./User/gpio.d \
./User/main.d \
./User/sdio.d \
./User/system_ch32v30x.d \
./User/systick.d \
./User/uart.d 


# Each subdirectory must supply rules for building sources it contributes
User/%.o: ../User/%.c
	@	@	riscv-none-embed-gcc -march=rv32imacxw -mabi=ilp32 -msmall-data-limit=8 -msave-restore -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -Wunused -Wuninitialized  -g -I"C:\Users\13329\Desktop\ch32v307\1\4\Debug" -I"C:\Users\13329\Desktop\ch32v307\1\4\Core" -I"C:\Users\13329\Desktop\ch32v307\1\4\User" -I"C:\Users\13329\Desktop\ch32v307\1\4\Peripheral\inc" -I"C:\Users\13329\Desktop\ch32v307\1\4\app" -I"C:\Users\13329\Desktop\ch32v307\1\4\component" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@

