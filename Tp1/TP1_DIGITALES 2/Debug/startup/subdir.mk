################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../startup/startup_mkl46z4.c 

C_DEPS += \
./startup/startup_mkl46z4.d 

OBJS += \
./startup/startup_mkl46z4.o 


# Each subdirectory must supply rules for building sources it contributes
startup/%.o: ../startup/%.c startup/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -DCPU_MKL46Z256VLL4_cm0plus -DCPU_MKL46Z256VLL4 -DFSL_RTOS_BM -DSDK_OS_BAREMETAL -DSDK_DEBUGCONSOLE=1 -DCR_INTEGER_PRINTF -DPRINTF_FLOAT_ENABLE=0 -DSDK_DEBUGCONSOLE_UART -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -D__REDLIB__ -I"C:\Users\aguat\OneDrive\Documentos\GitHub\TP-Digitales 2\Tp1\TP1_DIGITALES 2\board" -I"C:\Users\aguat\OneDrive\Documentos\GitHub\TP-Digitales 2\Tp1\TP1_DIGITALES 2\source" -I"C:\Users\aguat\OneDrive\Documentos\GitHub\TP-Digitales 2\Tp1\TP1_DIGITALES 2" -I"C:\Users\aguat\OneDrive\Documentos\GitHub\TP-Digitales 2\Tp1\TP1_DIGITALES 2\drivers" -I"C:\Users\aguat\OneDrive\Documentos\GitHub\TP-Digitales 2\Tp1\TP1_DIGITALES 2\CMSIS" -I"C:\Users\aguat\OneDrive\Documentos\GitHub\TP-Digitales 2\Tp1\TP1_DIGITALES 2\utilities" -I"C:\Users\aguat\OneDrive\Documentos\GitHub\TP-Digitales 2\Tp1\TP1_DIGITALES 2\startup" -Og -fno-common -g3 -Wall -c -ffunction-sections -fdata-sections -ffreestanding -fno-builtin -fmacro-prefix-map="$(<D)/"= -mcpu=cortex-m0plus -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-startup

clean-startup:
	-$(RM) ./startup/startup_mkl46z4.d ./startup/startup_mkl46z4.o

.PHONY: clean-startup

