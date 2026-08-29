"""Flash romfs image to MRAM via J-Link loadbin.

J-Link loadbin cannot directly use .img files (reports "unknown format"),
so we copy to a .bin temp file before loading.

Verified: J-Link loadbin writes MRAM (0x020D0000) correctly at ~164 KB/s.
"""

import os
import shutil
import tempfile

from . import config
from . import jlink


def flash_romfs(romfs_img_path=None, reset=False):
    """Flash romfs image to MRAM via J-Link loadbin.

    Args:
        romfs_img_path: path to romfs0.img.
                        Defaults to config.ROMFS_IMG.
        reset: whether to reset MCU after writing.

    Returns:
        True on success, False on failure
    """
    if romfs_img_path is None:
        romfs_img_path = config.ROMFS_IMG

    if not os.path.exists(romfs_img_path):
        print(f"ERROR: romfs image not found: {romfs_img_path}")
        return False

    file_size = os.path.getsize(romfs_img_path)
    print(f"romfs image: {file_size} bytes ({file_size/1024:.1f} KB)")
    print(f"Target: MRAM @ {config.ADDR_ROMFS_MRAM:#x}")

    # J-Link rejects .img extension — copy to .bin
    bin_path = None
    if romfs_img_path.endswith(".img"):
        bin_path = os.path.join(
            tempfile.gettempdir(),
            "romfs_deploy.bin",
        )
        shutil.copy2(romfs_img_path, bin_path)
        print(f"  Copied .img -> .bin for J-Link compatibility")
    else:
        bin_path = romfs_img_path

    try:
        result = jlink.loadbin(bin_path, config.ADDR_ROMFS_MRAM, reset=reset)
        output = result.stdout or result.stderr or ""
        if "O.K." in output or result.returncode == 0:
            print("romfs flashed to MRAM successfully.")
            return True
        else:
            lines = output.strip().splitlines()
            for line in lines[-20:]:
                print(f"  {line}")
            return False
    finally:
        # Clean up temp .bin
        if bin_path != romfs_img_path and os.path.exists(bin_path):
            os.unlink(bin_path)
