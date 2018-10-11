################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../HAL/EF_Anemometer.c \
../HAL/EF_GPS.c \
../HAL/nrf24l01.c 

OBJS += \
./HAL/EF_Anemometer.o \
./HAL/EF_GPS.o \
./HAL/nrf24l01.o 

C_DEPS += \
./HAL/EF_Anemometer.d \
./HAL/EF_GPS.d \
./HAL/nrf24l01.d 


# Each subdirectory must supply rules for building sources it contributes
HAL/%.o: ../HAL/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -Wall -Os -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atmega32 -DF_CPU=8000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


