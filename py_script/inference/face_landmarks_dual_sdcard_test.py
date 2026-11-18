# Face Landmarks 双模型（BlazeFace 检测 -> 468 关键点）- SD Card 版本
#
# 和 face_landmarks_sdcard_test.py（单模型、整帧跑关键点）并列的新脚本，旧的保留。
# 流程对齐上游 face_landmarks_single_face.py：
#   blazeface 找人脸 -> ROI 放大 2 倍 -> face_landmarks 在 ROI 上出 468 点
#   -> 之后每帧用关键点框重新居中 ROI，跟丢（无检测）就回到 blazeface 检测。
#
# 模型放在 /sdcard/models/：
#   blazeface_sdcard_test.tflite        (~165KB)
#   face_landmarks_sdcard_test.tflite   (~707KB)
#
# 想要更细的关键点可以把帧尺寸换成 sensor.VGA（旧的单模型脚本就是 VGA）。

import ml
import sensor
import time
from ml.preprocessing import Normalization
from ml.postprocessing.mediapipe import BlazeFace, FaceLandmarks

DET_MODEL = "/sdcard/models/blazeface_sdcard_test.tflite"
FACE_MODEL = "/sdcard/models/face_landmarks_sdcard_test.tflite"
DET_THRESHOLD = 0.4
FACE_THRESHOLD = 0.4


def clamp_rect(r, w, h):
    # 把框裁到图像内；完全落在图像外时返回 None（NMS 的 roi 宽高必须 >= 1）
    x1 = max(0, int(r[0]))
    y1 = max(0, int(r[1]))
    x2 = min(w, int(r[0] + r[2]))
    y2 = min(h, int(r[1] + r[3]))
    if x2 <= x1 or y2 <= y1:
        return None
    return (x1, y1, x2 - x1, y2 - y1)


def expand_rect(r, factor, w, h):
    # 以框中心放大 factor 倍：关键点模型需要比检测框更宽的视野
    cx = r[0] + r[2] / 2.0
    cy = r[1] + r[3] / 2.0
    rw = r[2] * factor
    rh = r[3] * factor
    return clamp_rect((cx - rw / 2.0, cy - rh / 2.0, rw, rh), w, h)


def flat(dets):
    # BlazeFace 返回扁平列表，FaceLandmarks 按版本可能返回分组列表，统一摊平
    if dets and isinstance(dets[0], list):
        out = []
        for group in dets:
            out.extend(group)
        return out
    return dets


sensor.reset()
sensor.set_pixformat(sensor.RGB565)
sensor.set_framesize(sensor.QVGA)
sensor.skip_frames(time=1000)

print("=" * 50)
# 先加载关键点模型：让它先分配 NPU arena
face_model = ml.Model(FACE_MODEL, postprocess=FaceLandmarks(threshold=FACE_THRESHOLD))
print(face_model)
det_model = ml.Model(DET_MODEL, postprocess=BlazeFace(threshold=DET_THRESHOLD))
print(det_model)
print("=" * 50)

roi = None          # 当前跟踪框；None 表示需要重新用人脸检测
frame_count = 0
time_list = []
fps_list = []
detect_count = 0

while True:
    t_start = time.ticks_ms()
    img = sensor.snapshot()
    w = img.width()
    h = img.height()

    if roi is None:
        # 检测帧：找人脸建立 ROI，这一帧不跑关键点
        detect_count = 0
        for r, score, _kp in flat(det_model.predict([img])):
            roi = expand_rect(r, 2.0, w, h)
            if roi is not None:
                break
        if roi is None:
            img.draw_string((4, 4), "No face", color=(255, 0, 0), scale=2)
    else:
        # 跟踪帧：只在 ROI 上跑关键点
        faces = flat(face_model.predict([Normalization(roi=roi)(img)]))
        if not faces:
            roi = None          # 跟丢了 -> 下一帧重新检测
        else:
            detect_count = len(faces)
            for r, score, keypoints in faces:
                img.draw_rectangle(r, color=(0, 0, 255), thickness=2)
                for i in range(len(keypoints)):
                    img.draw_circle((int(keypoints[i][0]), int(keypoints[i][1]), 1),
                                    color=(255, 0, 0), fill=True)
                # 用关键点框重新居中 ROI，下一帧继续跟
                roi = expand_rect(r, 2.0, w, h)
            img.draw_string((4, 4), "Faces: %d" % detect_count, color=(0, 255, 0), scale=2)

    t_end = time.ticks_ms()
    elapsed = time.ticks_diff(t_end, t_start)
    time_list.append(elapsed)

    if elapsed > 0:
        fps = 1000.0 / elapsed
        fps_list.append(fps)

    frame_count += 1

    if frame_count % 30 == 0:
        avg_time = sum(time_list) / len(time_list)
        avg_fps = sum(fps_list) / len(fps_list)
        min_time = min(time_list)
        max_time = max(time_list)
        max_fps = 1000.0 / min_time if min_time > 0 else 0
        min_fps = 1000.0 / max_time if max_time > 0 else 0

        print("[%4d 帧] 平均: %.2f fps (%.1f ms) | 范围: %.1f ~ %.1f fps | 检测: %d" %
              (frame_count, avg_fps, avg_time, min_fps, max_fps, detect_count))

        if frame_count >= 100:
            time_list = time_list[-30:]
            fps_list = fps_list[-30:]
