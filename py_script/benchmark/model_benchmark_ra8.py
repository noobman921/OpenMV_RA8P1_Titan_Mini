# SPDX-License-Identifier: Apache-2.0
# RA8P1 神经网络模型性能测试（SD 卡 Vela 版）
#
# 对齐 docs/benchmark.md 第 2 节的计时口径：
#   「计时包含输入复制、推理和输出回调，不含相机采集、图像预处理、检测后处理与绘图」
#
# 为此做三件事：
#   1. 输入预先缩放到模型自身输入尺寸（model.input_shape），放在计时区外。
#      这样 predict() 内部的 Normalization 只做 1:1 复制，不再做 VGA->小图的
#      双线性缩放 —— 那一步属于「图像预处理」，不该计入。
#   2. 主测值（aligned）不挂后处理，对应口径里的「不含检测后处理」。
#   3. 额外单测一列 with_pp（挂后处理），用来量化后处理的开销，
#      两者之差即「检测后处理」占用。
#
# 预热 5 次、3 轮各采样 30 次，与 docs/benchmark.md 一致。
#
# 覆盖 docs/performance.md「神经网络示例模型推理耗时」表的全部 21 个模型。
# 模型文件来自 py_script/models/<脚本名>.tflite，拷到 /sdcard/models/ 即可。
# 后处理参数对齐 OpenMV 官方示例，统一 threshold=0.4。
# 固件指纹：omv_ra8p1_xip.srec SHA-256 前缀 9ac90634（换固件后要更新）。
# 只跑子集时改 MODEL_FILTER（按名字子串匹配）。

import gc
import time

import image
import ml

# ---------- 测法参数（与 docs/benchmark.md 一致） ----------
WARMUP = 5
SAMPLES = 30
REPEATS = 3
THRESHOLD = 0.4

# 相机取景尺寸：统一抓一帧最大尺寸，供各模型自行缩放到自己的输入尺寸。
# 示例脚本里 400x400 与 QVGA/VGA 混用，这里取 VGA 作为公共源，差异只体现在
# 单独测的「预处理」列里，不影响 aligned 主测值。
CAM_WIDTH = 640
CAM_HEIGHT = 480

SD = "/sdcard/models/"

# ---------- 后处理类注册表 ----------
# 逐个 try：任一模块缺失不影响其它模型。
_PP = {}
try:
    from ml.postprocessing.edgeimpulse import Fomo
    _PP["fomo"] = Fomo
except ImportError:
    pass
try:
    from ml.postprocessing.mediapipe import (BlazeFace, BlazePalm, FaceLandmarks,
                                             HandLandmarks, MoveNet)
    _PP["blazeface"] = BlazeFace
    _PP["blazepalm"] = BlazePalm
    _PP["facelandmarks"] = FaceLandmarks
    _PP["handlandmarks"] = HandLandmarks
    _PP["movenet"] = MoveNet
except ImportError:
    pass
try:
    from ml.postprocessing.darknet import YoloLC
    _PP["yololc"] = YoloLC
except ImportError:
    pass
try:
    from ml.postprocessing.ultralytics import YoloV8
    _PP["yolov8"] = YoloV8
except ImportError:
    pass

# 数值回归模型的固定输入（取自 OpenMV tf_regression.py 示例）
REGRESSION_INPUT = (
    -3, -1, -2, 5, -2, 10, -1, 9, 0,
    2, 0, 9, 1, 10, 2, -1, 3, 5,
    3, 9, 3, 9, 6, 2, 6, 7, 5,
    10, 6, -1, 7, 4, 7, 8, 5, 7,
)

# ---------- 模型清单 ----------
# 顺序与 Titan_Mini_Openmv/docs/performance.md 的「神经网络示例模型推理耗时」表一致。
# input = "image" 图像输入 / "ndarray" 数值输入（TF 回归模型，不能喂图）
# postprocess 指向 _PP 的键；None 表示没有可用的 ml 内置后处理（不影响 aligned 主测值）
MODELS = (
    {"name": "FOMO 人脸中心检测", "file": "fomo_sdcard_test.tflite", "postprocess": "fomo"},
    {"name": "BlazeFace FLOAT32", "file": "blazeface_sdcard_test.tflite", "postprocess": "blazeface"},
    {"name": "BlazeFace Renesas INT8", "file": "blazeface_renesas_sdcard_test.tflite", "postprocess": "blazeface"},
    {"name": "BlazePalm 手掌检测", "file": "palm_detection_sdcard_test.tflite", "postprocess": "blazepalm"},
    {"name": "Face Landmarks 468 点", "file": "face_landmarks_sdcard_test.tflite", "postprocess": "facelandmarks"},
    {"name": "Hand Landmarks 21 点", "file": "hand_landmarks_sdcard_test.tflite", "postprocess": "handlandmarks"},
    {"name": "MoveNet 单人姿态", "file": "movenet_sdcard_test.tflite", "postprocess": "movenet"},
    {"name": "YOLO LC 行人检测", "file": "yolo_lc_sdcard_test.tflite", "postprocess": "yololc"},
    {"name": "YOLOv8n person", "file": "yolov8n_sdcard_test.tflite", "postprocess": "yolov8"},
    {"name": "YOLOv8n COCO80", "file": "yolov8n_coco80_sdcard_test.tflite", "postprocess": "yolov8"},
    {"name": "YOLOv8n Pose", "file": "yolov8n_pose_sdcard_test.tflite", "postprocess": None},
    {"name": "YOLOv5n6-xiao VOC20", "file": "yolov5n6_xiao_voc20_sdcard_test.tflite", "postprocess": None},
    {"name": "YOLO-Fastest 灰度人脸", "file": "yolo_fastest_face_sdcard_test.tflite", "postprocess": None},
    {"name": "Swift-YOLO Gesture", "file": "swift_yolo_gesture_sdcard_test.tflite", "postprocess": None},
    {"name": "Swift-YOLO Pet", "file": "swift_yolo_pet_sdcard_test.tflite", "postprocess": None},
    {"name": "Swift-YOLO Nano Person", "file": "swift_yolo_person_sdcard_test.tflite", "postprocess": None},
    {"name": "U-Net MobileNetV2", "file": "unet_pet_sdcard_test.tflite", "postprocess": None},
    {"name": "VWW 人体存在分类", "file": "vww96_sdcard_test.tflite", "postprocess": None},
    {"name": "ResNet8 CIFAR10", "file": "resnet8_sdcard_test.tflite", "postprocess": None},
    {"name": "MobileNet V1 0.25", "file": "mobilenetv1_025_sdcard_test.tflite", "postprocess": None},
    {"name": "数值回归（CPU）", "file": "force_int_quant.tflite", "postprocess": None, "input": "ndarray"},
)

# 只跑其中一部分时改这里：None = 全部
MODEL_FILTER = None


# ---------- 工具 ----------

def file_exists(path):
    try:
        handle = open(path, "rb")
        handle.close()
        return True
    except OSError:
        return False


def make_source():
    """抓一帧作为所有模型的公共输入源。不参与计时。"""
    try:
        import sensor
        sensor.reset()
        sensor.set_framesize(sensor.QVGA)
        sensor.set_pixformat(sensor.RGB565)
        sensor.skip_frames(time=1000)
        img = sensor.snapshot().copy()
        print("source: camera %dx%d RGB565" % (img.width(), img.height()))
        time.sleep_ms(2)
        return img
    except BaseException as error:
        print("source: camera unavailable (%s)" % error)
        print("source: using a synthetic frame")

    img = image.Image(CAM_WIDTH, CAM_HEIGHT, image.RGB565)
    img.draw_rectangle((0, 0, CAM_WIDTH, CAM_HEIGHT), fill=True, color=(100, 120, 140))
    for y in range(0, CAM_HEIGHT, 64):
        img.draw_rectangle((40, y, CAM_WIDTH - 80, 26), fill=True,
                           color=(60 + y % 180, 90 + y % 120, 200 - y % 150))
    print("source: synthetic %dx%d RGB565" % (img.width(), img.height()))
    time.sleep_ms(2)
    return img


def presize(src, width, height, grayscale):
    """把公共源缩放到模型输入尺寸。

    这一步等价于 ml.preprocessing.Normalization 内部的 draw_image，
    也就是口径里被排除的「图像预处理」。放在计时区外。
    """
    pixfmt = image.GRAYSCALE if grayscale else image.RGB565
    out = image.Image(width, height, pixfmt)
    hints = image.BILINEAR | image.CENTER | image.SCALE_ASPECT_EXPAND | image.BLACK_BACKGROUND
    out.draw_image(src, 0, 0, hint=hints)
    return out


def make_array_input(model):
    """TF 回归模型的定长数值输入。"""
    from ulab import numpy as np
    return np.array(REGRESSION_INPUT, dtype=np.int8).reshape(model.input_shape[0])


def timed(call, warmup=WARMUP, samples=SAMPLES, repeats=REPEATS):
    """返回一次调用的平均耗时(us)。"""
    for _ in range(warmup):
        call()
    total = 0
    for _ in range(repeats):
        for _ in range(samples):
            start = time.ticks_us()
            call()
            total += time.ticks_diff(time.ticks_us(), start)
    return total / float(samples * repeats)


def free_kb():
    return gc.mem_free() // 1024


def load_model(path, key):
    postprocess = _PP[key](threshold=THRESHOLD) if key else None
    return ml.Model(path, postprocess=postprocess)


def run_one(entry, source):
    """返回结果 dict。任何失败都不抛出。"""
    path = SD + entry["file"]
    if not file_exists(path):
        return {"status": "SKIP", "detail": "no %s" % path}

    key = entry.get("postprocess")
    if key is not None and key not in _PP:
        return {"status": "SKIP", "detail": "postprocessor '%s' unavailable" % key}

    model = None
    try:
        gc.collect()
        model = load_model(path, None)
        shape = model.input_shape[0]
        height, width, channels = shape[1], shape[2], shape[3]
        result = {"status": "OK", "shape": str(shape)}

        if entry.get("input") == "ndarray":
            data = make_array_input(model)
            result["pre_us"] = None
            result["aligned_us"] = timed(lambda m=model, d=data: m.predict([d]))
        else:
            grayscale = (channels == 1)
            # 预处理开销：公共源 -> 模型输入尺寸，单独计时（口径里不计入）
            result["pre_us"] = timed(
                lambda: presize(source, width, height, grayscale),
                warmup=1, samples=5, repeats=1)
            data = presize(source, width, height, grayscale)
            result["aligned_us"] = timed(lambda m=model, d=data: m.predict([d]))
        del model
        model = None
        gc.collect()

        # 后处理开销：同一模型挂上后处理再测一次，差值即「检测后处理」
        if key is not None:
            model = load_model(path, key)
            if entry.get("input") == "ndarray":
                data = make_array_input(model)
            else:
                data = presize(source, width, height, channels == 1)
            result["withpp_us"] = timed(lambda m=model, d=data: m.predict([d]))
            del model
            model = None
            gc.collect()

        return result
    except BaseException as error:
        return {"status": "FAIL", "detail": repr(error)}
    finally:
        model = None
        gc.collect()


def ms(us):
    return "   -    " if us is None else "%8.3f" % (us / 1000.0)


def main():
    print("=== RA8P1 model benchmark (sdcard vela) ===")
    print("firmware srec SHA-256 prefix: 9ac90634")
    print("warmup=%d samples=%d repeats=%d threshold=%.2f" % (
        WARMUP, SAMPLES, REPEATS, THRESHOLD))
    print("free heap: %d KB" % free_kb())
    time.sleep_ms(2)

    source = make_source()
    print("aligned  = input copy + inference + output conversion (no postprocess)")
    print("with_pp  = same, postprocessor attached")
    print("pre      = source -> model input resize (excluded from aligned)")
    time.sleep_ms(2)

    print("")
    targets = [e for e in MODELS
               if MODEL_FILTER is None or MODEL_FILTER in e["name"]]
    print("--- %d models ---" % len(targets))
    time.sleep_ms(2)

    rows = []
    for entry in targets:
        result = run_one(entry, source)
        rows.append((entry, result))
        if result["status"] == "OK":
            print("%-20s %-22s aligned %s ms  with_pp %s ms  heap %d KB" % (
                entry["name"], result["shape"], ms(result["aligned_us"]),
                ms(result.get("withpp_us")), free_kb()))
        else:
            print("%-20s %s %s" % (entry["name"], result["status"], result["detail"]))
        time.sleep_ms(2)

    print("")
    print("=== summary (ms per call) ===")
    print("%-20s %-22s %8s %8s %8s %8s" % (
        "model", "input", "pre", "aligned", "with_pp", "pp_delta"))
    time.sleep_ms(2)

    ok = 0
    for entry, result in rows:
        if result["status"] != "OK":
            print("%-20s %-22s %8s" % (entry["name"], "-", result["status"]))
            time.sleep_ms(2)
            continue
        ok += 1
        aligned = result["aligned_us"]
        withpp = result.get("withpp_us")
        delta = None if withpp is None else withpp - aligned
        print("%-20s %-22s %s %s %s %s" % (
            entry["name"], result["shape"], ms(result["pre_us"]),
            ms(aligned), ms(withpp), ms(delta)))
        time.sleep_ms(2)

    print("")
    print("measured %d of %d models, %d calls each" % (
        ok, len(MODELS), SAMPLES * REPEATS))
    print("aligned is the number comparable to docs/benchmark.md section 2")


main()
