################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/bootloader.c \
../src/hal_entry.c \
../src/hal_warmstart.c \
../src/user_printf.c 

C_DEPS += \
./src/bootloader.d \
./src/hal_entry.d \
./src/hal_warmstart.d \
./src/user_printf.d 

OBJS += \
./src/bootloader.o \
./src/hal_entry.o \
./src/hal_warmstart.o \
./src/user_printf.o 

SREC += \
bootloader_ra8p1_xip_flat.srec 

MAP += \
bootloader_ra8p1_xip_flat.map 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	$(file > $@.in,-mthumb -mfloat-abi=hard -mcpu=cortex-m85+nopacbti -O2 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-strict-aliasing -Wunused -Wuninitialized -Wall -Wextra -Wmissing-declarations -Wconversion -Wpointer-arith -Wshadow -Wlogical-op -Waggregate-return -Wfloat-equal -g -D_RENESAS_RA_ -D_RA_CORE=CPU0 -D_RA_ORDINAL=1 -I"D:/Single-chip/OpenMV/p1-repo/bootloader_ra8p1_xip_flat/ra_gen" -I"." -I"D:/Single-chip/OpenMV/p1-repo/bootloader_ra8p1_xip_flat/ra_cfg/fsp_cfg/bsp" -I"D:/Single-chip/OpenMV/p1-repo/bootloader_ra8p1_xip_flat/ra_cfg/fsp_cfg" -I"D:/Single-chip/OpenMV/p1-repo/bootloader_ra8p1_xip_flat/ra/arm/CMSIS_6/CMSIS/Core/Include" -I"D:/Single-chip/OpenMV/p1-repo/bootloader_ra8p1_xip_flat/src" -I"D:/Single-chip/OpenMV/p1-repo/bootloader_ra8p1_xip_flat/ra/fsp/inc" -I"D:/Single-chip/OpenMV/p1-repo/bootloader_ra8p1_xip_flat/ra/fsp/inc/api" -I"D:/Single-chip/OpenMV/p1-repo/bootloader_ra8p1_xip_flat/ra/fsp/inc/instances" -std=c99 -Wno-stringop-overflow -Wno-format-truncation -flax-vector-conversions --param=min-pagesize=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" -x c "$<")
	@echo Building file: $< && arm-none-eabi-gcc @"$@.in"

