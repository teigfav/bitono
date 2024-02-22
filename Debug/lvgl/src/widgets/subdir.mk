################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../lvgl/src/widgets/lv_arc.c \
../lvgl/src/widgets/lv_bar.c \
../lvgl/src/widgets/lv_btn.c \
../lvgl/src/widgets/lv_btnmatrix.c \
../lvgl/src/widgets/lv_canvas.c \
../lvgl/src/widgets/lv_checkbox.c \
../lvgl/src/widgets/lv_dropdown.c \
../lvgl/src/widgets/lv_img.c \
../lvgl/src/widgets/lv_label.c \
../lvgl/src/widgets/lv_line.c \
../lvgl/src/widgets/lv_objx_templ.c \
../lvgl/src/widgets/lv_roller.c \
../lvgl/src/widgets/lv_slider.c \
../lvgl/src/widgets/lv_switch.c \
../lvgl/src/widgets/lv_table.c \
../lvgl/src/widgets/lv_textarea.c 

C_DEPS += \
./lvgl/src/widgets/lv_arc.d \
./lvgl/src/widgets/lv_bar.d \
./lvgl/src/widgets/lv_btn.d \
./lvgl/src/widgets/lv_btnmatrix.d \
./lvgl/src/widgets/lv_canvas.d \
./lvgl/src/widgets/lv_checkbox.d \
./lvgl/src/widgets/lv_dropdown.d \
./lvgl/src/widgets/lv_img.d \
./lvgl/src/widgets/lv_label.d \
./lvgl/src/widgets/lv_line.d \
./lvgl/src/widgets/lv_objx_templ.d \
./lvgl/src/widgets/lv_roller.d \
./lvgl/src/widgets/lv_slider.d \
./lvgl/src/widgets/lv_switch.d \
./lvgl/src/widgets/lv_table.d \
./lvgl/src/widgets/lv_textarea.d 

OBJS += \
./lvgl/src/widgets/lv_arc.o \
./lvgl/src/widgets/lv_bar.o \
./lvgl/src/widgets/lv_btn.o \
./lvgl/src/widgets/lv_btnmatrix.o \
./lvgl/src/widgets/lv_canvas.o \
./lvgl/src/widgets/lv_checkbox.o \
./lvgl/src/widgets/lv_dropdown.o \
./lvgl/src/widgets/lv_img.o \
./lvgl/src/widgets/lv_label.o \
./lvgl/src/widgets/lv_line.o \
./lvgl/src/widgets/lv_objx_templ.o \
./lvgl/src/widgets/lv_roller.o \
./lvgl/src/widgets/lv_slider.o \
./lvgl/src/widgets/lv_switch.o \
./lvgl/src/widgets/lv_table.o \
./lvgl/src/widgets/lv_textarea.o 


# Each subdirectory must supply rules for building sources it contributes
lvgl/src/widgets/%.o: ../lvgl/src/widgets/%.c lvgl/src/widgets/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32H735xx -c -I../Core/Inc -I../display_lvgl -I../CustomEdh/ETH -I../CustomEdh/Tasklvgl -I../CustomEdh/TaskCLI -I../CustomEdh/ADF4372 -I../CustomEdh/FRAM -I../CustomEdh/Utility -I../CustomEdh/CLI -I../CustomEdh/Init -I../CustomEdh/AD5592R -I../CustomEdh/GSL1680F -I../CustomEdh/Taskctrl -I../CustomEdh/ExtFlash -I../CustomEdh/TaskForTest -I"D:/EDH/Bitono/lvgl_test_work_space/lvgl_test" -I../LWIP/App -I../LWIP/Target -I../Middlewares/Third_Party/LwIP/src/include -I../Middlewares/Third_Party/LwIP/system -I../Drivers/STM32H7xx_HAL_Driver/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Drivers/BSP/Components/lan8742 -I../Middlewares/Third_Party/LwIP/src/include/netif/ppp -I../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../Middlewares/Third_Party/LwIP/src/include/lwip -I../Middlewares/Third_Party/LwIP/src/include/lwip/apps -I../Middlewares/Third_Party/LwIP/src/include/lwip/priv -I../Middlewares/Third_Party/LwIP/src/include/lwip/prot -I../Middlewares/Third_Party/LwIP/src/include/netif -I../Middlewares/Third_Party/LwIP/src/include/compat/posix -I../Middlewares/Third_Party/LwIP/src/include/compat/posix/arpa -I../Middlewares/Third_Party/LwIP/src/include/compat/posix/net -I../Middlewares/Third_Party/LwIP/src/include/compat/posix/sys -I../Middlewares/Third_Party/LwIP/src/include/compat/stdc -I../Middlewares/Third_Party/LwIP/system/arch -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-lvgl-2f-src-2f-widgets

clean-lvgl-2f-src-2f-widgets:
	-$(RM) ./lvgl/src/widgets/lv_arc.d ./lvgl/src/widgets/lv_arc.o ./lvgl/src/widgets/lv_bar.d ./lvgl/src/widgets/lv_bar.o ./lvgl/src/widgets/lv_btn.d ./lvgl/src/widgets/lv_btn.o ./lvgl/src/widgets/lv_btnmatrix.d ./lvgl/src/widgets/lv_btnmatrix.o ./lvgl/src/widgets/lv_canvas.d ./lvgl/src/widgets/lv_canvas.o ./lvgl/src/widgets/lv_checkbox.d ./lvgl/src/widgets/lv_checkbox.o ./lvgl/src/widgets/lv_dropdown.d ./lvgl/src/widgets/lv_dropdown.o ./lvgl/src/widgets/lv_img.d ./lvgl/src/widgets/lv_img.o ./lvgl/src/widgets/lv_label.d ./lvgl/src/widgets/lv_label.o ./lvgl/src/widgets/lv_line.d ./lvgl/src/widgets/lv_line.o ./lvgl/src/widgets/lv_objx_templ.d ./lvgl/src/widgets/lv_objx_templ.o ./lvgl/src/widgets/lv_roller.d ./lvgl/src/widgets/lv_roller.o ./lvgl/src/widgets/lv_slider.d ./lvgl/src/widgets/lv_slider.o ./lvgl/src/widgets/lv_switch.d ./lvgl/src/widgets/lv_switch.o ./lvgl/src/widgets/lv_table.d ./lvgl/src/widgets/lv_table.o ./lvgl/src/widgets/lv_textarea.d ./lvgl/src/widgets/lv_textarea.o

.PHONY: clean-lvgl-2f-src-2f-widgets

