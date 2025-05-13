################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Hardware/port/Inc/oled.c 

C_DEPS += \
./Hardware/port/Inc/oled.d 

OBJS += \
./Hardware/port/Inc/oled.o 


# Each subdirectory must supply rules for building sources it contributes
Hardware/port/Inc/%.o Hardware/port/Inc/%.su Hardware/port/Inc/%.cyclo: ../Hardware/port/Inc/%.c Hardware/port/Inc/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -DUSE_HAL_DRIVER -DSTM32G474xx -c -I../Core/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32G4xx/Include -I../Drivers/CMSIS/Include -I"../Port/Inc" -I"../App/algorithim/Inc" -I"../App/filiter/Inc" -I"../Hardware/hal/Inc" -I"../Hardware/port/Inc" -I../Segger/SystemView -I../Segger/debug -I../Middlewares/ST/ARM/DSP/Inc -O3 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Hardware-2f-port-2f-Inc

clean-Hardware-2f-port-2f-Inc:
	-$(RM) ./Hardware/port/Inc/oled.cyclo ./Hardware/port/Inc/oled.d ./Hardware/port/Inc/oled.o ./Hardware/port/Inc/oled.su

.PHONY: clean-Hardware-2f-port-2f-Inc

