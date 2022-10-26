################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../bsp/Src/ds1307.c \
../bsp/Src/lcd.c 

OBJS += \
./bsp/Src/ds1307.o \
./bsp/Src/lcd.o 

C_DEPS += \
./bsp/Src/ds1307.d \
./bsp/Src/lcd.d 


# Each subdirectory must supply rules for building sources it contributes
bsp/Src/%.o bsp/Src/%.su: ../bsp/Src/%.c bsp/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DSTM32 -DSTM32F407G_DISC1 -DSTM32F4 -DSTM32F407VGTx -c -I../Inc -I"C:/Users/egbra/Desktop/MasteringMCU/MCU1/stm32f4xx_drivers/bsp/Inc" -I"C:/Users/egbra/Desktop/MasteringMCU/MCU1/stm32f4xx_drivers/drivers/Inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-bsp-2f-Src

clean-bsp-2f-Src:
	-$(RM) ./bsp/Src/ds1307.d ./bsp/Src/ds1307.o ./bsp/Src/ds1307.su ./bsp/Src/lcd.d ./bsp/Src/lcd.o ./bsp/Src/lcd.su

.PHONY: clean-bsp-2f-Src

