################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Middlewares/Third_Party/lwrb/src/lwrb.c 

OBJS += \
./Middlewares/Third_Party/lwrb/src/lwrb.o 

C_DEPS += \
./Middlewares/Third_Party/lwrb/src/lwrb.d 


# Each subdirectory must supply rules for building sources it contributes
Middlewares/Third_Party/lwrb/src/%.o Middlewares/Third_Party/lwrb/src/%.su Middlewares/Third_Party/lwrb/src/%.cyclo: ../Middlewares/Third_Party/lwrb/src/%.c Middlewares/Third_Party/lwrb/src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32G474xx -c -I"C:/CubeWorspace/stm32g474_svBoot/Middlewares/Third_Party/lwrb/inc" -I"C:/CubeWorspace/stm32g474_svBoot/Middlewares/Solvine/Utils/inc" -I"C:/CubeWorspace/stm32g474_svBoot/Drivers/BSP/inc" -I../USB_Device/App -I../USB_Device/Target -I../Core/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc/Legacy -I../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc -I../Drivers/CMSIS/Device/ST/STM32G4xx/Include -I../Drivers/CMSIS/Include -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Middlewares-2f-Third_Party-2f-lwrb-2f-src

clean-Middlewares-2f-Third_Party-2f-lwrb-2f-src:
	-$(RM) ./Middlewares/Third_Party/lwrb/src/lwrb.cyclo ./Middlewares/Third_Party/lwrb/src/lwrb.d ./Middlewares/Third_Party/lwrb/src/lwrb.o ./Middlewares/Third_Party/lwrb/src/lwrb.su

.PHONY: clean-Middlewares-2f-Third_Party-2f-lwrb-2f-src

