# QVGA image-operation benchmark.
#
# One captured RGB565 frame, fixed input, no sampling from the camera during
# timing. Warmup 10, then 3 rounds x 100 samples per case. Copy, working-image
# rebuild, explicit gc and printing sit outside the timer.
#
# Working image: source.copy(). The GC heap scans its SRAM area first, so the
# buffer lands in internal SRAM when it fits and falls back to SDRAM otherwise --
# no mode switch, always the faster placement available. The printed "mem"
# column reports where it actually went.
import board
import binascii
import csi
import gc
import image
import json
import time
import uctypes

WARMUP = 10
SAMPLES = 100
REPEATS = 3
SETTLE_MS = 2000
FRAMEBUFFERS = 4
EXPORT_FIXTURES = False
EMIT_JSON = False       # per-sample records for offline analysis

# Chip memory windows, used only to label where a buffer landed.
# GC heap bounds are link-time symbols that move with the linker script; they are
# deliberately not asserted here.
SRAM_WINDOW = (0x22000000, 0x221D4000, "SRAM")
SDRAM_WINDOW = (0x68000000, 0x6A000000, "SDRAM")


def region_of(address):
    for start, end, name in (SRAM_WINDOW, SDRAM_WINDOW):
        if start <= address < end:
            return name
    return "other"


def statistics(values):
    ordered = sorted(values)
    count = len(ordered)
    middle = count // 2
    median = (ordered[middle] if count % 2 else
              (ordered[middle - 1] + ordered[middle]) / 2)
    return {"mean": sum(values) / count, "p50": median,
            "p95": ordered[(95 * count + 99) // 100 - 1],
            "min": ordered[0], "max": ordered[-1]}


def emit(record):
    if EMIT_JSON:
        print("@BENCH " + json.dumps(record))
        time.sleep_ms(2)


def export_fixture(name, img):
    data = bytes(img)
    for offset in range(0, len(data), 128):
        encoded = binascii.hexlify(data[offset:offset + 128]).decode()
        print("@FIXTURE %s %d %s" % (name, offset, encoded))
        time.sleep_ms(2)
    print("@FIXTURE_END %s %d" % (name, len(data)))
    time.sleep_ms(2)


def run_case(case_name, source, operation, parameters):
    durations = [0] * SAMPLES
    measured = 0
    warmup_completed = 0
    frame = None
    output = None
    failure = None
    address = 0
    try:
        for index in range(WARMUP + SAMPLES):
            gc.collect()
            frame = source.copy()
            address = uctypes.addressof(frame)
            start = time.ticks_us()
            output = operation(frame)
            duration = time.ticks_diff(time.ticks_us(), start)
            if duration < 0:
                raise ValueError("Timed call exceeded the ticks_diff range")
            if index < WARMUP:
                warmup_completed += 1
            else:
                durations[measured] = duration
                measured += 1
            output = None
            frame = None
    except BaseException as error:
        failure = error
    finally:
        frame = None
        output = None

    values = durations[:measured]
    record = {"type": "case", "case": case_name,
              "working_address": hex(address), "working_region": region_of(address),
              "status": "pass" if failure is None else "error",
              "parameters": parameters,
              "warmup_completed": warmup_completed, "samples": measured,
              "samples_us": values, "statistics_us": statistics(values)}
    if failure is not None:
        record["error"] = repr(failure)
    emit(record)
    return record


def main():
    previous_preview = board.preview(False)
    previous_pipeline = None
    try:
        previous_pipeline = board.pipeline(False)
        camera = csi.CSI()
        camera.reset()
        camera.pixformat(csi.RGB565)
        camera.framesize(csi.QVGA)
        camera.framebuffers(FRAMEBUFFERS)
        camera.snapshot(time=SETTLE_MS)
        rgb = camera.snapshot().copy()
        if (rgb.width(), rgb.height(), rgb.format()) != (320, 240, image.RGB565):
            raise RuntimeError("Unexpected captured image format")

        gray = rgb.to_grayscale(copy=True)
        reference = gray.copy()
        reference.invert()
        kernel = (0, -1, 0, -1, 5, -1, 0, -1, 0)

        if EXPORT_FIXTURES:
            export_fixture("rgb565", rgb)
            export_fixture("gray", gray)
            export_fixture("gray_inverted", reference)

        cases = (
            ("invert", "gray", lambda img: img.invert()),
            ("difference", "gray", lambda img: img.difference(reference)),
            ("erode_3x3", "gray", lambda img: img.erode(1, threshold=0)),
            ("dilate_3x3", "gray", lambda img: img.dilate(1, threshold=0)),
            ("mean_3x3", "gray", lambda img: img.mean(1)),
            ("morph_sharpen_3x3", "gray", lambda img: img.morph(1, kernel, mul=1.0, add=0.0)),
            ("jpeg_q90", "gray", lambda img: img.compress(
                quality=90, copy=False, subsampling=image.JPEG_SUBSAMPLING_AUTO)),
            ("jpeg_q90", "rgb565", lambda img: img.compress(
                quality=90, copy=False, subsampling=image.JPEG_SUBSAMPLING_AUTO)),
            ("to_grayscale", "rgb565", lambda img: img.to_grayscale(copy=False)),
        )
        sources = {"gray": gray, "rgb565": rgb}

        print("QVGA 320x240  warmup=%d  samples=%d rounds=%d" % (
            WARMUP, SAMPLES, REPEATS))
        print("preview=off  pipeline=off  working image=source.copy()")
        print("%-20s %-7s %8s %8s %8s %6s" % (
            "case", "fmt", "mean_ms", "p50_ms", "p95_ms", "mem"))

        results = []
        for case_name, fmt, operation in cases:
            rows = [run_case(case_name, sources[fmt], operation, {"fmt": fmt})
                    for _ in range(REPEATS)]
            failed = [r for r in rows if r["status"] != "pass"]
            if failed:
                print("%-20s %-7s ERROR %s" % (
                    case_name, fmt, failed[0].get("error")))
                time.sleep_ms(5)
                results.append((case_name, fmt, None, None))
                continue
            means = [r["statistics_us"]["mean"] for r in rows]
            p50 = [r["statistics_us"]["p50"] for r in rows]
            p95 = [r["statistics_us"]["p95"] for r in rows]
            mem = rows[0]["working_region"]
            mean = sum(means) / len(means)
            print("%-20s %-7s %8.3f %8.3f %8.3f %6s" % (
                case_name, fmt, mean / 1000.0,
                (sum(p50) / len(p50)) / 1000.0,
                (sum(p95) / len(p95)) / 1000.0, mem))
            time.sleep_ms(5)
            results.append((case_name, fmt, mean / 1000.0, mem))

        print("done: %d cases x %d samples x %d rounds" % (
            len(cases), SAMPLES, REPEATS))
    finally:
        try:
            if previous_pipeline is not None:
                board.pipeline(previous_pipeline)
        finally:
            board.preview(previous_preview)


main()
