################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include Drivers/STM32F1xx_HAL_Driver/Src/subdir.mk
-include Core/Startup/subdir.mk
-include Core/Src/subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
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

OPTIONAL_TOOL_DEPS := \
$(wildcard ../makefile.defs) \
$(wildcard ../makefile.init) \
$(wildcard ../makefile.targets) \


BUILD_ARTIFACT_NAME := SPI-to-Ethernet
BUILD_ARTIFACT_EXTENSION := elf
BUILD_ARTIFACT_PREFIX :=
BUILD_ARTIFACT := $(BUILD_ARTIFACT_PREFIX)$(BUILD_ARTIFACT_NAME)$(if $(BUILD_ARTIFACT_EXTENSION),.$(BUILD_ARTIFACT_EXTENSION),)

# Add inputs and outputs from these tool invocations to the build variables 
EXECUTABLES += \
SPI-to-Ethernet.elf \

MAP_FILES += \
SPI-to-Ethernet.map \

SIZE_OUTPUT += \
default.size.stdout \

OBJDUMP_LIST += \
SPI-to-Ethernet.list \


# All Target
all:
	+@$(MAKE) --no-print-directory main-build && $(MAKE) --no-print-directory post-build

# Main-build Target
main-build: SPI-to-Ethernet.elf secondary-outputs

# Tool invocations
SPI-to-Ethernet.elf SPI-to-Ethernet.map: $(OBJS) $(USER_OBJS) E:\STM32Cube\SPI-to-Ethernet\STM32F103C8TX_FLASH.ld makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-gcc -o "SPI-to-Ethernet.elf" @"objects.list" $(USER_OBJS) $(LIBS) -mcpu=cortex-m3 -T"E:\STM32Cube\SPI-to-Ethernet\STM32F103C8TX_FLASH.ld" --specs=nosys.specs -Wl,-Map="SPI-to-Ethernet.map" -Wl,--gc-sections -static --specs=nano.specs -mfloat-abi=soft -mthumb -Wl,--start-group -lc -lm -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

default.size.stdout: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-size  $(EXECUTABLES)
	@echo 'Finished building: $@'
	@echo ' '

SPI-to-Ethernet.list: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-objdump -h -S $(EXECUTABLES) > "SPI-to-Ethernet.list"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) SPI-to-Ethernet.elf SPI-to-Ethernet.list SPI-to-Ethernet.map default.size.stdout
	-@echo ' '

post-build:
	arm-none-eabi-objcopy -O ihex SPI-to-Ethernet.elf SPI-to-Ethernet.hex
	-@echo ' '

secondary-outputs: $(SIZE_OUTPUT) $(OBJDUMP_LIST)

fail-specified-linker-script-missing:
	@echo 'Error: Cannot find the specified linker script. Check the linker settings in the build configuration.'
	@exit 2

warn-no-linker-script-specified:
	@echo 'Warning: No linker script specified. Check the linker settings in the build configuration.'

.PHONY: all clean dependents main-build fail-specified-linker-script-missing warn-no-linker-script-specified post-build

-include ../makefile.targets
