"""J-Link Commander wrapper.

Generates and executes J-Link Command Files for:
  - loadfile (S-Record to internal flash)
  - loadbin (binary to MRAM)
  - mem32  (memory read for verification)
"""

import os
import subprocess
import tempfile
from . import config


def _run_jlink(commands, exit_on_error=True):
    """Execute a list of J-Link Commander commands.

    Args:
        commands: list of command strings (without newlines)
        exit_on_error: pass -ExitOnError 1 to J-Link

    Returns:
        subprocess.CompletedProcess
    """
    script = "\n".join(commands) + "\n"
    with tempfile.NamedTemporaryFile(
        mode="w", suffix=".jlink", delete=False, encoding="ascii"
    ) as f:
        f.write(script)
        script_path = f.name

    try:
        args = [config.JLINK_EXE, "-CommandFile", script_path]
        if exit_on_error:
            args += ["-ExitOnError", "1"]
        result = subprocess.run(args, capture_output=True, text=True, timeout=120)
        return result
    finally:
        os.unlink(script_path)


def _connect_header():
    """Standard J-Link connect preamble."""
    return [
        f"device {config.JLINK_DEVICE}",
        "si SWD",
        f"speed {config.JLINK_SPEED}",
        "connect",
    ]


def loadfile(srec_path):
    """Flash an S-Record file to internal flash via J-Link loadfile.

    Args:
        srec_path: path to .srec file

    Returns:
        subprocess.CompletedProcess
    """
    srec_path = os.path.normpath(srec_path)
    if not os.path.exists(srec_path):
        raise FileNotFoundError(f"S-Record not found: {srec_path}")

    commands = _connect_header() + [
        f"loadfile {srec_path}",
        "r",
        "g",
        "exit",
    ]
    return _run_jlink(commands)


def loadbin(bin_path, address, reset=True):
    """Flash a binary file to memory via J-Link loadbin.

    Args:
        bin_path: path to .bin file
        address: target address (e.g. 0x020D0000 for MRAM)
        reset: whether to reset and go after loading

    Returns:
        subprocess.CompletedProcess
    """
    bin_path = os.path.normpath(bin_path)
    if not os.path.exists(bin_path):
        raise FileNotFoundError(f"Binary not found: {bin_path}")

    commands = _connect_header() + [
        f"loadbin {bin_path} {address:#x}",
    ]
    if reset:
        commands += ["r", "g"]
    commands += ["exit"]
    return _run_jlink(commands)


def loadfile_multi(srec_paths, reset=True):
    """Flash multiple S-Record files sequentially.

    Args:
        srec_paths: list of .srec file paths
        reset: whether to reset and go after all files

    Returns:
        subprocess.CompletedProcess
    """
    for p in srec_paths:
        if not os.path.exists(p):
            raise FileNotFoundError(f"S-Record not found: {p}")

    commands = _connect_header()
    for p in srec_paths:
        commands.append(f"loadfile {os.path.normpath(p)}")
    if reset:
        commands += ["r", "g"]
    commands += ["exit"]
    return _run_jlink(commands)


def mem32(address, num_words=1):
    """Read 32-bit words from memory via J-Link mem32.

    Args:
        address: start address
        num_words: number of 32-bit words to read

    Returns:
        subprocess.CompletedProcess
    """
    commands = _connect_header() + [
        f"mem32 {address:#x} {num_words}",
        "exit",
    ]
    return _run_jlink(commands, exit_on_error=False)
