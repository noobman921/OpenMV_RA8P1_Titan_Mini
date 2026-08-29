"""MCUboot image signing for secondary.

Wraps rm_mcuboot_port_sign.py with:
  - SLOT_SIZE patch (RASC generates 0x00030000, correct value is 0x00300000)
  - Environment variable setup (MCUBOOT_IMAGE_VERSION, KEY, OBJCOPY)
"""

import os
import re
import subprocess
import sys

from . import config


def _patch_slot_size(bsp_linker_path):
    """Patch SLOT_SIZE in bsp_linker_info.h if it has the wrong value.

    RASC generates SLOT_SIZE 0x00030000 (192KB) but OSPI secondary
    needs 0x00300000 (3MB). This is a known RASC limitation.

    Args:
        bsp_linker_path: path to bsp_linker_info.h

    Returns:
        True if patched, False if already correct or skipped
    """
    if not os.path.exists(bsp_linker_path):
        print(f"  WARNING: {bsp_linker_path} not found, skipping SLOT_SIZE patch")
        return False

    with open(bsp_linker_path, "r") as f:
        content = f.read()

    wrong = f"SLOT_SIZE 0x{config.SLOT_SIZE_WRONG:08X}"
    correct = f"SLOT_SIZE 0x{config.SLOT_SIZE_CORRECT:08X}"

    if wrong in content:
        content = content.replace(wrong, correct)
        with open(bsp_linker_path, "w") as f:
            f.write(content)
        print(f"  Patched SLOT_SIZE: {wrong} -> {correct}")
        return True
    else:
        print(f"  SLOT_SIZE already correct, skipping patch")
        return False


def sign(elf_path=None, output_path=None, patch_slot_size=True):
    """Sign secondary image with MCUboot ECDSA-P256.

    Args:
        elf_path: path to secondary .elf. Defaults to config.SECONDARY_ELF.
        output_path: path for signed output. Defaults to config.SECONDARY_SIGNED.
        patch_slot_size: whether to patch SLOT_SIZE before signing.

    Returns:
        True on success, False on failure
    """
    if elf_path is None:
        elf_path = config.SECONDARY_ELF
    if output_path is None:
        output_path = config.SECONDARY_SIGNED

    if not os.path.exists(elf_path):
        print(f"ERROR: ELF not found: {elf_path}")
        return False

    if not os.path.exists(config.MCUBOOT_SIGN_SCRIPT):
        print(f"ERROR: sign script not found: {config.MCUBOOT_SIGN_SCRIPT}")
        return False

    # Patch SLOT_SIZE if needed
    if patch_slot_size:
        _patch_slot_size(config.SECONDARY_BSP_LINKER)

    # Set environment variables required by rm_mcuboot_port_sign.py
    env = os.environ.copy()
    env["MCUBOOT_IMAGE_VERSION"] = config.MCUBOOT_IMAGE_VERSION
    env["MCUBOOT_IMAGE_SIGNING_KEY"] = config.MCUBOOT_KEY
    env["MCUBOOT_APP_BIN_CONVERTER"] = config.OBJCOPY

    # Run signing
    elf_name = os.path.basename(elf_path)
    out_name = os.path.basename(output_path)
    print(f"Signing {elf_name} -> {out_name}...")

    result = subprocess.run(
        [
            sys.executable,
            config.MCUBOOT_SIGN_SCRIPT,
            "sign",
            "--align", str(config.MCUBOOT_ALIGN),
            "--max-align", str(config.MCUBOOT_MAX_ALIGN),
            "--flash_erase_size_bytes", str(config.MCUBOOT_FLASH_ERASE_SIZE),
            "--confirm",
            "--pad-header",
            elf_name,
            out_name,
        ],
        cwd=os.path.dirname(elf_path),
        env=env,
        capture_output=True,
        text=True,
    )

    if result.stdout:
        for line in result.stdout.strip().splitlines():
            print(f"  {line}")
    if result.stderr:
        for line in result.stderr.strip().splitlines():
            print(f"  {line}")

    # Renesas rm_mcuboot_port_sign.py has a bug: sys.exit(1) is unconditional
    # (line 234 is outside the if/else). Check output file instead.
    if os.path.exists(output_path) and os.path.getsize(output_path) > 0:
        size = os.path.getsize(output_path)
        print(f"  Signed: {output_path} ({size/1024:.1f} KB)")
        return True
    else:
        print(f"  Signing failed (output file not generated)")
        return False
