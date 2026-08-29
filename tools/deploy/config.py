"""Project paths, addresses, and tool configurations."""

import os

# Workspace root (one level up from tools/)
WORKSPACE = os.path.normpath(os.path.join(os.path.dirname(__file__), "..", ".."))

# ── J-Link ──────────────────────────────────────────────
JLINK_EXE = r"D:\Single-chip\STLINK\JLink\JLink_V796\JLink.exe"  # 根据实际安装路径修改
JLINK_DEVICE = "R7KA8P1KF_CPU0"
JLINK_SPEED = 4000  # kHz

# ── MCUboot Signing ─────────────────────────────────────
MCUBOOT_KEY = os.path.join(
    WORKSPACE, "bootloader_ra8p1", "ra", "mcu-tools", "MCUboot", "root-ec-p256.pem"
)
MCUBOOT_SIGN_SCRIPT = os.path.join(
    WORKSPACE, "bootloader_ra8p1", "ra", "fsp", "src", "rm_mcuboot_port",
    "rm_mcuboot_port_sign.py",
)
OBJCOPY = (
    r"D:\Single-chip\MySoftware\e2studiofsp6.4\toolchains\gcc_arm" # 根据实际安装路径修改
    r"\13.2.rel1\bin\arm-none-eabi-objcopy.exe"
)
MCUBOOT_IMAGE_VERSION = "1.1.0"
MCUBOOT_ALIGN = 32
MCUBOOT_MAX_ALIGN = 32
MCUBOOT_FLASH_ERASE_SIZE = 32768

# ── Address Map ─────────────────────────────────────────
ADDR_INTERNAL_FLASH = 0x02000000
ADDR_SECONDARY_OSPI = 0x90000000
ADDR_ROMFS_MRAM = 0x020D0000
ROMFS_SIZE = 192 * 1024  # 192 KB
SLOT_SIZE_CORRECT = 0x00300000  # 3 MB
SLOT_SIZE_WRONG = 0x00030000    # RASC default (incorrect for OSPI)

# ── USB CDC ─────────────────────────────────────────────
USB_VID_PRIMARY = 0x0000
USB_PID_PRIMARY = 0x0002
USB_VID_SECONDARY = 0x37C5  # OpenMV
USB_PID_SECONDARY = 0x1204
BAUDRATE = 115200  # virtual baud rate (does not limit actual USB CDC speed)

# ── Build Outputs ───────────────────────────────────────
BOOTLOADER_SREC = os.path.join(
    WORKSPACE, "bootloader_ra8p1", "Debug", "bootloader_ra8p1.srec"
)
PRIMARY_CPU0_SREC = os.path.join(
    WORKSPACE, "primary_cpu0_ra8p1", "Debug", "primary_cpu0_ra8p1.srec"
)
PRIMARY_CPU1_SREC = os.path.join(
    WORKSPACE, "primary_cpu1_ra8p1", "Debug", "primary_cpu1_ra8p1.srec"
)
SECONDARY_ELF = os.path.join(
    WORKSPACE, "secondary_cpu0_ra8p1", "Debug", "secondary_cpu0_ra8p1.elf"
)
SECONDARY_SIGNED = os.path.join(
    WORKSPACE, "secondary_cpu0_ra8p1", "Debug", "secondary_cpu0_ra8p1.bin.signed"
)
SECONDARY_BSP_LINKER = os.path.join(
    WORKSPACE, "secondary_cpu0_ra8p1", "Debug", "bsp_linker_info.h"
)

# ── romfs ───────────────────────────────────────────────
ROMFS_JSON = os.path.join(
    WORKSPACE, "secondary_cpu0_ra8p1", "src", "romfs.json"
)
ROMFS_IMG = os.path.join(
    WORKSPACE, "secondary_cpu0_ra8p1", "src", "romfs0.img"
)
MKROMFS_SCRIPT = os.path.join(
    WORKSPACE, "secondary_cpu0_ra8p1", "src", "OpenMV", "tools", "mkromfs.py"
)
