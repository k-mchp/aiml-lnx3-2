################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../lnx3_masters_egt.cpp 

CPP_DEPS += \
./lnx3_masters_egt.d 

OBJS += \
./lnx3_masters_egt.o 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.cpp subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	arm-buildroot-linux-gnueabihf-g++ -std=c++2a -O0 -g3 -Wall -c -fmessage-length=0 `pkg-config libegt --cflags` -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean--2e-

clean--2e-:
	-$(RM) ./lnx3_masters_egt.d ./lnx3_masters_egt.o

.PHONY: clean--2e-

