################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Middlewares/ST/usbx/common/usbx_device_classes/src/ux_device_class_hid_activate.c \
../Middlewares/ST/usbx/common/usbx_device_classes/src/ux_device_class_hid_control_request.c \
../Middlewares/ST/usbx/common/usbx_device_classes/src/ux_device_class_hid_deactivate.c \
../Middlewares/ST/usbx/common/usbx_device_classes/src/ux_device_class_hid_descriptor_send.c \
../Middlewares/ST/usbx/common/usbx_device_classes/src/ux_device_class_hid_entry.c \
../Middlewares/ST/usbx/common/usbx_device_classes/src/ux_device_class_hid_event_get.c \
../Middlewares/ST/usbx/common/usbx_device_classes/src/ux_device_class_hid_event_set.c \
../Middlewares/ST/usbx/common/usbx_device_classes/src/ux_device_class_hid_initialize.c \
../Middlewares/ST/usbx/common/usbx_device_classes/src/ux_device_class_hid_interrupt_thread.c \
../Middlewares/ST/usbx/common/usbx_device_classes/src/ux_device_class_hid_report_get.c \
../Middlewares/ST/usbx/common/usbx_device_classes/src/ux_device_class_hid_report_set.c \
../Middlewares/ST/usbx/common/usbx_device_classes/src/ux_device_class_hid_uninitialize.c 

OBJS += \
./Middlewares/ST/usbx/common/usbx_device_classes/src/ux_device_class_hid_activate.o \
./Middlewares/ST/usbx/common/usbx_device_classes/src/ux_device_class_hid_control_request.o \
./Middlewares/ST/usbx/common/usbx_device_classes/src/ux_device_class_hid_deactivate.o \
./Middlewares/ST/usbx/common/usbx_device_classes/src/ux_device_class_hid_descriptor_send.o \
./Middlewares/ST/usbx/common/usbx_device_classes/src/ux_device_class_hid_entry.o \
./Middlewares/ST/usbx/common/usbx_device_classes/src/ux_device_class_hid_event_get.o \
./Middlewares/ST/usbx/common/usbx_device_classes/src/ux_device_class_hid_event_set.o \
./Middlewares/ST/usbx/common/usbx_device_classes/src/ux_device_class_hid_initialize.o \
./Middlewares/ST/usbx/common/usbx_device_classes/src/ux_device_class_hid_interrupt_thread.o \
./Middlewares/ST/usbx/common/usbx_device_classes/src/ux_device_class_hid_report_get.o \
./Middlewares/ST/usbx/common/usbx_device_classes/src/ux_device_class_hid_report_set.o \
./Middlewares/ST/usbx/common/usbx_device_classes/src/ux_device_class_hid_uninitialize.o 

C_DEPS += \
./Middlewares/ST/usbx/common/usbx_device_classes/src/ux_device_class_hid_activate.d \
./Middlewares/ST/usbx/common/usbx_device_classes/src/ux_device_class_hid_control_request.d \
./Middlewares/ST/usbx/common/usbx_device_classes/src/ux_device_class_hid_deactivate.d \
./Middlewares/ST/usbx/common/usbx_device_classes/src/ux_device_class_hid_descriptor_send.d \
./Middlewares/ST/usbx/common/usbx_device_classes/src/ux_device_class_hid_entry.d \
./Middlewares/ST/usbx/common/usbx_device_classes/src/ux_device_class_hid_event_get.d \
./Middlewares/ST/usbx/common/usbx_device_classes/src/ux_device_class_hid_event_set.d \
./Middlewares/ST/usbx/common/usbx_device_classes/src/ux_device_class_hid_initialize.d \
./Middlewares/ST/usbx/common/usbx_device_classes/src/ux_device_class_hid_interrupt_thread.d \
./Middlewares/ST/usbx/common/usbx_device_classes/src/ux_device_class_hid_report_get.d \
./Middlewares/ST/usbx/common/usbx_device_classes/src/ux_device_class_hid_report_set.d \
./Middlewares/ST/usbx/common/usbx_device_classes/src/ux_device_class_hid_uninitialize.d 


# Each subdirectory must supply rules for building sources it contributes
Middlewares/ST/usbx/common/usbx_device_classes/src/%.o Middlewares/ST/usbx/common/usbx_device_classes/src/%.su Middlewares/ST/usbx/common/usbx_device_classes/src/%.cyclo: ../Middlewares/ST/usbx/common/usbx_device_classes/src/%.c Middlewares/ST/usbx/common/usbx_device_classes/src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0plus -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32G0B1xx -DTX_INCLUDE_USER_DEFINE_FILE -DUX_INCLUDE_USER_DEFINE_FILE -DNX_INCLUDE_USER_DEFINE_FILE -DUSE_FULL_LL_DRIVER -DUSBPD_PORT_COUNT=1 -D_SNK -DUSBPDCORE_LIB_PD3_CONFIG_MINSNK -c -I../Core/Inc -I../Drivers/STM32G0xx_HAL_Driver/Inc -I../Drivers/STM32G0xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32G0xx/Include -I../Drivers/CMSIS/Include -I../AZURE_RTOS/App -I../USBX/App -I../USBX/Target -I../NetXDuo/App -I../Middlewares/ST/usbx/common/usbx_device_classes/inc/ -I../Middlewares/ST/usbx/common/core/inc/ -I../Middlewares/ST/usbx/ports/generic/inc/ -I../Middlewares/ST/usbx/common/usbx_network/inc/ -I../Middlewares/ST/usbx/common/usbx_stm32_device_controllers/ -I../Middlewares/ST/netxduo/common/inc/ -I../Middlewares/ST/netxduo/ports/cortex_m0/gnu/inc/ -I../Middlewares/ST/threadx/common/inc/ -I../Middlewares/ST/threadx/ports/cortex_m0/gnu/inc/ -I../USBPD/App -I../USBPD/Target -I../Middlewares/ST/STM32_USBPD_Library/Core/inc -I../Middlewares/ST/STM32_USBPD_Library/Devices/STM32G0XX/inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Middlewares-2f-ST-2f-usbx-2f-common-2f-usbx_device_classes-2f-src

clean-Middlewares-2f-ST-2f-usbx-2f-common-2f-usbx_device_classes-2f-src:
	-$(RM) ./Middlewares/ST/usbx/common/usbx_device_classes/src/ux_device_class_hid_activate.cyclo ./Middlewares/ST/usbx/common/usbx_device_classes/src/ux_device_class_hid_activate.d ./Middlewares/ST/usbx/common/usbx_device_classes/src/ux_device_class_hid_activate.o ./Middlewares/ST/usbx/common/usbx_device_classes/src/ux_device_class_hid_activate.su ./Middlewares/ST/usbx/common/usbx_device_classes/src/ux_device_class_hid_control_request.cyclo ./Middlewares/ST/usbx/common/usbx_device_classes/src/ux_device_class_hid_control_request.d ./Middlewares/ST/usbx/common/usbx_device_classes/src/ux_device_class_hid_control_request.o ./Middlewares/ST/usbx/common/usbx_device_classes/src/ux_device_class_hid_control_request.su ./Middlewares/ST/usbx/common/usbx_device_classes/src/ux_device_class_hid_deactivate.cyclo ./Middlewares/ST/usbx/common/usbx_device_classes/src/ux_device_class_hid_deactivate.d ./Middlewares/ST/usbx/common/usbx_device_classes/src/ux_device_class_hid_deactivate.o ./Middlewares/ST/usbx/common/usbx_device_classes/src/ux_device_class_hid_deactivate.su ./Middlewares/ST/usbx/common/usbx_device_classes/src/ux_device_class_hid_descriptor_send.cyclo ./Middlewares/ST/usbx/common/usbx_device_classes/src/ux_device_class_hid_descriptor_send.d ./Middlewares/ST/usbx/common/usbx_device_classes/src/ux_device_class_hid_descriptor_send.o ./Middlewares/ST/usbx/common/usbx_device_classes/src/ux_device_class_hid_descriptor_send.su ./Middlewares/ST/usbx/common/usbx_device_classes/src/ux_device_class_hid_entry.cyclo ./Middlewares/ST/usbx/common/usbx_device_classes/src/ux_device_class_hid_entry.d ./Middlewares/ST/usbx/common/usbx_device_classes/src/ux_device_class_hid_entry.o ./Middlewares/ST/usbx/common/usbx_device_classes/src/ux_device_class_hid_entry.su ./Middlewares/ST/usbx/common/usbx_device_classes/src/ux_device_class_hid_event_get.cyclo ./Middlewares/ST/usbx/common/usbx_device_classes/src/ux_device_class_hid_event_get.d ./Middlewares/ST/usbx/common/usbx_device_classes/src/ux_device_class_hid_event_get.o ./Middlewares/ST/usbx/common/usbx_device_classes/src/ux_device_class_hid_event_get.su ./Middlewares/ST/usbx/common/usbx_device_classes/src/ux_device_class_hid_event_set.cyclo ./Middlewares/ST/usbx/common/usbx_device_classes/src/ux_device_class_hid_event_set.d ./Middlewares/ST/usbx/common/usbx_device_classes/src/ux_device_class_hid_event_set.o ./Middlewares/ST/usbx/common/usbx_device_classes/src/ux_device_class_hid_event_set.su ./Middlewares/ST/usbx/common/usbx_device_classes/src/ux_device_class_hid_initialize.cyclo ./Middlewares/ST/usbx/common/usbx_device_classes/src/ux_device_class_hid_initialize.d ./Middlewares/ST/usbx/common/usbx_device_classes/src/ux_device_class_hid_initialize.o ./Middlewares/ST/usbx/common/usbx_device_classes/src/ux_device_class_hid_initialize.su ./Middlewares/ST/usbx/common/usbx_device_classes/src/ux_device_class_hid_interrupt_thread.cyclo ./Middlewares/ST/usbx/common/usbx_device_classes/src/ux_device_class_hid_interrupt_thread.d ./Middlewares/ST/usbx/common/usbx_device_classes/src/ux_device_class_hid_interrupt_thread.o ./Middlewares/ST/usbx/common/usbx_device_classes/src/ux_device_class_hid_interrupt_thread.su ./Middlewares/ST/usbx/common/usbx_device_classes/src/ux_device_class_hid_report_get.cyclo ./Middlewares/ST/usbx/common/usbx_device_classes/src/ux_device_class_hid_report_get.d ./Middlewares/ST/usbx/common/usbx_device_classes/src/ux_device_class_hid_report_get.o ./Middlewares/ST/usbx/common/usbx_device_classes/src/ux_device_class_hid_report_get.su ./Middlewares/ST/usbx/common/usbx_device_classes/src/ux_device_class_hid_report_set.cyclo ./Middlewares/ST/usbx/common/usbx_device_classes/src/ux_device_class_hid_report_set.d ./Middlewares/ST/usbx/common/usbx_device_classes/src/ux_device_class_hid_report_set.o ./Middlewares/ST/usbx/common/usbx_device_classes/src/ux_device_class_hid_report_set.su ./Middlewares/ST/usbx/common/usbx_device_classes/src/ux_device_class_hid_uninitialize.cyclo ./Middlewares/ST/usbx/common/usbx_device_classes/src/ux_device_class_hid_uninitialize.d ./Middlewares/ST/usbx/common/usbx_device_classes/src/ux_device_class_hid_uninitialize.o ./Middlewares/ST/usbx/common/usbx_device_classes/src/ux_device_class_hid_uninitialize.su

.PHONY: clean-Middlewares-2f-ST-2f-usbx-2f-common-2f-usbx_device_classes-2f-src

