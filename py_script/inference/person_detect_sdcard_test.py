# MobileNet 行人检测 (图像分类) - SD Card 版本
# 对比 ROMFS vs SD Card 性能
# 运行环境：OpenMV IDE

import ml
import time
import sensor
import gc

# ====== 配置 ======
ROMFS_MODEL = "/rom/person_detect.tflite"       # 注意：当前 romfs 分区未收录此模型
SDCARD_MODEL = "/sdcard/models/person_detect_sdcard_test.tflite"
TEST_MODE = "sdcard"  # 改为 "romfs" 或 "sdcard"

# ====== 初始化 ======
sensor.reset()
sensor.set_framesize(sensor.VGA)
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
# person_detect 是纯分类模型，不需要后处理
# 300KB 的模型走 GC 堆；装不下时 py_ml.c 会自动退到 uma 池。
# 改用 GC 堆（已扩展至 SDRAM）。
model = ml.Model(model_path)
load_time = time.ticks_diff(time.ticks_ms(), load_start)

mem_after = gc.mem_alloc()
mem_used = (mem_after - mem_before) / 1024.0

# 标签自动加载失败时手动回退（短文件名大小写问题）
if model.labels is None:
    try:
        model.labels = [line.rstrip('\n') for line in open("/sdcard/models/person_detect_sdcard_test.txt", "r")]
    except Exception:
        model.labels = ["no_person", "person"]

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

    # 分类结果：把标签和置信度配对后按置信度排序
    scores = sorted(
        zip(model.labels, model.predict([img])[0].flatten().tolist()),
        key=lambda x: x[1],
        reverse=True
    )
    top_label, top_score = scores[0]

    # 屏幕叠加显示
    img.draw_string((4, 4), "%s %.2f" % (top_label, top_score), color=(255, 0, 0), scale=2)

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

        print("[%4d 帧] 平均: %.2f fps (%.1f ms) | 范围: %.1f ~ %.1f fps | 结果: %s %.2f" %
              (frame_count, avg_fps, avg_time, min_fps, max_fps, top_label, top_score))

        # 重置统计（避免列表过长）
        if frame_count >= 100:
            time_list = time_list[-30:]
            fps_list = fps_list[-30:]
