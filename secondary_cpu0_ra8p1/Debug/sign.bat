@echo off
set MCUBOOT_IMAGE_VERSION=1.1.0
set MCUBOOT_IMAGE_SIGNING_KEY=D:\Single-chip\e2studio\ws7_xip\bootloader_ra8p1\ra\mcu-tools\MCUboot\root-ec-p256.pem
set MCUBOOT_APP_BIN_CONVERTER=D:\Single-chip\MySoftware\e2studiofsp6.4\toolchains\gcc_arm\13.2.rel1\bin\arm-none-eabi-objcopy.exe

:: Fix SLOT_SIZE for OSPI-based secondary (0x300000 = 3MB)
powershell -Command "(gc bsp_linker_info.h) -replace 'SLOT_SIZE 0x00030000', 'SLOT_SIZE 0x00300000' | Out-File -encoding ASCII bsp_linker_info.h"

python "D:\Single-chip\e2studio\ws7_xip\bootloader_ra8p1\ra\fsp\src\rm_mcuboot_port\rm_mcuboot_port_sign.py" sign --align 32 --max-align 32 --flash_erase_size_bytes 32768 --confirm --pad-header secondary_cpu0_ra8p1.elf secondary_cpu0_ra8p1.bin.signed

pause
