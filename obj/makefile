################################################################################
# MRS Version: 1.9.2
# 自动生成的文件。不要编辑！
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include component/subdir.mk
-include app/subdir.mk
-include User/subdir.mk
-include Startup/subdir.mk
-include Peripheral/src/subdir.mk
-include Debug/subdir.mk
-include Core/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(ASM_UPPER_DEPS)),)
-include $(ASM_UPPER_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
SECONDARY_FLASH += \
4.hex \

SECONDARY_LIST += \
4.lst \

SECONDARY_SIZE += \
4.siz \


# 所有目标
all: 4.elf secondary-outputs

# 工具调用
4.elf: $(OBJS) $(USER_OBJS)
	@	@	riscv-none-embed-gcc -march=rv32imacxw -mabi=ilp32 -msmall-data-limit=8 -msave-restore -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -Wunused -Wuninitialized  -g -T "C:\Users\13329\Desktop\ch32v307\1\4\Ld\Link.ld" -nostartfiles -Xlinker --gc-sections -Wl,-Map,"4.map" --specs=nano.specs -u _printf_float -u _scanf_float --specs=nosys.specs -o "4.elf" $(OBJS) $(USER_OBJS) $(LIBS)
	@	@
4.hex: 4.elf
	@	riscv-none-embed-objcopy -O ihex "4.elf"  "4.hex"
	@	@
4.lst: 4.elf
	@	riscv-none-embed-objdump --all-headers --demangle --disassemble -M xw "4.elf" > "4.lst"
	@	@
4.siz: 4.elf
	@	riscv-none-embed-size --format=berkeley "4.elf"
	@	@
# 其他目标
clean:
	-$(RM) $(ASM_UPPER_DEPS)$(OBJS)$(SECONDARY_FLASH)$(SECONDARY_LIST)$(SECONDARY_SIZE)$(ASM_DEPS)$(S_DEPS)$(S_UPPER_DEPS)$(C_DEPS) 4.elf
	-@
secondary-outputs: $(SECONDARY_FLASH) $(SECONDARY_LIST) $(SECONDARY_SIZE)

.PHONY: all clean dependents

-include ../makefile.targets
