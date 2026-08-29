"""Flash bootloader + primary (CPU0 + CPU1) to internal flash via J-Link loadfile.

Uses S-Record format (.srec) which J-Link handles natively for internal flash.

Order matters: bootloader first, then primary CPU0, then primary CPU1.
After flashing, MCU resets and runs bootloader -> MCUboot -> primary.
"""

import os

from . import config
from . import jlink


def flash_internal(
    bootloader_srec=None,
    primary_cpu0_srec=None,
    primary_cpu1_srec=None,
    reset=True,
):
    """Flash bootloader + primary to internal flash via J-Link.

    Args:
        bootloader_srec: path to bootloader .srec. Defaults to config.
        primary_cpu0_srec: path to primary CPU0 .srec. Defaults to config.
        primary_cpu1_srec: path to primary CPU1 .srec. Defaults to config.
        reset: whether to reset MCU after all files are loaded.

    Returns:
        True on success, False on failure
    """
    srecs = [
        bootloader_srec or config.BOOTLOADER_SREC,
        primary_cpu0_srec or config.PRIMARY_CPU0_SREC,
        primary_cpu1_srec or config.PRIMARY_CPU1_SREC,
    ]

    for p in srecs:
        if not os.path.exists(p):
            print(f"ERROR: S-Record not found: {p}")
            return False
        print(f"  {os.path.basename(p)}")

    print("Flashing internal flash via J-Link loadfile...")
    result = jlink.loadfile_multi(srecs, reset=reset)

    # J-Link outputs to stdout, not stderr
    output = result.stdout or result.stderr or ""
    if "O.K." in output or result.returncode == 0:
        print("Internal flash programmed successfully.")
        return True
    else:
        # Show last 20 lines of J-Link output for diagnosis
        lines = output.strip().splitlines()
        for line in lines[-20:]:
            print(f"  {line}")
        return False
