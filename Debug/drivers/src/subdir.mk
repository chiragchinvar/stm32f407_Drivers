################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../drivers/src/stm32f407_gpio.c \
../drivers/src/stm32f407_i2c.c \
../drivers/src/stm32f407_spi.c 

OBJS += \
./drivers/src/stm32f407_gpio.o \
./drivers/src/stm32f407_i2c.o \
./drivers/src/stm32f407_spi.o 

C_DEPS += \
./drivers/src/stm32f407_gpio.d \
./drivers/src/stm32f407_i2c.d \
./drivers/src/stm32f407_spi.d 


# Each subdirectory must supply rules for building sources it contributes
drivers/src/stm32f407_gpio.o: ../drivers/src/stm32f407_gpio.c drivers/src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DSTM32 -DSTM32F407G_DISC1 -DSTM32F4 -DSTM32F407VGTx -c -I"D:/Courses/Embedded Systems Programming on ARM Cortex-M3M4 Processor/my project/stm32f407_disc_drivers/drivers/inc" -I../Inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"drivers/src/stm32f407_gpio.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
drivers/src/stm32f407_i2c.o: ../drivers/src/stm32f407_i2c.c drivers/src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DSTM32 -DSTM32F407G_DISC1 -DSTM32F4 -DSTM32F407VGTx -c -I"D:/Courses/Embedded Systems Programming on ARM Cortex-M3M4 Processor/my project/stm32f407_disc_drivers/drivers/inc" -I../Inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"drivers/src/stm32f407_i2c.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
drivers/src/stm32f407_spi.o: ../drivers/src/stm32f407_spi.c drivers/src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DSTM32 -DSTM32F407G_DISC1 -DSTM32F4 -DSTM32F407VGTx -c -I"D:/Courses/Embedded Systems Programming on ARM Cortex-M3M4 Processor/my project/stm32f407_disc_drivers/drivers/inc" -I../Inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"drivers/src/stm32f407_spi.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

