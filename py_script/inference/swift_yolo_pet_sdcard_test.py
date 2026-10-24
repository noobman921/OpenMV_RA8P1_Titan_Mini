# Swift-YOLO 猫狗检测- SD Card 版本
# 模型: models_vela/swift_yolo_pet_192/  (Ethos-U55-256, vela 5.2.0)
# 拷到 SD: /sdcard/models/swift_yolo_pet_sdcard_test.tflite + .txt
#
# Seeed 导出的图已经解码好 anchor 和 sigmoid：xywh 是模型像素，
# 第 4 列是百分制分数（0~100），不是 0~1。
import ml
import sensor
import time
from ml.utils import NMS
from ulab import numpy as np

MODEL_PATH = "/sdcard/models/swift_yolo_pet_sdcard_test.tflite"
THRESHOLD = 0.4


def postprocess(model, inputs, outputs):
    scale = model.output_scale[0]
    zero = model.output_zero_point[0]
    rows = outputs[0][0]
    indices = np.nonzero(rows[:, 4] > THRESHOLD * 100 / scale + zero)[0]
    if not len(indices):
        return ()
    boxes = (np.take(rows, indices, axis=0) - float(zero)) * scale
    classes = np.argmax(boxes[:, 5:], axis=1)
    nms = NMS(192, 192, inputs[0].roi)
    for i in range(boxes.shape[0]):
        cx, cy = boxes[i, 0], boxes[i, 1]
        w, h = boxes[i, 2], boxes[i, 3]
        score = min(1.0, boxes[i, 4] / 100.0)
        nms.add_bounding_box(cx - w / 2, cy - h / 2, cx + w / 2, cy + h / 2,
                             score, int(classes[i]))
    return nms.get_bounding_boxes(threshold=THRESHOLD, sigma=0.1)


sensor.reset()
sensor.set_pixformat(sensor.RGB565)
sensor.set_framesize(sensor.QVGA)
sensor.skip_frames(time=1000)

model = ml.Model(MODEL_PATH, postprocess=postprocess)
print(model)
print("labels:", model.labels)

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
        print("Swift-YOLO pet", count, "objects  %.1f fps" % clock.fps())
        last = time.ticks_ms()
