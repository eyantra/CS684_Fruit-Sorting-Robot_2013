################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/FSLemon.cpp \
../src/FSVision.cpp \
../src/FruitAttribute.cpp \
../src/FruitSortingApp.cpp \
../src/Group.cpp \
../src/GroupManager.cpp \
../src/test.cpp 

OBJS += \
./src/FSLemon.o \
./src/FSVision.o \
./src/FruitAttribute.o \
./src/FruitSortingApp.o \
./src/Group.o \
./src/GroupManager.o \
./src/test.o 

CPP_DEPS += \
./src/FSLemon.d \
./src/FSVision.d \
./src/FruitAttribute.d \
./src/FruitSortingApp.d \
./src/Group.d \
./src/GroupManager.d \
./src/test.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/usr/include/opencv -I/usr/include/opencv2/ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


