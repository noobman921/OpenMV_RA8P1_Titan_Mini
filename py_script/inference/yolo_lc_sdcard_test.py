# YOLO-LC 行人检测 - SD Card 版本
# 对比 ROMFS vs SD Card 性能
# 运行环境：OpenMV IDE
#
# YOLO LC 是 YOLOV2 的变体，官方定位为"无需 NPU 也能在 OpenMV 上跑得动"。

import ml
import time
import sensor
from ml.postprocessing.darknet import YoloLC
import gc
import board
# ====== 配置 ======
ROMFS_MODEL = "/rom/yolo_lc_192.tflite"        # 注意：当前 romfs 分区未收录此模型
SDCARD_MODEL = "/sdcard/models/yolo_lc_sdcard_test.tflite"   # FAT32 短文件名
TEST_MODE = "sdcard"  # 改为 "romfs" 或 "sdcard"

# ====== 初始化 ======
sensor.reset()
sensor.set_framesize(sensor.VGA)
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
# 339KB 的模型走 GC 堆；装不下时 py_ml.c 会自动退到 uma 池。
# 改用 GC 堆（已扩展至 SDRAM）。
model = ml.Model(model_path, postprocess=YoloLC(threshold=0.4))
load_time = time.ticks_diff(time.ticks_ms(), load_start)

mem_after = gc.mem_alloc()
mem_used = (mem_after - mem_before) / 1024.0

# 标签自动加载失败时手动回退（短文件名大小写问题）
if model.labels is None:
    try:
        model.labels = [line.rstrip('\n') for line in open("/sdcard/models/yolo_lc_sdcard_test.txt", "r")]
    except Exception:
        model.labels = ["person"]

print("加载耗时: %d ms" % load_time)
print("内存占用: %.1f KB" % mem_used)
print("输入形状: %s" % str(model.input_shape))
print("标签: %s" % str(model.labels))
print("=" * 50)
print()

# 每类一个颜色
n = len(model.labels)
model_class_colors = [(int(255 * i // n), int(255 * (n - i - 1) // n), 255) for i in range(n)]

# ====== 性能统计 ======
frame_count = 0
time_list = []
fps_list = []
det_count_list = []

# ====== 主循环 ======
# 每帧打印分段耗时。
#
# 为什么加这个：LC + 预览开着会掉连接，而 blaze + 预览、LC + 无预览都不掉。
# 嫌疑是「predict 期间 VM 不让出 → 协议任务停摆 → IDE 等不到 CHANNEL_READ
# 的响应而放弃会话」。要验证就得知道 predict 到底占多久。
#
# 参照：blaze 的 pred ≈ 20~27 ms，且【每帧都打印这行】也不掉 —— 所以打印本身
# 不引入问题，加在这里是安全的。
#
# 对照跑法（4 组，只看边界）：
#   LC + 预览开  → 预期掉；pred 值记下来
#   LC + 预览关  → 预期不掉；pred 值应与上面【基本相同】
#   若两者 pred 一样而只有一个掉，则「predict 时长」是触发量，机制成立。
while True:
    t_start = time.ticks_ms()

    t0 = time.ticks_ms()
    img = sensor.snapshot()
    t1 = time.ticks_ms()

    # boxes: 每类一个 ((x, y, w, h), score) 列表
    boxes = model.predict([img])
    t2 = time.ticks_ms()

    detect_count = 0
    for i, class_detections in enumerate(boxes):
        if len(class_detections) == 0:
            continue

        rects = [r for r, score in class_detections]
        labels = [model.labels[i] for j in range(len(rects))]
        colors = [model_class_colors[i] for j in range(len(rects))]
        ml.utils.draw_predictions(img, rects, labels, colors, format=None)

        detect_count += len(rects)

    t3 = time.ticks_ms()

    # print("snap=%d pred=%d draw=%d" % (t1 - t0, t2 - t1, t3 - t2))

    t_end = time.ticks_ms()
    elapsed = time.ticks_diff(t_end, t_start)
    time_list.append(elapsed)
    det_count_list.append(detect_count)

    if elapsed > 0:
        fps = 1000.0 / elapsed
        fps_list.append(fps)

    frame_count += 1

    # 每 30 帧显示统计
    if frame_count % 30 == 0:
        avg_time = sum(time_list) / len(time_list)
        avg_fps = sum(fps_list) / len(fps_list)
        avg_det = sum(det_count_list) / len(det_count_list)
        min_time = min(time_list)
        max_time = max(time_list)
        max_fps = 1000.0 / min_time if min_time > 0 else 0
        min_fps = 1000.0 / max_time if max_time > 0 else 0

        print("[%4d 帧] 平均: %.2f fps (%.1f ms) | 范围: %.1f ~ %.1f fps | 平均检测: %.1f" %
              (frame_count, avg_fps, avg_time, min_fps, max_fps, avg_det))

        # 重置统计（避免列表过长）
        if frame_count >= 100:
            time_list = time_list[-30:]
            fps_list = fps_list[-30:]
            det_count_list = det_count_list[-30:]
