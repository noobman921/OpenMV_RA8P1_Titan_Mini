# BlazeFace 人脸检测 - SD Card 版本
# 对比 ROMFS vs SD Card 性能
# 运行环境：OpenMV IDE

import ml
import time
import sensor
from ml.postprocessing.mediapipe import BlazeFace
import gc
import board

# ====== 配置 ======
ROMFS_MODEL = "/rom/blazeface_front_128.tflite"
# SDCARD_MODEL = "/sdcard/models/blazeface_sdcard_test.tflite"
SDCARD_MODEL = "/sdcard/models/blazeface_sdcard_test.tflite"
TEST_MODE = "sdcard"  # 改为 "romfs" 或 "sdcard"

# ====== 初始化 ======
sensor.reset()
sensor.set_framesize(sensor. QVGA)
sensor.set_pixformat(sensor.RGB565)
sensor.skip_frames(time=1000)
board.preview(True)

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
# 190KB 的模型走 GC 堆（已扩展至 SDRAM）；装不下时 py_ml.c 会自动退到 uma 池。
model = ml.Model(model_path, postprocess=BlazeFace(threshold=0.4))
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
    t0 = time.ticks_ms()
    img = sensor.snapshot()
    t1 = time.ticks_ms()
    t2 = time.ticks_ms()
    # faces is a list of ((x, y, w, h), score, keypoints) tuples
    for r, score, keypoints in model.predict([img]):
        # 画检测框
        img.draw_rectangle(r, color=(0, 0, 255), thickness=2)
        img.draw_string((r[0], r[1] - 10), "%.2f" % score, color=(0, 0, 255))

        # 画关键点
        # keypoints: (6, 2) - right_eye, left_eye, nose, mouth, right_ear, left_ear
        for i in range(6):
            x = int(keypoints[i][0])
            y = int(keypoints[i][1])
            img.draw_circle((x, y, 3), color=(255, 0, 0), thickness=2)
    t3 = time.ticks_ms()
    t4 = time.ticks_ms()
    t_end = time.ticks_ms()
    elapsed = time.ticks_diff(t_end, t_start)
    time_list.append(elapsed)
    # print("snap=%d prep=%d pred=%d draw=%d" % (t1-t0, t2-t1, t3-t2, t4-t3))
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
