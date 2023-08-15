################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Middlewares/ST/usbx/common/usbx_network/src/ux_network_driver.c 

OBJS += \
./Middlewares/ST/usbx/common/usbx_network/src/ux_network_driver.o 

C_DEPS += \
./Middlewares/ST/usbx/common/usbx_network/src/ux_network_driver.d 


# Each subdirectory must supply rules for building sources it contributes
Middlewares/ST/usbx/common/usbx_network/src/%.o Middlewares/ST/usbx/common/usbx_network/src/%.su Middlewares/ST/usbx/common/usbx_network/src/%.cyclo: ../Middlewares/ST/usbx/common/usbx_network/src/%.c Middlewares/ST/usbx/common/usbx_network/src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0plus -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32G0B1xx -DTX_INCLUDE_USER_DEFINE_FILE -DUX_INCLUDE_USER_DEFINE_FILE -DNX_INCLUDE_USER_DEFINE_FILE -DUSE_FULL_LL_DRIVER -DUSBPD_PORT_COUNT=1 -D_SNK -DUSBPDCORE_LIB_PD3_CONFIG_MINSNK -c -I../Core/Inc -I../Drivers/STM32G0xx_HAL_Driver/Inc -I../Drivers/STM32G0xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32G0xx/Include -I../Drivers/CMSIS/Include -I../AZURE_RTOS/App -I../USBX/App -I../USBX/Target -I../NetXDuo/App -I../Middlewares/ST/usbx/common/usbx_device_classes/inc/ -I../Middlewares/ST/usbx/common/core/inc/ -I../Middlewares/ST/usbx/ports/generic/inc/ -I../Middlewares/ST/usbx/common/usbx_network/inc/ -I../Middlewares/ST/usbx/common/usbx_stm32_device_controllers/ -I../Middlewares/ST/netxduo/common/inc/ -I../Middlewares/ST/netxduo/ports/cortex_m0/gnu/inc/ -I../Middlewares/ST/threadx/common/inc/ -I../Middlewares/ST/threadx/ports/cortex_m0/gnu/inc/ -I../USBPD/App -I../USBPD/Target -I../Middlewares/ST/STM32_USBPD_Library/Core/inc -I../Middlewares/ST/STM32_USBPD_Library/Devices/STM32G0XX/inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Middlewares-2f-ST-2f-usbx-2f-common-2f-usbx_network-2f-src

clean-Middlewares-2f-ST-2f-usbx-2f-common-2f-usbx_network-2f-src:
	-$(RM) ./Middlewares/ST/usbx/common/usbx_network/src/ux_network_driver.cyclo ./Middlewares/ST/usbx/common/usbx_network/src/ux_network_driver.d ./Middlewares/ST/usbx/common/usbx_network/src/ux_network_driver.o ./Middlewares/ST/usbx/common/usbx_network/src/ux_network_driver.su

.PHONY: clean-Middlewares-2f-ST-2f-usbx-2f-common-2f-usbx_network-2f-src

