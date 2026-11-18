# Hand Landmarks 双模型（Palm 检测 -> 21 关键点）- SD Card 版本
#
# 和 hand_landmarks_sdcard_test.py（单模型、整帧跑关键点）并列的新脚本，旧的保留。
# 流程对齐上游 hand_landmarks_single_hand.py：
#   palm_detection 找手 -> ROI 放大 3 倍 -> hand_landmarks 在 ROI 上出 21 点
#   -> 之后每帧用关键点框重新居中 ROI，跟丢（无检测）就回到 palm 检测。
#
# 模型放在 /sdcard/models/：
#   palm_detection_sdcard_test.tflite   (~1.28MB)
#   hand_landmarks_sdcard_test.tflite   (~2.0MB)

import ml
import sensor
import time
from ml.preprocessing import Normalization
from ml.postprocessing.mediapipe import BlazePalm, HandLandmarks

PALM_MODEL = "/sdcard/models/palm_detection_sdcard_test.tflite"
HAND_MODEL = "/sdcard/models/hand_landmarks_sdcard_test.tflite"
PALM_THRESHOLD = 0.5
HAND_THRESHOLD = 0.4

# 21 个关键点之间的连线（MediaPipe 手部骨架）
HAND_LINES = ((0, 1), (1, 2), (2, 3), (3, 4),
              (0, 5), (5, 6), (6, 7), (7, 8),
              (5, 9), (9, 10), (10, 11), (11, 12),
              (9, 13), (13, 14), (14, 15), (15, 16),
              (13, 17), (17, 18), (18, 19), (19, 20),
              (0, 17))


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
    # BlazePalm 返回扁平列表，HandLandmarks 返回按类别分组的列表，统一摊平
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
hand_model = ml.Model(HAND_MODEL, postprocess=HandLandmarks(threshold=HAND_THRESHOLD))
print(hand_model)
palm_model = ml.Model(PALM_MODEL, postprocess=BlazePalm(threshold=PALM_THRESHOLD))
print(palm_model)
print("=" * 50)

roi = None          # 当前跟踪框；None 表示需要重新用 palm 检测
frame_count = 0
time_list = []
fps_list = []
detect_count = 0

# 分步计时（µs 累计）。只统计不打印时就设 False，不影响主流程
PROFILE = True
acc_snap = acc_detect = acc_track = acc_draw = 0
n_detect = n_track = 0

while True:
    t_start = time.ticks_us()
    img = sensor.snapshot()
    w = img.width()
    h = img.height()
    t_model0 = time.ticks_us()

    if roi is None:
        # 检测帧：找手建立 ROI，这一帧不跑关键点
        det_frame = True
        palms = flat(palm_model.predict([img]))
    else:
        # 跟踪帧：只在 ROI 上跑关键点
        det_frame = False
        hands = flat(hand_model.predict([Normalization(roi=roi)(img)]))
    t_model1 = time.ticks_us()

    if det_frame:
        detect_count = 0
        roi = None
        for r, score, _kp in palms:
            roi = expand_rect(r, 3.0, w, h)
            if roi is not None:
                break
        if roi is None:
            img.draw_string((4, 4), "No hand", color=(255, 0, 0), scale=2)
    elif not hands:
        roi = None              # 跟丢了 -> 下一帧重新检测
    else:
        detect_count = len(hands)
        for r, score, keypoints in hands:
            img.draw_rectangle(r, color=(0, 255, 0), thickness=2)
            for i in range(len(keypoints)):
                img.draw_circle((int(keypoints[i][0]), int(keypoints[i][1]), 2),
                                color=(255, 0, 0), fill=True)
            for a, b in HAND_LINES:
                img.draw_line((int(keypoints[a][0]), int(keypoints[a][1]),
                               int(keypoints[b][0]), int(keypoints[b][1])),
                              color=(255, 255, 0), thickness=1)
            # 用关键点框重新居中 ROI，下一帧继续跟
            roi = expand_rect(r, 2.0, w, h)
        img.draw_string((4, 4), "Hands: %d" % detect_count, color=(0, 255, 0), scale=2)
    t_end = time.ticks_us()

    d_snap = time.ticks_diff(t_model0, t_start)     # 取帧
    d_model = time.ticks_diff(t_model1, t_model0)   # palm 或 hand 推理
    d_draw = time.ticks_diff(t_end, t_model1)       # 绘制 + ROI 更新
    d_total = time.ticks_diff(t_end, t_start)

    if PROFILE:
        acc_snap += d_snap
        acc_draw += d_draw
        if det_frame:
            acc_detect += d_model
            n_detect += 1
        else:
            acc_track += d_model
            n_track += 1

    elapsed = d_total // 1000
    time_list.append(elapsed)

    if elapsed > 0:
        fps_list.append(1000.0 / elapsed)

    frame_count += 1

    if (frame_count % 30 == 0) and PROFILE:
        # 各段取本窗口内的单次平均值（ms）；palm / hand 的次数可能不同，分别给次数
        n = min(frame_count, 30)
        avg_ms = sum(time_list[-n:]) / float(n)
        avg_fps = sum(fps_list[-n:]) / float(len(fps_list[-n:])) if fps_list else 0.0

        def avg(v, c):
            return (v / c / 1000.0) if c else 0.0

        print("[%4d 帧] %.2f fps (%.1f ms/帧) | snap %.1f | palm %.1f×%d | hand %.1f×%d | draw %.1f ms" %
              (frame_count, avg_fps, avg_ms,
               avg(acc_snap, n), avg(acc_detect, n_detect), n_detect,
               avg(acc_track, n_track), n_track, avg(acc_draw, n)))

        acc_snap = acc_detect = acc_track = acc_draw = 0
        n_detect = n_track = 0

        if frame_count >= 100:
            time_list = time_list[-30:]
            fps_list = fps_list[-30:]
