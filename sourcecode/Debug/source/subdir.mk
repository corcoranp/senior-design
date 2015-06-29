################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../source/QRCode.cpp \
../source/Zone.cpp \
../source/bootloader.cpp \
../source/sysops.cpp 

OBJS += \
./source/QRCode.o \
./source/Zone.o \
./source/bootloader.o \
./source/sysops.o 

CPP_DEPS += \
./source/QRCode.d \
./source/Zone.d \
./source/bootloader.d \
./source/sysops.d 


# Each subdirectory must supply rules for building sources it contributes
source/%.o: ../source/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cygwin C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


