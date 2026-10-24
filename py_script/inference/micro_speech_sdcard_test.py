# Micro Speech 关键词识别 - SD Card 版本
# 对比 ROMFS vs SD Card 性能
# 运行环境：OpenMV IDE

import ml
import time
import sensor
import audio
import gc

# ====== 配置 ======
ROMFS_MODEL = "/rom/micro_speech.tflite"
SDCARD_MODEL = "/sdcard/models/micro_speech_sdcard_test.tflite"
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
# 18KB 远小于 fb_alloc overlay(128KB)，但为统一测试走 GC 堆
model = ml.Model(model_path)
load_time = time.ticks_diff(time.ticks_ms(), load_start)

mem_after = gc.mem_alloc()
mem_used = (mem_after - mem_before) / 1024.0

# 标签自动加载失败时手动回退
if model.labels is None:
    try:
        model.labels = [line.rstrip('\n') for line in open("/sdcard/models/micro_speech_sdcard_test.txt", "r")]
    except Exception:
        model.labels = ["silence", "unknown", "yes", "no"]

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
print("注意：此模型需要音频输入，当前脚本仅测试加载和推理性能")
print("实际使用需要配合 audio 模块采集音频特征")
print()

while True:
    t_start = time.ticks_ms()
    img = sensor.snapshot()

    # 注意：micro_speech 实际需要音频 MFCC 特征作为输入
    # 这里仅作为性能测试框架，实际部署需替换为音频输入
    # 分类结果：把标签和置信度配对后按置信度排序
    try:
        scores = sorted(
            zip(model.labels, model.predict([img])[0].flatten().tolist()),
            key=lambda x: x[1],
            reverse=True
        )
        top_label, top_score = scores[0]

        # 屏幕叠加显示
        img.draw_string((4, 4), "%s %.2f" % (top_label, top_score), color=(255, 0, 0), scale=2)
    except Exception as e:
        img.draw_string((4, 4), "需要音频输入", color=(255, 0, 0), scale=2)
        top_label = "N/A"
        top_score = 0.0

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
