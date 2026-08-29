"""Flash secondary image to OSPI via USB CDC Xmodem.

Automates the full menu interaction:
  1. Open USB CDC port (auto-detect by VID/PID or manual COM port)
  2. Trigger menu prompt
  3. Send '2' to start Xmodem download
  4. Transfer signed secondary image via Xmodem-128
  5. Wait for MCU reset (USB CDC disconnect is expected)
"""

import os
import sys
import time

import serial
import serial.tools.list_ports
import xmodem

from . import config


def find_primary_port():
    """Auto-detect RA8P1 primary USB CDC port by VID/PID.

    Returns:
        COM port name (e.g. 'COM7') or None if not found
    """
    for p in serial.tools.list_ports.comports():
        if p.vid == config.USB_VID_PRIMARY and p.pid == config.USB_PID_PRIMARY:
            return p.device
    return None


def find_secondary_port():
    """Auto-detect RA8P1 secondary (OpenMV) USB CDC port by VID/PID.

    Returns:
        COM port name (e.g. 'COM10') or None if not found
    """
    for p in serial.tools.list_ports.comports():
        if p.vid == config.USB_VID_SECONDARY and p.pid == config.USB_PID_SECONDARY:
            return p.device
    return None


def _open_port(port, retries=5, retry_delay=2):
    """Open serial port with retries.

    Args:
        port: COM port name, or 'auto' to auto-detect
        retries: number of open attempts
        retry_delay: seconds between retries

    Returns:
        serial.Serial instance or None
    """
    if port == "auto":
        port = find_primary_port()
        if port is None:
            print("  Auto-detect: no primary USB CDC device found")
            return None
        print(f"  Auto-detect: found {port}")

    for attempt in range(retries):
        try:
            ser = serial.Serial(port, config.BAUDRATE, timeout=5)
            print(f"  Opened {port}")
            return ser
        except serial.SerialException as e:
            print(f"  Retry {attempt+1}/{retries}: {e}")
            time.sleep(retry_delay)

    print(f"  Failed to open {port}")
    return None


def _drain(ser, timeout=2):
    """Read and discard buffered data."""
    old = ser.timeout
    ser.timeout = timeout
    data = ser.read(4096)
    ser.timeout = old
    return data


def _wait_for(ser, target, timeout=30):
    """Wait until serial output contains target string.

    Returns:
        accumulated text received
    """
    buf = b""
    deadline = time.time() + timeout
    while time.time() < deadline:
        ser.timeout = max(min(deadline - time.time(), 1.0), 0.1)
        chunk = ser.read(256)
        if chunk:
            buf += chunk
            if target.encode("ascii", errors="replace") in buf:
                return buf.decode("ascii", errors="replace")
    return buf.decode("ascii", errors="replace")


def flash_secondary(signed_bin_path=None, port="auto"):
    """Flash secondary image to OSPI via Xmodem over USB CDC.

    Args:
        signed_bin_path: path to signed secondary binary.
                         Defaults to config.SECONDARY_SIGNED.
        port: COM port name or 'auto' for VID/PID auto-detect.

    Returns:
        True on success, False on failure
    """
    if signed_bin_path is None:
        signed_bin_path = config.SECONDARY_SIGNED

    if not os.path.exists(signed_bin_path):
        print(f"ERROR: signed image not found: {signed_bin_path}")
        return False

    file_size = os.path.getsize(signed_bin_path)
    print(f"Secondary image: {file_size} bytes ({file_size/1024:.1f} KB)")

    # Step 1: Open port
    print("[1/5] Opening USB CDC port...")
    ser = _open_port(port)
    if not ser:
        return False

    try:
        # Step 2: Trigger menu
        print("[2/5] Triggering menu...")
        ser.write(b"\r\n")
        time.sleep(1)
        text = _drain(ser, 2).decode("ascii", errors="replace")

        if ">" not in text:
            ser.write(b"\r\n")
            time.sleep(1)
            text = _drain(ser, 2).decode("ascii", errors="replace")

        if ">" not in text:
            print("  WARNING: no menu prompt '>' seen")

        # Step 3: Send '2' to trigger Xmodem download
        print("[3/5] Triggering Xmodem download (menu option 2)...")
        ser.write(b"2")

        # Step 4: Wait for MCU Xmodem ready
        text = _wait_for(ser, "Xmodem", timeout=30)
        if "Xmodem" not in text:
            print("  WARNING: no Xmodem prompt seen, continuing...")

        # Step 5: Xmodem transfer
        print("[4/5] Xmodem transfer in progress...")
        # Xmodem-128: 132 bytes/frame (3 hdr + 128 data + 1 cksum)
        num_frames = (file_size + 127) // 128
        total_tx = num_frames * 132  # expected total bytes sent including protocol overhead
        progress = [0]
        last_print = [0.0]
        start_time = [0.0]

        def getc(size, timeout=10):
            old = ser.timeout
            ser.timeout = timeout
            data = ser.read(size)
            ser.timeout = old
            return data or None

        def putc(data, timeout=10):
            n = ser.write(data)
            progress[0] += n
            now = time.time()
            if now - last_print[0] > 2.0:
                elapsed = now - start_time[0]
                pct = progress[0] * 100 / total_tx
                speed = progress[0] / elapsed / 1024 if elapsed > 0 else 0
                print(
                    f"  {progress[0]/1024:.0f}/{total_tx/1024:.0f} KB "
                    f"({pct:.1f}%)  {speed:.1f} KB/s"
                )
                last_print[0] = now
            return n

        with open(signed_bin_path, "rb") as f:
            modem = xmodem.XMODEM(getc, putc, mode="xmodem")
            start_time[0] = time.time()
            try:
                result = modem.send(f)
                elapsed = time.time() - start_time[0]
            except serial.SerialException:
                # MCU reset after transfer complete — this is expected
                # Xmodem library may raise when reading ACK after final EOT
                result = True
                elapsed = time.time() - start_time[0]

        if result:
            speed = file_size / elapsed / 1024 if elapsed > 0 else 0
            print(f"  Xmodem transfer OK  ({elapsed:.1f}s, {speed:.1f} KB/s)")
        else:
            print("  Xmodem transfer FAILED")
            return False

        # Step 5: Wait for MCU reset
        print("[5/5] MCU resetting (USB CDC will disconnect)...")
        time.sleep(3)
        try:
            _drain(ser, 2)
        except serial.SerialException:
            pass  # expected: MCU reset disconnects USB CDC

        print("Secondary image flashed successfully.")
        return True

    except Exception as e:
        print(f"ERROR: {e}")
        return False
    finally:
        try:
            ser.close()
        except Exception:
            pass
