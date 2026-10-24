# YOLOv5n6-xiao VOC20 目标检测 - SD Card 版本
# 模型: models_vela/yolov5n6_xiao_voc20_192/  (Titan 预编译版，Vela 5.1.0)
# 拷到 SD: /sdcard/models/yolov5n6_xiao_voc20_sdcard_test.tflite + .txt
#
# 说明：该模型的原始 tflite 未公开下载地址，这里用的是 Titan 仓库里已编译好的
# Ethos-U55-256 版本。加速器配置与我们一致，可直接跑。
import ml
import sensor
import time
from ml.utils import NMS
from ulab import numpy as np

MODEL_PATH = "/sdcard/models/yolov5n6_xiao_voc20_sdcard_test.tflite"
THRESHOLD = 0.3
MAX_CANDIDATES = 60


class XiaoDetector:
    def __init__(self, threshold=THRESHOLD):
        self.threshold = threshold

    def __call__(self, model, inputs, outputs):
        rows = outputs[0].reshape((model.output_shape[0][1], 25))
        scale = model.output_scale[0]
        zero = float(model.output_zero_point[0])
        # YOLOv5 是 objectness * class prob，不是只看 objectness
        class_max = np.max(rows[:, 5:], axis=1)
        scores = ((rows[:, 4] - zero) * scale) * ((class_max - zero) * scale)
        indices = np.nonzero(scores > self.threshold)[0]

        candidates = []
        for index in indices:
            index = int(index)
            score = min(1.0, float(scores[index]))
            if len(candidates) < MAX_CANDIDATES:
                candidates.append((score, index))
                candidates.sort()
            elif score > candidates[0][0]:
                candidates[0] = (score, index)
                candidates.sort()
        if not candidates:
            return ()

        selected = np.take(rows, [it[1] for it in candidates], axis=0)
        classes = np.argmax(selected[:, 5:], axis=1)
        per_class = {}
        for i, (score, _) in enumerate(candidates):
            row = selected[i]
            cx = (float(row[0]) - zero) * scale * 192
            cy = (float(row[1]) - zero) * scale * 192
            w = (float(row[2]) - zero) * scale * 192
            h = (float(row[3]) - zero) * scale * 192
            if w > 0 and h > 0:
                class_id = int(classes[i])
                if class_id not in per_class:
                    per_class[class_id] = NMS(192, 192, inputs[0].roi)
                per_class[class_id].add_bounding_box(cx - w / 2, cy - h / 2,
                                                     cx + w / 2, cy + h / 2,
                                                     score, class_id)
        detections = [[] for _ in range(20)]
        for class_id, nms in per_class.items():
            kept = nms.get_bounding_boxes(threshold=self.threshold, sigma=0.1)
            if len(kept) > class_id:
                detections[class_id] = kept[class_id]
        return detections


sensor.reset()
sensor.set_pixformat(sensor.RGB565)
sensor.set_framesize(sensor.QVGA)
sensor.skip_frames(time=1000)

model = ml.Model(MODEL_PATH, postprocess=XiaoDetector())
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
        print("YOLOv5n6-xiao VOC20", count, "objects  %.1f fps" % clock.fps())
        last = time.ticks_ms()
