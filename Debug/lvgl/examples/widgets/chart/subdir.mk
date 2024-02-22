################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../lvgl/examples/widgets/chart/lv_example_chart_1.c \
../lvgl/examples/widgets/chart/lv_example_chart_2.c \
../lvgl/examples/widgets/chart/lv_example_chart_3.c \
../lvgl/examples/widgets/chart/lv_example_chart_4.c \
../lvgl/examples/widgets/chart/lv_example_chart_5.c \
../lvgl/examples/widgets/chart/lv_example_chart_6.c \
../lvgl/examples/widgets/chart/lv_example_chart_7.c \
../lvgl/examples/widgets/chart/lv_example_chart_8.c \
../lvgl/examples/widgets/chart/lv_example_chart_9.c 

C_DEPS += \
./lvgl/examples/widgets/chart/lv_example_chart_1.d \
./lvgl/examples/widgets/chart/lv_example_chart_2.d \
./lvgl/examples/widgets/chart/lv_example_chart_3.d \
./lvgl/examples/widgets/chart/lv_example_chart_4.d \
./lvgl/examples/widgets/chart/lv_example_chart_5.d \
./lvgl/examples/widgets/chart/lv_example_chart_6.d \
./lvgl/examples/widgets/chart/lv_example_chart_7.d \
./lvgl/examples/widgets/chart/lv_example_chart_8.d \
./lvgl/examples/widgets/chart/lv_example_chart_9.d 

OBJS += \
./lvgl/examples/widgets/chart/lv_example_chart_1.o \
./lvgl/examples/widgets/chart/lv_example_chart_2.o \
./lvgl/examples/widgets/chart/lv_example_chart_3.o \
./lvgl/examples/widgets/chart/lv_example_chart_4.o \
./lvgl/examples/widgets/chart/lv_example_chart_5.o \
./lvgl/examples/widgets/chart/lv_example_chart_6.o \
./lvgl/examples/widgets/chart/lv_example_chart_7.o \
./lvgl/examples/widgets/chart/lv_example_chart_8.o \
./lvgl/examples/widgets/chart/lv_example_chart_9.o 


# Each subdirectory must supply rules for building sources it contributes
lvgl/examples/widgets/chart/%.o: ../lvgl/examples/widgets/chart/%.c lvgl/examples/widgets/chart/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32H735xx -c -I../Core/Inc -I../display_lvgl -I../CustomEdh/ETH -I../CustomEdh/Tasklvgl -I../CustomEdh/TaskCLI -I../CustomEdh/ADF4372 -I../CustomEdh/FRAM -I../CustomEdh/Utility -I../CustomEdh/CLI -I../CustomEdh/Init -I../CustomEdh/AD5592R -I../CustomEdh/GSL1680F -I../CustomEdh/Taskctrl -I../CustomEdh/ExtFlash -I../CustomEdh/TaskForTest -I"D:/EDH/Bitono/lvgl_test_work_space/lvgl_test" -I../LWIP/App -I../LWIP/Target -I../Middlewares/Third_Party/LwIP/src/include -I../Middlewares/Third_Party/LwIP/system -I../Drivers/STM32H7xx_HAL_Driver/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Drivers/BSP/Components/lan8742 -I../Middlewares/Third_Party/LwIP/src/include/netif/ppp -I../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../Middlewares/Third_Party/LwIP/src/include/lwip -I../Middlewares/Third_Party/LwIP/src/include/lwip/apps -I../Middlewares/Third_Party/LwIP/src/include/lwip/priv -I../Middlewares/Third_Party/LwIP/src/include/lwip/prot -I../Middlewares/Third_Party/LwIP/src/include/netif -I../Middlewares/Third_Party/LwIP/src/include/compat/posix -I../Middlewares/Third_Party/LwIP/src/include/compat/posix/arpa -I../Middlewares/Third_Party/LwIP/src/include/compat/posix/net -I../Middlewares/Third_Party/LwIP/src/include/compat/posix/sys -I../Middlewares/Third_Party/LwIP/src/include/compat/stdc -I../Middlewares/Third_Party/LwIP/system/arch -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-lvgl-2f-examples-2f-widgets-2f-chart

clean-lvgl-2f-examples-2f-widgets-2f-chart:
	-$(RM) ./lvgl/examples/widgets/chart/lv_example_chart_1.d ./lvgl/examples/widgets/chart/lv_example_chart_1.o ./lvgl/examples/widgets/chart/lv_example_chart_2.d ./lvgl/examples/widgets/chart/lv_example_chart_2.o ./lvgl/examples/widgets/chart/lv_example_chart_3.d ./lvgl/examples/widgets/chart/lv_example_chart_3.o ./lvgl/examples/widgets/chart/lv_example_chart_4.d ./lvgl/examples/widgets/chart/lv_example_chart_4.o ./lvgl/examples/widgets/chart/lv_example_chart_5.d ./lvgl/examples/widgets/chart/lv_example_chart_5.o ./lvgl/examples/widgets/chart/lv_example_chart_6.d ./lvgl/examples/widgets/chart/lv_example_chart_6.o ./lvgl/examples/widgets/chart/lv_example_chart_7.d ./lvgl/examples/widgets/chart/lv_example_chart_7.o ./lvgl/examples/widgets/chart/lv_example_chart_8.d ./lvgl/examples/widgets/chart/lv_example_chart_8.o ./lvgl/examples/widgets/chart/lv_example_chart_9.d ./lvgl/examples/widgets/chart/lv_example_chart_9.o

.PHONY: clean-lvgl-2f-examples-2f-widgets-2f-chart

