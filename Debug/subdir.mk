################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../main.c 

OBJS += \
./main.o 

C_DEPS += \
./main.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gcc -I/usr/local/ti/c2000/C2000Ware_2_01_00_00/device_support/f2806x/ -I/usr/local/ti/c2000/C2000Ware_2_01_00_00/device_support/f2806x/common/include -I/usr/local/ti/c2000/C2000Ware_2_01_00_00/device_support/f2806x/headers -I/usr/local/ti/c2000/C2000Ware_2_01_00_00/device_support/f2806x/headers/include -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


