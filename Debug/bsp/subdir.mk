################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../bsp/ds1037.c \
../bsp/lcd.c 

OBJS += \
./bsp/ds1037.o \
./bsp/lcd.o 

C_DEPS += \
./bsp/ds1037.d \
./bsp/lcd.d 


# Each subdirectory must supply rules for building sources it contributes
bsp/ds1037.o: ../bsp/ds1037.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DSTM32 -DSTM32F407G_DISC1 -DSTM32F4 -DSTM32F407VGTx -DDEBUG -c -I"C:/Users/ans/STM32CubeIDE/workspace_1.0.2/stm32f4xx_drivers/drivers/Inc" -I"C:/Users/ans/STM32CubeIDE/workspace_1.0.2/stm32f4xx_drivers/bsp" -I../Inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"bsp/ds1037.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
bsp/lcd.o: ../bsp/lcd.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DSTM32 -DSTM32F407G_DISC1 -DSTM32F4 -DSTM32F407VGTx -DDEBUG -c -I"C:/Users/ans/STM32CubeIDE/workspace_1.0.2/stm32f4xx_drivers/drivers/Inc" -I"C:/Users/ans/STM32CubeIDE/workspace_1.0.2/stm32f4xx_drivers/bsp" -I../Inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"bsp/lcd.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

