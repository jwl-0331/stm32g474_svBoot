################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Middlewares/Solvine/Utils/src/svCLI.c \
../Middlewares/Solvine/Utils/src/svLinkBuffer.c \
../Middlewares/Solvine/Utils/src/svList.c \
../Middlewares/Solvine/Utils/src/svOvRingBuffer.c \
../Middlewares/Solvine/Utils/src/svRingBuffer.c \
../Middlewares/Solvine/Utils/src/svTaskTimer.c \
../Middlewares/Solvine/Utils/src/svUtils.c 

OBJS += \
./Middlewares/Solvine/Utils/src/svCLI.o \
./Middlewares/Solvine/Utils/src/svLinkBuffer.o \
./Middlewares/Solvine/Utils/src/svList.o \
./Middlewares/Solvine/Utils/src/svOvRingBuffer.o \
./Middlewares/Solvine/Utils/src/svRingBuffer.o \
./Middlewares/Solvine/Utils/src/svTaskTimer.o \
./Middlewares/Solvine/Utils/src/svUtils.o 

C_DEPS += \
./Middlewares/Solvine/Utils/src/svCLI.d \
./Middlewares/Solvine/Utils/src/svLinkBuffer.d \
./Middlewares/Solvine/Utils/src/svList.d \
./Middlewares/Solvine/Utils/src/svOvRingBuffer.d \
./Middlewares/Solvine/Utils/src/svRingBuffer.d \
./Middlewares/Solvine/Utils/src/svTaskTimer.d \
./Middlewares/Solvine/Utils/src/svUtils.d 


# Each subdirectory must supply rules for building sources it contributes
Middlewares/Solvine/Utils/src/%.o Middlewares/Solvine/Utils/src/%.su Middlewares/Solvine/Utils/src/%.cyclo: ../Middlewares/Solvine/Utils/src/%.c Middlewares/Solvine/Utils/src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32G474xx -c -I"C:/CubeWorspace/stm32g474_svBoot/Middlewares/Third_Party/lwrb/inc" -I"C:/CubeWorspace/stm32g474_svBoot/Middlewares/Solvine/Utils/inc" -I"C:/CubeWorspace/stm32g474_svBoot/Drivers/BSP/inc" -I../USB_Device/App -I../USB_Device/Target -I../Core/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc/Legacy -I../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc -I../Drivers/CMSIS/Device/ST/STM32G4xx/Include -I../Drivers/CMSIS/Include -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Middlewares-2f-Solvine-2f-Utils-2f-src

clean-Middlewares-2f-Solvine-2f-Utils-2f-src:
	-$(RM) ./Middlewares/Solvine/Utils/src/svCLI.cyclo ./Middlewares/Solvine/Utils/src/svCLI.d ./Middlewares/Solvine/Utils/src/svCLI.o ./Middlewares/Solvine/Utils/src/svCLI.su ./Middlewares/Solvine/Utils/src/svLinkBuffer.cyclo ./Middlewares/Solvine/Utils/src/svLinkBuffer.d ./Middlewares/Solvine/Utils/src/svLinkBuffer.o ./Middlewares/Solvine/Utils/src/svLinkBuffer.su ./Middlewares/Solvine/Utils/src/svList.cyclo ./Middlewares/Solvine/Utils/src/svList.d ./Middlewares/Solvine/Utils/src/svList.o ./Middlewares/Solvine/Utils/src/svList.su ./Middlewares/Solvine/Utils/src/svOvRingBuffer.cyclo ./Middlewares/Solvine/Utils/src/svOvRingBuffer.d ./Middlewares/Solvine/Utils/src/svOvRingBuffer.o ./Middlewares/Solvine/Utils/src/svOvRingBuffer.su ./Middlewares/Solvine/Utils/src/svRingBuffer.cyclo ./Middlewares/Solvine/Utils/src/svRingBuffer.d ./Middlewares/Solvine/Utils/src/svRingBuffer.o ./Middlewares/Solvine/Utils/src/svRingBuffer.su ./Middlewares/Solvine/Utils/src/svTaskTimer.cyclo ./Middlewares/Solvine/Utils/src/svTaskTimer.d ./Middlewares/Solvine/Utils/src/svTaskTimer.o ./Middlewares/Solvine/Utils/src/svTaskTimer.su ./Middlewares/Solvine/Utils/src/svUtils.cyclo ./Middlewares/Solvine/Utils/src/svUtils.d ./Middlewares/Solvine/Utils/src/svUtils.o ./Middlewares/Solvine/Utils/src/svUtils.su

.PHONY: clean-Middlewares-2f-Solvine-2f-Utils-2f-src

