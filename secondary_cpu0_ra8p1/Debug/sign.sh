#!/bin/bash
export MCUBOOT_IMAGE_VERSION=1.0.0
export MCUBOOT_IMAGE_SIGNING_KEY="D:/Single-chip/e2studio/ws7_xip/bootloader_ra8p1/ra/mcu-tools/MCUboot/root-ec-p256.pem"
export MCUBOOT_APP_BIN_CONVERTER="D:/Single-chip/MySoftware/e2studiofsp6.4/toolchains/gcc_arm/13.2.rel1/bin/arm-none-eabi-objcopy.exe"

python "D:/Single-chip/e2studio/ws7_xip/bootloader_ra8p1/ra/fsp/src/rm_mcuboot_port/rm_mcuboot_port_sign.py" sign --align 32 --max-align 32 --flash_erase_size_bytes 32768 --confirm --pad-header secondary_cpu0_ra8p1.elf secondary_cpu0_ra8p1.bin.signed

read -p "Press any key to continue..."
