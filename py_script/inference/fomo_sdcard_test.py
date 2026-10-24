# FOMO 人脸检测 - SD Card 版本
# 对比 ROMFS vs SD Card 性能
# 运行环境：OpenMV IDE

import ml
import time
import sensor
from ml.postprocessing.edgeimpulse import Fomo
import math
import gc

# ====== 配置 ======
ROMFS_MODEL = "/rom/fomo_face_detection.tflite"
# SDCARD_MODEL = "/sdcard/models/fomo_sdcard_test.tflite"
SDCARD_MODEL = "/sdcard/models/fomo_sdcard_test.tflite"
TEST_MODE = "sdcard"  # 改为 "romfs" 或 "sdcard"

# ====== 初始化 ======
sensor.reset()
sensor.set_framesize(sensor.QVGA)
sensor.set_pixformat(sensor.RGB565)
sensor.skip_frames(time=1000)

colors = [
    (255, 0, 0),
    (0, 255, 0),
    (255, 255, 0),
    (0, 0, 255),
    (255, 0, 255),
    (0, 255, 255),
    (255, 255, 255),
]

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
# fb_alloc 主栈在 VGA 下只有 ~40KB（见 docs/ML_MEMORY_ARCHITECTURE_DESIGN.md §7.5），
# 装不下 57KB 的模型，故走 GC 堆（已扩展至 SDRAM）。
model = ml.Model(model_path, postprocess=Fomo(threshold=0.4))
load_time = time.ticks_diff(time.ticks_ms(), load_start)

mem_after = gc.mem_alloc()
mem_used = (mem_after - mem_before) / 1024.0

print("加载耗时: %d ms" % load_time)
print("内存占用: %.1f KB" % mem_used)
print("输入形状: %s" % str(model.input_shape))
print("标签: %s" % str(model.labels))
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
    t_s = time.ticks_ms()
    for i, detection_list in enumerate(model.predict([img])):
        if i == 0:
            continue  # 背景类别
        if len(detection_list) == 0:
            continue  # 该类别没有检测结果

        for (x, y, w, h), score in detection_list:
            center_x = math.floor(x + (w / 2))
            center_y = math.floor(y + (h / 2))
            img.draw_circle((center_x, center_y, 12), color=colors[i], thickness=2)
            img.draw_string((center_x - 10, center_y - 20), "%.2f" % score, color=colors[i])

    t_end = time.ticks_ms()
    elapsed = time.ticks_diff(t_end, t_start)
    print("cal time:", t_end-t_s)
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

        print("[%4d 帧] 平均: %.2f fps (%.1f ms) | 范围: %.1f ~ %.1f fps" %
              (frame_count, avg_fps, avg_time, min_fps, max_fps))

        # 重置统计（避免列表过长）
        if frame_count >= 100:
            time_list = time_list[-30:]
            fps_list = fps_list[-30:]
