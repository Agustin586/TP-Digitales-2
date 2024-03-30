################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../board/board.c \
../board/clock_config.c \
../board/peripherals.c \
../board/pin_mux.c 

C_DEPS += \
./board/board.d \
./board/clock_config.d \
./board/peripherals.d \
./board/pin_mux.d 

OBJS += \
./board/board.o \
./board/clock_config.o \
./board/peripherals.o \
./board/pin_mux.o 


# Each subdirectory must supply rules for building sources it contributes
board/%.o: ../board/%.c board/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -DCPU_MKL46Z256VLL4_cm0plus -DCPU_MKL46Z256VLL4 -DFSL_RTOS_BM -DSDK_OS_BAREMETAL -DSDK_DEBUGCONSOLE=1 -DCR_INTEGER_PRINTF -DPRINTF_FLOAT_ENABLE=0 -DSDK_DEBUGCONSOLE_UART -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -D__REDLIB__ -I"C:\Users\aguat\OneDrive\Documentos\GitHub\TP-Digitales 2\Tp1\TP1_DIGITALES 2\board" -I"C:\Users\aguat\OneDrive\Documentos\GitHub\TP-Digitales 2\Tp1\TP1_DIGITALES 2\source" -I"C:\Users\aguat\OneDrive\Documentos\GitHub\TP-Digitales 2\Tp1\TP1_DIGITALES 2" -I"C:\Users\aguat\OneDrive\Documentos\GitHub\TP-Digitales 2\Tp1\TP1_DIGITALES 2\drivers" -I"C:\Users\aguat\OneDrive\Documentos\GitHub\TP-Digitales 2\Tp1\TP1_DIGITALES 2\CMSIS" -I"C:\Users\aguat\OneDrive\Documentos\GitHub\TP-Digitales 2\Tp1\TP1_DIGITALES 2\utilities" -I"C:\Users\aguat\OneDrive\Documentos\GitHub\TP-Digitales 2\Tp1\TP1_DIGITALES 2\startup" -Og -fno-common -g3 -Wall -c -ffunction-sections -fdata-sections -ffreestanding -fno-builtin -fmacro-prefix-map="$(<D)/"= -mcpu=cortex-m0plus -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-board

clean-board:
	-$(RM) ./board/board.d ./board/board.o ./board/clock_config.d ./board/clock_config.o ./board/peripherals.d ./board/peripherals.o ./board/pin_mux.d ./board/pin_mux.o

.PHONY: clean-board

