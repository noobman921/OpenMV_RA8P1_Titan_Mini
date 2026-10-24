# YOLOv8n 目标检测 - SD Card 版本
# 对比 ROMFS vs SD Card 性能
# 运行环境：OpenMV IDE
#
# YOLOv8n 是 Ultralytics YOLOv8 系列的 nano 版本（最小最快）

import ml
import time
import sensor
import gc

# ====== 配置 ======
ROMFS_MODEL = "/rom/yolov8n_192.tflite"         # 注意：当前 romfs 分区未收录此模型
SDCARD_MODEL = "/sdcard/models/yolov8n_sdcard_test.tflite"
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
# 3152KB 远超 fb_alloc 主栈（VGA 下仅 ~40KB），必须走 GC 堆（已扩展至 SDRAM）
# YOLOv8 后处理需根据实际输出格式确定（可能需要自定义或使用通用 YOLO 后处理）
model = ml.Model(model_path)
load_time = time.ticks_diff(time.ticks_ms(), load_start)

mem_after = gc.mem_alloc()
mem_used = (mem_after - mem_before) / 1024.0

# 标签自动加载失败时手动回退（COCO 80 类子集）
if model.labels is None:
    try:
        model.labels = [line.rstrip('\n') for line in open("/sdcard/models/yolov8n_sdcard_test.txt", "r")]
    except Exception:
        # 简化版 COCO 标签（实际应包含完整 80 类）
        model.labels = [
            "person", "bicycle", "car", "motorcycle", "airplane", "bus", "train", "truck", "boat",
            "traffic light", "fire hydrant", "stop sign", "parking meter", "bench", "bird", "cat",
            "dog", "horse", "sheep", "cow", "elephant", "bear", "zebra", "giraffe", "backpack",
            "umbrella", "handbag", "tie", "suitcase", "frisbee", "skis", "snowboard", "sports ball",
            "kite", "baseball bat", "baseball glove", "skateboard", "surfboard", "tennis racket",
            "bottle", "wine glass", "cup", "fork", "knife", "spoon", "bowl", "banana", "apple",
            "sandwich", "orange", "broccoli", "carrot", "hot dog", "pizza", "donut", "cake",
            "chair", "couch", "potted plant", "bed", "dining table", "toilet", "tv", "laptop",
            "mouse", "remote", "keyboard", "cell phone", "microwave", "oven", "toaster", "sink",
            "refrigerator", "book", "clock", "vase", "scissors", "teddy bear", "hair drier", "toothbrush"
        ]

print("加载耗时: %d ms" % load_time)
print("内存占用: %.1f KB" % mem_used)
print("输入形状: %s" % str(model.input_shape))
print("输出形状: %s" % str(model.output_shape))
print("标签数量: %d" % len(model.labels))
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
print("注意：YOLOv8 需要自定义后处理解析输出张量")
print("当前脚本仅测试加载和推理性能，实际部署需实现 NMS 后处理")
print()

while True:
    t_start = time.ticks_ms()
    img = sensor.snapshot()

    detect_count = 0
    try:
        # YOLOv8 原始输出需要后处理（NMS、坐标解码等）
        # 此处仅作为性能测试框架，实际部署需实现完整后处理
        outputs = model.predict([img])

        # 提示：实际后处理应包括：
        # 1. 解析输出张量（通常是 (1, 84, 8400) 格式：4 box + 80 classes）
        # 2. 置信度过滤
        # 3. NMS（非极大值抑制）
        # 4. 坐标转换到原图尺寸

        # TODO: 实现后处理后，这里应返回检测框列表
        # 当前仅显示提示信息
        img.draw_string((4, 4), "需要后处理", color=(255, 255, 0), scale=2)

    except Exception as e:
        img.draw_string((4, 4), "推理错误: %s" % str(e)[:20], color=(255, 0, 0), scale=1)

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
