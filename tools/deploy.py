#!/usr/bin/env python3
"""RA8P1 Unified Deployment Tool.

Subcommands:
  sign              Sign secondary image with MCUboot
  flash-internal    Flash bootloader + primary to internal flash (J-Link)
  flash-secondary   Flash secondary to OSPI via USB CDC Xmodem
  flash-romfs       Flash romfs to MRAM via J-Link
  update            Sign + flash-secondary + flash-romfs (MCU already in primary)
  flash             Flash all (internal + secondary + romfs)
  deploy            Sign + flash all

Usage:
  python deploy.py update                    # most common: MCU in primary, update secondary + romfs
  python deploy.py update --port COM7        # specify COM port
  python deploy.py flash-secondary --port auto
  python deploy.py flash-romfs
  python deploy.py deploy
"""

import argparse
import sys
import time

# Add tools/ to path so we can import deploy package
import os
sys.path.insert(0, os.path.join(os.path.dirname(__file__), ".."))

from deploy.sign import sign
from deploy.flash_internal import flash_internal
from deploy.flash_secondary import flash_secondary, find_primary_port, find_secondary_port
from deploy.flash_romfs import flash_romfs


def cmd_sign(args):
    if not sign(
        elf_path=args.elf,
        output_path=args.output,
        patch_slot_size=not args.no_patch_slot_size,
    ):
        sys.exit(1)


def cmd_flash_internal(args):
    if not flash_internal(reset=not args.no_reset):
        sys.exit(1)


def cmd_flash_secondary(args):
    if not flash_secondary(
        signed_bin_path=args.image,
        port=args.port,
    ):
        sys.exit(1)


def cmd_flash_romfs(args):
    if not flash_romfs(
        romfs_img_path=args.image,
        reset=not args.no_reset,
    ):
        sys.exit(1)


def cmd_flash(args):
    """Flash all: internal -> wait for MCU -> secondary -> romfs."""
    print("=" * 50)
    print(" RA8P1 Full Flash")
    print("=" * 50)

    # Step 1: Internal flash (bootloader + primary)
    print("\n[1/3] Flashing internal flash...")
    if not flash_internal():
        sys.exit(1)

    # Step 2: Wait for MCU to boot and USB CDC to enumerate
    print("\n[2/3] Waiting for MCU to boot (USB CDC)...")
    # bootloader -> MCUboot -> primary -> USB CDC init takes ~5-8s
    for _ in range(15):
        port_name = find_primary_port()
        if port_name:
            print(f"  USB CDC ready: {port_name}")
            break
        time.sleep(1)
    else:
        print("  WARNING: USB CDC not detected after 15s, trying anyway...")

    port = args.port if hasattr(args, "port") else "auto"
    if not flash_secondary(port=port):
        print("WARNING: secondary flash failed, continuing...")

    # Step 3: romfs
    print("\n[3/3] Flashing romfs...")
    if not flash_romfs():
        sys.exit(1)

    print("\nAll flash operations completed.")


def cmd_update(args):
    """Update secondary + romfs, assuming MCU is already running primary.

    Prerequisite: MCU is powered on and in primary mode (USB CDC available).
    Steps: sign -> flash-secondary (Xmodem) -> flash-romfs (J-Link)
    """
    print("=" * 50)
    print(" RA8P1 Update (MCU in primary)")
    print("=" * 50)

    print("\n[1/3] Signing secondary image...")
    if not sign():
        sys.exit(1)

    print("\n[2/3] Flashing secondary via Xmodem...")
    if not flash_secondary(port=args.port, signed_bin_path=args.image):
        sys.exit(1)

    # Wait for MCU to reboot into secondary (OpenMV) mode before touching MRAM
    # Writing MRAM via J-Link while MCU is still transitioning can lock the chip
    print("\n[3/3] Flashing romfs to MRAM...")
    print("  Waiting for OpenMV USB CDC (MCU rebooting)...")
    for _ in range(30):
        port_name = find_secondary_port()
        if port_name:
            print(f"  OpenMV ready: {port_name}")
            break
        time.sleep(1)
    else:
        print("  WARNING: OpenMV CDC not detected after 30s, proceeding anyway...")

    if not flash_romfs():
        sys.exit(1)

    print("\nUpdate completed.")


def cmd_deploy(args):
    """Sign + flash all."""
    print("=" * 50)
    print(" RA8P1 Deploy (sign + flash)")
    print("=" * 50)

    print("\n[1/4] Signing secondary image...")
    if not sign():
        sys.exit(1)

    print("\n[2/4] Flashing internal flash...")
    if not flash_internal():
        sys.exit(1)

    print("\n[3/4] Waiting for MCU to boot (USB CDC)...")
    for _ in range(15):
        if find_primary_port():
            print(f"  USB CDC ready: {find_primary_port()}")
            break
        time.sleep(1)
    else:
        print("  WARNING: USB CDC not detected after 15s, trying anyway...")

    port = args.port if hasattr(args, "port") else "auto"
    if not flash_secondary(port=port):
        print("WARNING: secondary flash failed, continuing...")

    print("\n[4/4] Flashing romfs...")
    print("  Waiting for OpenMV USB CDC (MCU rebooting)...")
    for _ in range(30):
        if find_secondary_port():
            print(f"  OpenMV ready: {find_secondary_port()}")
            break
        time.sleep(1)
    else:
        print("  WARNING: OpenMV CDC not detected after 30s, proceeding anyway...")

    if not flash_romfs():
        sys.exit(1)

    print("\nDeploy completed.")


def main():
    parser = argparse.ArgumentParser(
        description="RA8P1 Unified Deployment Tool",
        formatter_class=argparse.RawDescriptionHelpFormatter,
    )
    sub = parser.add_subparsers(dest="command", required=True)

    # sign
    p = sub.add_parser("sign", help="Sign secondary image with MCUboot")
    p.add_argument("--elf", default=None, help="Path to secondary .elf")
    p.add_argument("--output", default=None, help="Path for signed output")
    p.add_argument("--no-patch-slot-size", action="store_true",
                    help="Skip SLOT_SIZE patch in bsp_linker_info.h")
    p.set_defaults(func=cmd_sign)

    # flash-internal
    p = sub.add_parser("flash-internal",
                       help="Flash bootloader+primary to internal flash (J-Link)")
    p.add_argument("--no-reset", action="store_true",
                   help="Do not reset MCU after flashing")
    p.set_defaults(func=cmd_flash_internal)

    # flash-secondary
    p = sub.add_parser("flash-secondary",
                       help="Flash secondary to OSPI via USB CDC Xmodem")
    p.add_argument("--port", default="auto",
                   help="COM port or 'auto' for VID/PID detection (default: auto)")
    p.add_argument("--image", default=None,
                   help="Path to signed secondary binary")
    p.set_defaults(func=cmd_flash_secondary)

    # flash-romfs
    p = sub.add_parser("flash-romfs", help="Flash romfs to MRAM via J-Link")
    p.add_argument("--image", default=None, help="Path to romfs0.img")
    p.add_argument("--no-reset", action="store_true",
                   help="Do not reset MCU after flashing")
    p.set_defaults(func=cmd_flash_romfs)

    # update (sign + secondary + romfs, MCU already in primary)
    p = sub.add_parser("update",
                       help="Sign + flash-secondary + flash-romfs (MCU in primary)")
    p.add_argument("--port", default="auto",
                   help="COM port or 'auto' for VID/PID detection (default: auto)")
    p.add_argument("--image", default=None,
                   help="Path to signed secondary binary (skip sign if provided)")
    p.set_defaults(func=cmd_update)

    # flash (all)
    p = sub.add_parser("flash", help="Flash all (internal + secondary + romfs)")
    p.add_argument("--port", default="auto",
                   help="COM port for secondary flash (default: auto)")
    p.set_defaults(func=cmd_flash)

    # deploy (sign + flash)
    p = sub.add_parser("deploy", help="Sign + flash all")
    p.add_argument("--port", default="auto",
                   help="COM port for secondary flash (default: auto)")
    p.set_defaults(func=cmd_deploy)

    args = parser.parse_args()
    args.func(args)


if __name__ == "__main__":
    main()
