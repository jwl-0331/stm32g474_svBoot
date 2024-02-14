################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/BSP/src/Flash.c \
../Drivers/BSP/src/LED.c \
../Drivers/BSP/src/RTC.c \
../Drivers/BSP/src/Reset.c \
../Drivers/BSP/src/UART.c \
../Drivers/BSP/src/svDebug.c \
../Drivers/BSP/src/svPlatform.c 

OBJS += \
./Drivers/BSP/src/Flash.o \
./Drivers/BSP/src/LED.o \
./Drivers/BSP/src/RTC.o \
./Drivers/BSP/src/Reset.o \
./Drivers/BSP/src/UART.o \
./Drivers/BSP/src/svDebug.o \
./Drivers/BSP/src/svPlatform.o 

C_DEPS += \
./Drivers/BSP/src/Flash.d \
./Drivers/BSP/src/LED.d \
./Drivers/BSP/src/RTC.d \
./Drivers/BSP/src/Reset.d \
./Drivers/BSP/src/UART.d \
./Drivers/BSP/src/svDebug.d \
./Drivers/BSP/src/svPlatform.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/BSP/src/%.o Drivers/BSP/src/%.su Drivers/BSP/src/%.cyclo: ../Drivers/BSP/src/%.c Drivers/BSP/src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32G474xx -c -I"C:/CubeWorspace/stm32g474_svBoot/Middlewares/Third_Party/lwrb/inc" -I"C:/CubeWorspace/stm32g474_svBoot/Middlewares/Solvine/Utils/inc" -I"C:/CubeWorspace/stm32g474_svBoot/Drivers/BSP/inc" -I../USB_Device/App -I../USB_Device/Target -I../Core/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc/Legacy -I../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc -I../Drivers/CMSIS/Device/ST/STM32G4xx/Include -I../Drivers/CMSIS/Include -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Drivers-2f-BSP-2f-src

clean-Drivers-2f-BSP-2f-src:
	-$(RM) ./Drivers/BSP/src/Flash.cyclo ./Drivers/BSP/src/Flash.d ./Drivers/BSP/src/Flash.o ./Drivers/BSP/src/Flash.su ./Drivers/BSP/src/LED.cyclo ./Drivers/BSP/src/LED.d ./Drivers/BSP/src/LED.o ./Drivers/BSP/src/LED.su ./Drivers/BSP/src/RTC.cyclo ./Drivers/BSP/src/RTC.d ./Drivers/BSP/src/RTC.o ./Drivers/BSP/src/RTC.su ./Drivers/BSP/src/Reset.cyclo ./Drivers/BSP/src/Reset.d ./Drivers/BSP/src/Reset.o ./Drivers/BSP/src/Reset.su ./Drivers/BSP/src/UART.cyclo ./Drivers/BSP/src/UART.d ./Drivers/BSP/src/UART.o ./Drivers/BSP/src/UART.su ./Drivers/BSP/src/svDebug.cyclo ./Drivers/BSP/src/svDebug.d ./Drivers/BSP/src/svDebug.o ./Drivers/BSP/src/svDebug.su ./Drivers/BSP/src/svPlatform.cyclo ./Drivers/BSP/src/svPlatform.d ./Drivers/BSP/src/svPlatform.o ./Drivers/BSP/src/svPlatform.su

.PHONY: clean-Drivers-2f-BSP-2f-src

