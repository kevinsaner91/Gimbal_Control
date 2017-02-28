################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Gimbal_control.cpp \
../SBGC.cpp \
../Serial.cpp \
../writeToFile.cpp 

OBJS += \
./Gimbal_control.o \
./SBGC.o \
./Serial.o \
./writeToFile.o 

CPP_DEPS += \
./Gimbal_control.d \
./SBGC.d \
./Serial.d \
./writeToFile.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	arm-phytec-linux-gnueabi-gcc  -march=armv7-a -mthumb-interwork -mfloat-abi=hard -mfpu=neon -mtune=cortex-a9 --sysroot=/opt/yogurt/iMX6-PD15.2.0/sysroots/cortexa9hf-vfp-neon-phytec-linux-gnueabi -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


