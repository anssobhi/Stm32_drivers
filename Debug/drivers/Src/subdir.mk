################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../drivers/Src/stm32f407xx_I2c.c \
../drivers/Src/stm32f407xx_gpio.c \
../drivers/Src/stm32f407xx_rcc.c \
../drivers/Src/stm32f407xx_spi.c \
../drivers/Src/stm32f407xx_usart.c 

OBJS += \
./drivers/Src/stm32f407xx_I2c.o \
./drivers/Src/stm32f407xx_gpio.o \
./drivers/Src/stm32f407xx_rcc.o \
./drivers/Src/stm32f407xx_spi.o \
./drivers/Src/stm32f407xx_usart.o 

C_DEPS += \
./drivers/Src/stm32f407xx_I2c.d \
./drivers/Src/stm32f407xx_gpio.d \
./drivers/Src/stm32f407xx_rcc.d \
./drivers/Src/stm32f407xx_spi.d \
./drivers/Src/stm32f407xx_usart.d 


# Each subdirectory must supply rules for building sources it contributes
drivers/Src/stm32f407xx_I2c.o: ../drivers/Src/stm32f407xx_I2c.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DSTM32 -DSTM32F407G_DISC1 -DSTM32F4 -DSTM32F407VGTx -DDEBUG -c -I"C:/Users/ans/STM32CubeIDE/workspace_1.0.2/stm32f4xx_drivers/drivers/Inc" -I"C:/Users/ans/STM32CubeIDE/workspace_1.0.2/stm32f4xx_drivers/bsp" -I../Inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"drivers/Src/stm32f407xx_I2c.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
drivers/Src/stm32f407xx_gpio.o: ../drivers/Src/stm32f407xx_gpio.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DSTM32 -DSTM32F407G_DISC1 -DSTM32F4 -DSTM32F407VGTx -DDEBUG -c -I"C:/Users/ans/STM32CubeIDE/workspace_1.0.2/stm32f4xx_drivers/drivers/Inc" -I"C:/Users/ans/STM32CubeIDE/workspace_1.0.2/stm32f4xx_drivers/bsp" -I../Inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"drivers/Src/stm32f407xx_gpio.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
drivers/Src/stm32f407xx_rcc.o: ../drivers/Src/stm32f407xx_rcc.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DSTM32 -DSTM32F407G_DISC1 -DSTM32F4 -DSTM32F407VGTx -DDEBUG -c -I"C:/Users/ans/STM32CubeIDE/workspace_1.0.2/stm32f4xx_drivers/drivers/Inc" -I"C:/Users/ans/STM32CubeIDE/workspace_1.0.2/stm32f4xx_drivers/bsp" -I../Inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"drivers/Src/stm32f407xx_rcc.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
drivers/Src/stm32f407xx_spi.o: ../drivers/Src/stm32f407xx_spi.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DSTM32 -DSTM32F407G_DISC1 -DSTM32F4 -DSTM32F407VGTx -DDEBUG -c -I"C:/Users/ans/STM32CubeIDE/workspace_1.0.2/stm32f4xx_drivers/drivers/Inc" -I"C:/Users/ans/STM32CubeIDE/workspace_1.0.2/stm32f4xx_drivers/bsp" -I../Inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"drivers/Src/stm32f407xx_spi.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
drivers/Src/stm32f407xx_usart.o: ../drivers/Src/stm32f407xx_usart.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DSTM32 -DSTM32F407G_DISC1 -DSTM32F4 -DSTM32F407VGTx -DDEBUG -c -I"C:/Users/ans/STM32CubeIDE/workspace_1.0.2/stm32f4xx_drivers/drivers/Inc" -I"C:/Users/ans/STM32CubeIDE/workspace_1.0.2/stm32f4xx_drivers/bsp" -I../Inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"drivers/Src/stm32f407xx_usart.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

