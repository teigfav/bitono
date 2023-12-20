################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../lvgl/src/draw/sw/blend/lv_draw_sw_blend.c \
../lvgl/src/draw/sw/blend/lv_draw_sw_blend_to_argb8888.c \
../lvgl/src/draw/sw/blend/lv_draw_sw_blend_to_rgb565.c \
../lvgl/src/draw/sw/blend/lv_draw_sw_blend_to_rgb888.c 

C_DEPS += \
./lvgl/src/draw/sw/blend/lv_draw_sw_blend.d \
./lvgl/src/draw/sw/blend/lv_draw_sw_blend_to_argb8888.d \
./lvgl/src/draw/sw/blend/lv_draw_sw_blend_to_rgb565.d \
./lvgl/src/draw/sw/blend/lv_draw_sw_blend_to_rgb888.d 

OBJS += \
./lvgl/src/draw/sw/blend/lv_draw_sw_blend.o \
./lvgl/src/draw/sw/blend/lv_draw_sw_blend_to_argb8888.o \
./lvgl/src/draw/sw/blend/lv_draw_sw_blend_to_rgb565.o \
./lvgl/src/draw/sw/blend/lv_draw_sw_blend_to_rgb888.o 


# Each subdirectory must supply rules for building sources it contributes
lvgl/src/draw/sw/blend/%.o: ../lvgl/src/draw/sw/blend/%.c lvgl/src/draw/sw/blend/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32H735xx -c -I../Core/Inc -I../CustomEdh/TaskSint -I../display_lvgl -I../CustomEdh/ETH -I../CustomEdh/Tasklvgl -I../CustomEdh/TaskCLI -I../CustomEdh/ADF4372 -I../CustomEdh/FRAM -I../CustomEdh/Utility -I../CustomEdh/CLI -I../CustomEdh/Init -I../CustomEdh/AD5592R -I../CustomEdh/GSL1680F -I../CustomEdh/TaskGFX -I../CustomEdh/Task_Bias_Pwrctrl -I../CustomEdh/ExtFlash -I../CustomEdh/TaskForTest -I"D:/EDH/Bitono/lvgl_test_work_space/lvgl_test" -I../LWIP/App -I../LWIP/Target -I../Middlewares/Third_Party/LwIP/src/include -I../Middlewares/Third_Party/LwIP/system -I../Drivers/STM32H7xx_HAL_Driver/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Drivers/BSP/Components/lan8742 -I../Middlewares/Third_Party/LwIP/src/include/netif/ppp -I../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../Middlewares/Third_Party/LwIP/src/include/lwip -I../Middlewares/Third_Party/LwIP/src/include/lwip/apps -I../Middlewares/Third_Party/LwIP/src/include/lwip/priv -I../Middlewares/Third_Party/LwIP/src/include/lwip/prot -I../Middlewares/Third_Party/LwIP/src/include/netif -I../Middlewares/Third_Party/LwIP/src/include/compat/posix -I../Middlewares/Third_Party/LwIP/src/include/compat/posix/arpa -I../Middlewares/Third_Party/LwIP/src/include/compat/posix/net -I../Middlewares/Third_Party/LwIP/src/include/compat/posix/sys -I../Middlewares/Third_Party/LwIP/src/include/compat/stdc -I../Middlewares/Third_Party/LwIP/system/arch -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-lvgl-2f-src-2f-draw-2f-sw-2f-blend

clean-lvgl-2f-src-2f-draw-2f-sw-2f-blend:
	-$(RM) ./lvgl/src/draw/sw/blend/lv_draw_sw_blend.d ./lvgl/src/draw/sw/blend/lv_draw_sw_blend.o ./lvgl/src/draw/sw/blend/lv_draw_sw_blend_to_argb8888.d ./lvgl/src/draw/sw/blend/lv_draw_sw_blend_to_argb8888.o ./lvgl/src/draw/sw/blend/lv_draw_sw_blend_to_rgb565.d ./lvgl/src/draw/sw/blend/lv_draw_sw_blend_to_rgb565.o ./lvgl/src/draw/sw/blend/lv_draw_sw_blend_to_rgb888.d ./lvgl/src/draw/sw/blend/lv_draw_sw_blend_to_rgb888.o

.PHONY: clean-lvgl-2f-src-2f-draw-2f-sw-2f-blend

