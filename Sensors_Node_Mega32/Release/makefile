################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include Utilities/subdir.mk
-include ServiceLayer/subdir.mk
-include MCAL/subdir.mk
-include HAL/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
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
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
LSS += \
Sensors_Node_Mega32.lss \

FLASH_IMAGE += \
Sensors_Node_Mega32.hex \

EEPROM_IMAGE += \
Sensors_Node_Mega32.eep \

SIZEDUMMY += \
sizedummy \


# All Target
all: Sensors_Node_Mega32.elf secondary-outputs

# Tool invocations
Sensors_Node_Mega32.elf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: AVR C Linker'
	avr-gcc -Wl,-Map,Sensors_Node_Mega32.map -Wl,-gc-sections -mmcu=atmega32 -o "Sensors_Node_Mega32.elf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

Sensors_Node_Mega32.lss: Sensors_Node_Mega32.elf
	@echo 'Invoking: AVR Create Extended Listing'
	-avr-objdump -h -S Sensors_Node_Mega32.elf  >"Sensors_Node_Mega32.lss"
	@echo 'Finished building: $@'
	@echo ' '

Sensors_Node_Mega32.hex: Sensors_Node_Mega32.elf
	@echo 'Create Flash image (ihex format)'
	-avr-objcopy -R .eeprom -R .fuse -R .lock -R .signature -O ihex Sensors_Node_Mega32.elf  "Sensors_Node_Mega32.hex"
	@echo 'Finished building: $@'
	@echo ' '

Sensors_Node_Mega32.eep: Sensors_Node_Mega32.elf
	@echo 'Create eeprom image (ihex format)'
	-avr-objcopy -j .eeprom --no-change-warnings --change-section-lma .eeprom=0 -O ihex Sensors_Node_Mega32.elf  "Sensors_Node_Mega32.eep"
	@echo 'Finished building: $@'
	@echo ' '

sizedummy: Sensors_Node_Mega32.elf
	@echo 'Invoking: Print Size'
	-avr-size --format=avr --mcu=atmega32 Sensors_Node_Mega32.elf
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(OBJS)$(C_DEPS)$(ASM_DEPS)$(EEPROM_IMAGE)$(FLASH_IMAGE)$(ELFS)$(LSS)$(S_DEPS)$(SIZEDUMMY)$(S_UPPER_DEPS) Sensors_Node_Mega32.elf
	-@echo ' '

secondary-outputs: $(LSS) $(FLASH_IMAGE) $(EEPROM_IMAGE) $(SIZEDUMMY)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
