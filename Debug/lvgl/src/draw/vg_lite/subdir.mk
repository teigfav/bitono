################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../lvgl/src/draw/vg_lite/lv_draw_buf_vg_lite.c \
../lvgl/src/draw/vg_lite/lv_draw_vg_lite.c \
../lvgl/src/draw/vg_lite/lv_draw_vg_lite_arc.c \
../lvgl/src/draw/vg_lite/lv_draw_vg_lite_border.c \
../lvgl/src/draw/vg_lite/lv_draw_vg_lite_box_shadow.c \
../lvgl/src/draw/vg_lite/lv_draw_vg_lite_fill.c \
../lvgl/src/draw/vg_lite/lv_draw_vg_lite_img.c \
../lvgl/src/draw/vg_lite/lv_draw_vg_lite_label.c \
../lvgl/src/draw/vg_lite/lv_draw_vg_lite_layer.c \
../lvgl/src/draw/vg_lite/lv_draw_vg_lite_line.c \
../lvgl/src/draw/vg_lite/lv_draw_vg_lite_mask_rect.c \
../lvgl/src/draw/vg_lite/lv_draw_vg_lite_triangle.c \
../lvgl/src/draw/vg_lite/lv_draw_vg_lite_vector.c \
../lvgl/src/draw/vg_lite/lv_vg_lite_decoder.c \
../lvgl/src/draw/vg_lite/lv_vg_lite_math.c \
../lvgl/src/draw/vg_lite/lv_vg_lite_path.c \
../lvgl/src/draw/vg_lite/lv_vg_lite_utils.c 

C_DEPS += \
./lvgl/src/draw/vg_lite/lv_draw_buf_vg_lite.d \
./lvgl/src/draw/vg_lite/lv_draw_vg_lite.d \
./lvgl/src/draw/vg_lite/lv_draw_vg_lite_arc.d \
./lvgl/src/draw/vg_lite/lv_draw_vg_lite_border.d \
./lvgl/src/draw/vg_lite/lv_draw_vg_lite_box_shadow.d \
./lvgl/src/draw/vg_lite/lv_draw_vg_lite_fill.d \
./lvgl/src/draw/vg_lite/lv_draw_vg_lite_img.d \
./lvgl/src/draw/vg_lite/lv_draw_vg_lite_label.d \
./lvgl/src/draw/vg_lite/lv_draw_vg_lite_layer.d \
./lvgl/src/draw/vg_lite/lv_draw_vg_lite_line.d \
./lvgl/src/draw/vg_lite/lv_draw_vg_lite_mask_rect.d \
./lvgl/src/draw/vg_lite/lv_draw_vg_lite_triangle.d \
./lvgl/src/draw/vg_lite/lv_draw_vg_lite_vector.d \
./lvgl/src/draw/vg_lite/lv_vg_lite_decoder.d \
./lvgl/src/draw/vg_lite/lv_vg_lite_math.d \
./lvgl/src/draw/vg_lite/lv_vg_lite_path.d \
./lvgl/src/draw/vg_lite/lv_vg_lite_utils.d 

OBJS += \
./lvgl/src/draw/vg_lite/lv_draw_buf_vg_lite.o \
./lvgl/src/draw/vg_lite/lv_draw_vg_lite.o \
./lvgl/src/draw/vg_lite/lv_draw_vg_lite_arc.o \
./lvgl/src/draw/vg_lite/lv_draw_vg_lite_border.o \
./lvgl/src/draw/vg_lite/lv_draw_vg_lite_box_shadow.o \
./lvgl/src/draw/vg_lite/lv_draw_vg_lite_fill.o \
./lvgl/src/draw/vg_lite/lv_draw_vg_lite_img.o \
./lvgl/src/draw/vg_lite/lv_draw_vg_lite_label.o \
./lvgl/src/draw/vg_lite/lv_draw_vg_lite_layer.o \
./lvgl/src/draw/vg_lite/lv_draw_vg_lite_line.o \
./lvgl/src/draw/vg_lite/lv_draw_vg_lite_mask_rect.o \
./lvgl/src/draw/vg_lite/lv_draw_vg_lite_triangle.o \
./lvgl/src/draw/vg_lite/lv_draw_vg_lite_vector.o \
./lvgl/src/draw/vg_lite/lv_vg_lite_decoder.o \
./lvgl/src/draw/vg_lite/lv_vg_lite_math.o \
./lvgl/src/draw/vg_lite/lv_vg_lite_path.o \
./lvgl/src/draw/vg_lite/lv_vg_lite_utils.o 


# Each subdirectory must supply rules for building sources it contributes
lvgl/src/draw/vg_lite/%.o: ../lvgl/src/draw/vg_lite/%.c lvgl/src/draw/vg_lite/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32H735xx -c -I../Core/Inc -I../CustomEdh/TaskSint -I../display_lvgl -I../CustomEdh/ETH -I../CustomEdh/Tasklvgl -I../CustomEdh/TaskCLI -I../CustomEdh/ADF4372 -I../CustomEdh/FRAM -I../CustomEdh/Utility -I../CustomEdh/CLI -I../CustomEdh/Init -I../CustomEdh/AD5592R -I../CustomEdh/GSL1680F -I../CustomEdh/TaskGFX -I../CustomEdh/Task_Bias_Pwrctrl -I../CustomEdh/ExtFlash -I../CustomEdh/TaskForTest -I"D:/EDH/Bitono/lvgl_test_work_space/lvgl_test" -I../LWIP/App -I../LWIP/Target -I../Middlewares/Third_Party/LwIP/src/include -I../Middlewares/Third_Party/LwIP/system -I../Drivers/STM32H7xx_HAL_Driver/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Drivers/BSP/Components/lan8742 -I../Middlewares/Third_Party/LwIP/src/include/netif/ppp -I../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../Middlewares/Third_Party/LwIP/src/include/lwip -I../Middlewares/Third_Party/LwIP/src/include/lwip/apps -I../Middlewares/Third_Party/LwIP/src/include/lwip/priv -I../Middlewares/Third_Party/LwIP/src/include/lwip/prot -I../Middlewares/Third_Party/LwIP/src/include/netif -I../Middlewares/Third_Party/LwIP/src/include/compat/posix -I../Middlewares/Third_Party/LwIP/src/include/compat/posix/arpa -I../Middlewares/Third_Party/LwIP/src/include/compat/posix/net -I../Middlewares/Third_Party/LwIP/src/include/compat/posix/sys -I../Middlewares/Third_Party/LwIP/src/include/compat/stdc -I../Middlewares/Third_Party/LwIP/system/arch -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-lvgl-2f-src-2f-draw-2f-vg_lite

clean-lvgl-2f-src-2f-draw-2f-vg_lite:
	-$(RM) ./lvgl/src/draw/vg_lite/lv_draw_buf_vg_lite.d ./lvgl/src/draw/vg_lite/lv_draw_buf_vg_lite.o ./lvgl/src/draw/vg_lite/lv_draw_vg_lite.d ./lvgl/src/draw/vg_lite/lv_draw_vg_lite.o ./lvgl/src/draw/vg_lite/lv_draw_vg_lite_arc.d ./lvgl/src/draw/vg_lite/lv_draw_vg_lite_arc.o ./lvgl/src/draw/vg_lite/lv_draw_vg_lite_border.d ./lvgl/src/draw/vg_lite/lv_draw_vg_lite_border.o ./lvgl/src/draw/vg_lite/lv_draw_vg_lite_box_shadow.d ./lvgl/src/draw/vg_lite/lv_draw_vg_lite_box_shadow.o ./lvgl/src/draw/vg_lite/lv_draw_vg_lite_fill.d ./lvgl/src/draw/vg_lite/lv_draw_vg_lite_fill.o ./lvgl/src/draw/vg_lite/lv_draw_vg_lite_img.d ./lvgl/src/draw/vg_lite/lv_draw_vg_lite_img.o ./lvgl/src/draw/vg_lite/lv_draw_vg_lite_label.d ./lvgl/src/draw/vg_lite/lv_draw_vg_lite_label.o ./lvgl/src/draw/vg_lite/lv_draw_vg_lite_layer.d ./lvgl/src/draw/vg_lite/lv_draw_vg_lite_layer.o ./lvgl/src/draw/vg_lite/lv_draw_vg_lite_line.d ./lvgl/src/draw/vg_lite/lv_draw_vg_lite_line.o ./lvgl/src/draw/vg_lite/lv_draw_vg_lite_mask_rect.d ./lvgl/src/draw/vg_lite/lv_draw_vg_lite_mask_rect.o ./lvgl/src/draw/vg_lite/lv_draw_vg_lite_triangle.d ./lvgl/src/draw/vg_lite/lv_draw_vg_lite_triangle.o ./lvgl/src/draw/vg_lite/lv_draw_vg_lite_vector.d ./lvgl/src/draw/vg_lite/lv_draw_vg_lite_vector.o ./lvgl/src/draw/vg_lite/lv_vg_lite_decoder.d ./lvgl/src/draw/vg_lite/lv_vg_lite_decoder.o ./lvgl/src/draw/vg_lite/lv_vg_lite_math.d ./lvgl/src/draw/vg_lite/lv_vg_lite_math.o ./lvgl/src/draw/vg_lite/lv_vg_lite_path.d ./lvgl/src/draw/vg_lite/lv_vg_lite_path.o ./lvgl/src/draw/vg_lite/lv_vg_lite_utils.d ./lvgl/src/draw/vg_lite/lv_vg_lite_utils.o

.PHONY: clean-lvgl-2f-src-2f-draw-2f-vg_lite

