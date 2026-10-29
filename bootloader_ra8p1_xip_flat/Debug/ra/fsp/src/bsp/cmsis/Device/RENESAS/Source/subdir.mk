################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../ra/fsp/src/bsp/cmsis/Device/RENESAS/Source/startup.c \
../ra/fsp/src/bsp/cmsis/Device/RENESAS/Source/system.c 

C_DEPS += \
./ra/fsp/src/bsp/cmsis/Device/RENESAS/Source/startup.d \
./ra/fsp/src/bsp/cmsis/Device/RENESAS/Source/system.d 

OBJS += \
./ra/fsp/src/bsp/cmsis/Device/RENESAS/Source/startup.o \
./ra/fsp/src/bsp/cmsis/Device/RENESAS/Source/system.o 

SREC += \
bootloader_ra8p1_xip_flat.srec 

MAP += \
bootloader_ra8p1_xip_flat.map 


# Each subdirectory must supply rules for building sources it contributes
ra/fsp/src/bsp/cmsis/Device/RENESAS/Source/%.o: ../ra/fsp/src/bsp/cmsis/Device/RENESAS/Source/%.c
	$(file > $@.in,-mthumb -mfloat-abi=hard -mcpu=cortex-m85+nopacbti -O2 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-strict-aliasing -Wunused -Wuninitialized -Wall -Wextra -Wmissing-declarations -Wconversion -Wpointer-arith -Wshadow -Wlogical-op -Waggregate-return -Wfloat-equal -g -D_RENESAS_RA_ -D_RA_CORE=CPU0 -D_RA_ORDINAL=1 -I"D:/Single-chip/OpenMV/p1-repo/bootloader_ra8p1_xip_flat/ra_gen" -I"." -I"D:/Single-chip/OpenMV/p1-repo/bootloader_ra8p1_xip_flat/ra_cfg/fsp_cfg/bsp" -I"D:/Single-chip/OpenMV/p1-repo/bootloader_ra8p1_xip_flat/ra_cfg/fsp_cfg" -I"D:/Single-chip/OpenMV/p1-repo/bootloader_ra8p1_xip_flat/ra/arm/CMSIS_6/CMSIS/Core/Include" -I"D:/Single-chip/OpenMV/p1-repo/bootloader_ra8p1_xip_flat/src" -I"D:/Single-chip/OpenMV/p1-repo/bootloader_ra8p1_xip_flat/ra/fsp/inc" -I"D:/Single-chip/OpenMV/p1-repo/bootloader_ra8p1_xip_flat/ra/fsp/inc/api" -I"D:/Single-chip/OpenMV/p1-repo/bootloader_ra8p1_xip_flat/ra/fsp/inc/instances" -std=c99 -Wno-stringop-overflow -Wno-format-truncation -flax-vector-conversions --param=min-pagesize=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" -x c "$<")
	@echo Building file: $< && arm-none-eabi-gcc @"$@.in"

