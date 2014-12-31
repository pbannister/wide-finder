################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../ZReader.cpp \
../ZScanner.cpp \
../ZString.cpp \
../ZTimer.cpp \
../ZTrace.cpp \
../main.cpp 

OBJS += \
./ZReader.o \
./ZScanner.o \
./ZString.o \
./ZTimer.o \
./ZTrace.o \
./main.o 

CPP_DEPS += \
./ZReader.d \
./ZScanner.d \
./ZString.d \
./ZTimer.d \
./ZTrace.d \
./main.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -DNDEBUG -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


