################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../source/SD2_board.c \
../source/TP1_DIGITALES\ 2.c \
../source/key.c \
../source/mefACUM.c \
../source/mefCRUCE.c \
../source/mefMODO.c \
../source/mefRUTA.c \
../source/mefSEC.c \
../source/mtb.c \
../source/semihost_hardfault.c 

C_DEPS += \
./source/SD2_board.d \
./source/TP1_DIGITALES\ 2.d \
./source/key.d \
./source/mefACUM.d \
./source/mefCRUCE.d \
./source/mefMODO.d \
./source/mefRUTA.d \
./source/mefSEC.d \
./source/mtb.d \
./source/semihost_hardfault.d 

OBJS += \
./source/SD2_board.o \
./source/TP1_DIGITALES\ 2.o \
./source/key.o \
./source/mefACUM.o \
./source/mefCRUCE.o \
./source/mefMODO.o \
./source/mefRUTA.o \
./source/mefSEC.o \
./source/mtb.o \
./source/semihost_hardfault.o 


# Each subdirectory must supply rules for building sources it contributes
source/%.o: ../source/%.c source/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -DCPU_MKL46Z256VLL4_cm0plus -DCPU_MKL46Z256VLL4 -DFSL_RTOS_BM -DSDK_OS_BAREMETAL -DSDK_DEBUGCONSOLE=1 -DCR_INTEGER_PRINTF -DPRINTF_FLOAT_ENABLE=0 -DSDK_DEBUGCONSOLE_UART -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -D__REDLIB__ -I"C:\Users\aguat\OneDrive\Documentos\GitHub\TP-Digitales 2\Tp1\TP1_DIGITALES 2\board" -I"C:\Users\aguat\OneDrive\Documentos\GitHub\TP-Digitales 2\Tp1\TP1_DIGITALES 2\source" -I"C:\Users\aguat\OneDrive\Documentos\GitHub\TP-Digitales 2\Tp1\TP1_DIGITALES 2" -I"C:\Users\aguat\OneDrive\Documentos\GitHub\TP-Digitales 2\Tp1\TP1_DIGITALES 2\drivers" -I"C:\Users\aguat\OneDrive\Documentos\GitHub\TP-Digitales 2\Tp1\TP1_DIGITALES 2\CMSIS" -I"C:\Users\aguat\OneDrive\Documentos\GitHub\TP-Digitales 2\Tp1\TP1_DIGITALES 2\utilities" -I"C:\Users\aguat\OneDrive\Documentos\GitHub\TP-Digitales 2\Tp1\TP1_DIGITALES 2\startup" -Og -fno-common -g3 -Wall -c -ffunction-sections -fdata-sections -ffreestanding -fno-builtin -fmacro-prefix-map="$(<D)/"= -mcpu=cortex-m0plus -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

source/TP1_DIGITALES\ 2.o: ../source/TP1_DIGITALES\ 2.c source/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -DCPU_MKL46Z256VLL4_cm0plus -DCPU_MKL46Z256VLL4 -DFSL_RTOS_BM -DSDK_OS_BAREMETAL -DSDK_DEBUGCONSOLE=1 -DCR_INTEGER_PRINTF -DPRINTF_FLOAT_ENABLE=0 -DSDK_DEBUGCONSOLE_UART -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -D__REDLIB__ -I"C:\Users\aguat\OneDrive\Documentos\GitHub\TP-Digitales 2\Tp1\TP1_DIGITALES 2\board" -I"C:\Users\aguat\OneDrive\Documentos\GitHub\TP-Digitales 2\Tp1\TP1_DIGITALES 2\source" -I"C:\Users\aguat\OneDrive\Documentos\GitHub\TP-Digitales 2\Tp1\TP1_DIGITALES 2" -I"C:\Users\aguat\OneDrive\Documentos\GitHub\TP-Digitales 2\Tp1\TP1_DIGITALES 2\drivers" -I"C:\Users\aguat\OneDrive\Documentos\GitHub\TP-Digitales 2\Tp1\TP1_DIGITALES 2\CMSIS" -I"C:\Users\aguat\OneDrive\Documentos\GitHub\TP-Digitales 2\Tp1\TP1_DIGITALES 2\utilities" -I"C:\Users\aguat\OneDrive\Documentos\GitHub\TP-Digitales 2\Tp1\TP1_DIGITALES 2\startup" -Og -fno-common -g3 -Wall -c -ffunction-sections -fdata-sections -ffreestanding -fno-builtin -fmacro-prefix-map="$(<D)/"= -mcpu=cortex-m0plus -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"source/TP1_DIGITALES 2.d" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-source

clean-source:
	-$(RM) ./source/SD2_board.d ./source/SD2_board.o ./source/TP1_DIGITALES\ 2.d ./source/TP1_DIGITALES\ 2.o ./source/key.d ./source/key.o ./source/mefACUM.d ./source/mefACUM.o ./source/mefCRUCE.d ./source/mefCRUCE.o ./source/mefMODO.d ./source/mefMODO.o ./source/mefRUTA.d ./source/mefRUTA.o ./source/mefSEC.d ./source/mefSEC.o ./source/mtb.d ./source/mtb.o ./source/semihost_hardfault.d ./source/semihost_hardfault.o

.PHONY: clean-source

