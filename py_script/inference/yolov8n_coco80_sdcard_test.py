# YOLOv8n COCO80 通用目标检测 - SD Card 版本
# 模型: models_vela/yolov8n_coco80_192/  (Ethos-U55-256, vela 5.2.0)
# 拷到 SD: /sdcard/models/yolov8n_coco80_sdcard_test.tflite + .txt (80 行)
#
# 与 yolov8n_sdcard_test.py 的区别：那个是 person 单类，这个是 COCO 80 类。
import ml
import sensor
import time
from ml.postprocessing.ultralytics import YoloV8

MODEL_PATH = "/sdcard/models/yolov8n_coco80_sdcard_test.tflite"
THRESHOLD = 0.4

sensor.reset()
sensor.set_pixformat(sensor.RGB565)
sensor.set_framesize(sensor.QVGA)
sensor.skip_frames(time=1000)

model = ml.Model(MODEL_PATH, postprocess=YoloV8(threshold=THRESHOLD,
                                               nms_threshold=THRESHOLD))
print(model)
print("labels:", len(model.labels) if model.labels else 0)

clock = time.clock()
last = time.ticks_ms()
while True:
    clock.tick()
    img = sensor.snapshot()
    count = 0
    for class_id, objects in enumerate(model.predict([img])):
        for rect, score in objects:
            img.draw_rectangle(rect, color=(0, 255, 0), thickness=2)
            label = "%s %.2f" % (model.labels[class_id] if model.labels else class_id, score)
            img.draw_string((rect[0], max(0, rect[1] - 10)), label, color=(0, 255, 0))
            count += 1
    if time.ticks_diff(time.ticks_ms(), last) >= 1000:
        print("COCO80", count, "objects  %.1f fps" % clock.fps())
        last = time.ticks_ms()
