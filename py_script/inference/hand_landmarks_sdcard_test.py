# Hand Landmarks 手部关键点检测 - SD Card 版本
# 对比 ROMFS vs SD Card 性能
# 运行环境：OpenMV IDE

import ml
import time
import sensor
from ml.postprocessing.mediapipe import HandLandmarks
import gc

# ====== 配置 ======
ROMFS_MODEL = "/rom/hand_landmarks_full_224.tflite"  # 注意：当前 romfs 分区未收录此模型
SDCARD_MODEL = "/sdcard/models/hand_landmarks_sdcard_test.tflite"         # FAT32 短文件名
TEST_MODE = "sdcard"  # 改为 "romfs" 或 "sdcard"

# ====== 初始化 ======
sensor.reset()
sensor.set_framesize(sensor.QVGA)
sensor.set_pixformat(sensor.RGB565)
sensor.skip_frames(time=1000)

# ====== 加载模型 ======
print("=" * 50)
if TEST_MODE == "romfs":
    print("测试模式: ROMFS (零拷贝)")
    model_path = ROMFS_MODEL
else:
    print("测试模式: SD Card (读入 RAM)")
    model_path = SDCARD_MODEL

print("模型路径: %s" % model_path)
print("=" * 50)

gc.collect()
mem_before = gc.mem_alloc()

load_start = time.ticks_ms()
# 3199KB 远超 fb_alloc 主栈（VGA 下仅 ~40KB），必须走 GC 堆（已扩展至 SDRAM）
model = ml.Model(model_path, postprocess=HandLandmarks())
load_time = time.ticks_diff(time.ticks_ms(), load_start)

mem_after = gc.mem_alloc()
mem_used = (mem_after - mem_before) / 1024.0

print("加载耗时: %d ms" % load_time)
print("内存占用: %.1f KB" % mem_used)
print("输入形状: %s" % str(model.input_shape))
print("=" * 50)
print()

# ====== 性能统计 ======
frame_count = 0
time_list = []
fps_list = []

# ====== 主循环 ======
while True:
    t_start = time.ticks_ms()
    img = sensor.snapshot()

    # landmarks: list of (21, 3) keypoints - (x, y, z)
    # 21 keypoints: wrist + 4 fingers × 5 joints each
    landmarks = model.predict([img])

    detect_count = len(landmarks)

    # 绘制所有关键点
    for keypoint in landmarks:
        for i in range(len(keypoint)):
            x = int(keypoint[i][0])
            y = int(keypoint[i][1])
            img.draw_circle((x, y, 2), color=(0, 255, 0), thickness=1)

        # 绘制连接线（手部骨架）
        connections = [
            (0, 1), (1, 2), (2, 3), (3, 4),      # 拇指
            (0, 5), (5, 6), (6, 7), (7, 8),      # 食指
            (0, 9), (9, 10), (10, 11), (11, 12), # 中指
            (0, 13), (13, 14), (14, 15), (15, 16), # 无名指
            (0, 17), (17, 18), (18, 19), (19, 20), # 小指
            (5, 9), (9, 13), (13, 17)            # 手掌
        ]
        for start, end in connections:
            x1, y1 = int(keypoint[start][0]), int(keypoint[start][1])
            x2, y2 = int(keypoint[end][0]), int(keypoint[end][1])
            img.draw_line((x1, y1, x2, y2), color=(255, 255, 0), thickness=1)

    # 屏幕叠加显示检测数量
    if detect_count > 0:
        img.draw_string((4, 4), "Hands: %d" % detect_count, color=(0, 255, 0), scale=2)
    else:
        img.draw_string((4, 4), "No hand", color=(255, 0, 0), scale=2)

    t_end = time.ticks_ms()
    elapsed = time.ticks_diff(t_end, t_start)
    time_list.append(elapsed)

    if elapsed > 0:
        fps = 1000.0 / elapsed
        fps_list.append(fps)

    frame_count += 1

    # 每 30 帧显示统计
    if frame_count % 30 == 0:
        avg_time = sum(time_list) / len(time_list)
        avg_fps = sum(fps_list) / len(fps_list)
        min_time = min(time_list)
        max_time = max(time_list)
        max_fps = 1000.0 / min_time if min_time > 0 else 0
        min_fps = 1000.0 / max_time if max_time > 0 else 0

        print("[%4d 帧] 平均: %.2f fps (%.1f ms) | 范围: %.1f ~ %.1f fps | 检测: %d" %
              (frame_count, avg_fps, avg_time, min_fps, max_fps, detect_count))

        # 重置统计（避免列表过长）
        if frame_count >= 100:
            time_list = time_list[-30:]
            fps_list = fps_list[-30:]
