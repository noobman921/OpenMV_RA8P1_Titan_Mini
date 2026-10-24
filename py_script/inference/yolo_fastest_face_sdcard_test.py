# YOLO-Fastest Face v4 人脸检测（灰度）- SD Card 版本
# 模型: models_vela/yolo_fastest_face_192/  (Ethos-U55-256, vela 5.2.0)
# 拷到 SD: /sdcard/models/yolo_fastest_face_sdcard_test.tflite + .txt
#
# 输入是**单通道灰度** 192x192x1，与其它检测模型不同。
# 解码遵循 Arm CMSIS-MLEK 的 DetectorPostProcessing（Apache-2.0）。
import math
import ml
import sensor
import time

MODEL_PATH = "/sdcard/models/yolo_fastest_face_sdcard_test.tflite"
THRESHOLD = 0.5
NMS_THRESHOLD = 0.45
MAX_CANDIDATES = 60
MAX_FACES = 16


class YoloFastestFace:
    # 导出顺序：6x6 头（大人脸）在前，12x12 头（小人脸）在后
    ANCHORS = (((38, 77), (47, 97), (61, 126)),
               ((14, 26), (19, 37), (28, 55)))
    SCALES = (0.13408391177654266, 0.18535925447940826)
    ZEROS = (47, 10)

    def __init__(self, threshold=THRESHOLD, nms_threshold=NMS_THRESHOLD,
                 max_candidates=MAX_CANDIDATES, max_faces=MAX_FACES):
        self.threshold = threshold
        self.nms_threshold = nms_threshold
        self.max_candidates = max_candidates
        self.max_faces = max_faces
        logit = math.log(threshold / (1 - threshold))
        self.raw_thresholds = tuple(math.floor(logit / s + z)
                                    for s, z in zip(self.SCALES, self.ZEROS))

    @staticmethod
    def sigmoid(v):
        return 1.0 / (1.0 + math.exp(-v))

    @staticmethod
    def iou(a, b):
        inter = (max(0, min(a[2], b[2]) - max(a[0], b[0]))
                 * max(0, min(a[3], b[3]) - max(a[1], b[1])))
        union = ((a[2] - a[0]) * (a[3] - a[1])
                 + (b[2] - b[0]) * (b[3] - b[1]) - inter)
        return inter / union if union > 0 else 0

    def __call__(self, model, inputs, outputs):
        candidates = []
        for head, grid in enumerate((6, 12)):
            rows = outputs[head].reshape((grid * grid * 3, 6))
            scale, zero = self.SCALES[head], self.ZEROS[head]
            for index in range(grid * grid * 3):
                raw_obj = int(rows[index, 4])
                if raw_obj <= self.raw_thresholds[head]:
                    continue
                objectness = self.sigmoid((raw_obj - zero) * scale)
                score = objectness * self.sigmoid((int(rows[index, 5]) - zero) * scale)
                if score <= self.threshold:
                    continue
                candidates.append((score, head, index))
                if len(candidates) > self.max_candidates:
                    candidates.sort(reverse=True)
                    candidates.pop()
        candidates.sort(reverse=True)

        selected = []
        for score, head, index in candidates:
            grid = 6 if head == 0 else 12
            row = outputs[head].reshape((grid * grid * 3, 6))[index]
            scale, zero = self.SCALES[head], self.ZEROS[head]
            cell, anchor = index // 3, index % 3
            cx = (cell % grid + self.sigmoid((int(row[0]) - zero) * scale)) / grid
            cy = (cell // grid + self.sigmoid((int(row[1]) - zero) * scale)) / grid
            aw, ah = self.ANCHORS[head][anchor]
            w = aw * math.exp((int(row[2]) - zero) * scale) / 192
            h = ah * math.exp((int(row[3]) - zero) * scale) / 192
            box = (cx - w / 2, cy - h / 2, cx + w / 2, cy + h / 2)
            if any(self.iou(box, old[0]) > self.nms_threshold for old in selected):
                continue
            selected.append((box, score))
            if len(selected) >= self.max_faces:
                break

        rx, ry, rw, rh = inputs[0].roi
        result = []
        for box, score in selected:
            x1, y1 = max(0, min(1, box[0])), max(0, min(1, box[1]))
            x2, y2 = max(0, min(1, box[2])), max(0, min(1, box[3]))
            x, y = rx + int(x1 * rw), ry + int(y1 * rh)
            w, h = int((x2 - x1) * rw), int((y2 - y1) * rh)
            if w > 0 and h > 0:
                result.append(((x, y, w, h), score))
        return result


sensor.reset()
sensor.set_pixformat(sensor.GRAYSCALE)   # 该模型输入是 1 通道
sensor.set_framesize(sensor.QVGA)
sensor.skip_frames(time=1000)

model = ml.Model(MODEL_PATH, postprocess=YoloFastestFace())
print(model)

clock = time.clock()
last = time.ticks_ms()
while True:
    clock.tick()
    img = sensor.snapshot()
    faces = model.predict([img])
    for rect, score in faces:
        img.draw_rectangle(rect, color=(255, 0, 0), thickness=2)
        img.draw_string((rect[0], max(0, rect[1] - 10)), "%.2f" % score,
                        color=(255, 0, 0))
    if time.ticks_diff(time.ticks_ms(), last) >= 1000:
        print("YOLO-Fastest face", len(faces), "faces  %.1f fps" % clock.fps())
        last = time.ticks_ms()
