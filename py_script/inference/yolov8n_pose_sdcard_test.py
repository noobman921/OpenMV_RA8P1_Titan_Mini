# YOLOv8n-pose 多人姿态（17 关键点）- SD Card 版本
# 模型: models_vela/yolov8n_pose_192/  (Ethos-U55-256, vela 5.2.0)
# 拷到 SD: /sdcard/models/yolov8n_pose_sdcard_test.tflite + .txt
#
# 该模型直接输出 DFL 原始头，不是 COCO80 框，所以需要专用解码器。
# 解码契约来自 HimaxWiseEyePlus/YOLOv8_on_WE2（Arm Apache-2.0 示例）。
import math
import ml
import sensor
import time

MODEL_PATH = "/sdcard/models/yolov8n_pose_sdcard_test.tflite"
PERSON_THRESHOLD = 0.4
KEYPOINT_THRESHOLD = 0.5
NMS_THRESHOLD = 0.45

SKELETON = ((0, 1), (0, 2), (1, 3), (2, 4), (5, 6),
            (5, 7), (7, 9), (6, 8), (8, 10), (5, 11),
            (6, 12), (11, 12), (11, 13), (13, 15), (12, 14), (14, 16))


class YoloV8Pose:
    # stride, grid, bbox 输出索引, person 输出索引, keypoint anchor 偏移
    HEADS = ((8, 24, 1, 4, 0), (16, 12, 0, 6, 576), (32, 6, 5, 2, 720))

    def __init__(self, threshold=PERSON_THRESHOLD, nms_threshold=NMS_THRESHOLD,
                 max_candidates=60, max_people=10):
        self.threshold = threshold
        self.nms_threshold = nms_threshold
        self.max_candidates = max_candidates
        self.max_people = max_people

    @staticmethod
    def sigmoid(v):
        if v >= 0:
            return 1.0 / (1.0 + math.exp(-v))
        v = math.exp(v)
        return v / (1.0 + v)

    @staticmethod
    def dfl(tensor, row, offset, scale):
        # 16 个 bin 的 softmax 期望；zero point 在这里抵消
        peak = max(int(tensor[0, row, offset + k]) for k in range(16))
        total = 0.0
        weighted = 0.0
        for k in range(16):
            v = math.exp((int(tensor[0, row, offset + k]) - peak) * scale)
            total += v
            weighted += k * v
        return weighted / total

    @staticmethod
    def iou(a, b):
        inter = (max(0.0, min(a[2], b[2]) - max(a[0], b[0]))
                 * max(0.0, min(a[3], b[3]) - max(a[1], b[1])))
        union = ((a[2] - a[0]) * (a[3] - a[1])
                 + (b[2] - b[0]) * (b[3] - b[1]) - inter)
        return inter / union if union > 0 else 0.0

    def __call__(self, model, inputs, outputs):
        candidates = []
        logit_threshold = math.log(self.threshold / (1.0 - self.threshold))
        for stride, grid, box_out, score_out, base in self.HEADS:
            scale = model.output_scale[score_out]
            zero = model.output_zero_point[score_out]
            for row in range(grid * grid):
                logit = (int(outputs[score_out][0, row, 0]) - zero) * scale
                if logit >= logit_threshold:
                    cand = (self.sigmoid(logit), row, stride, grid, box_out, base + row)
                    if len(candidates) < self.max_candidates:
                        candidates.append(cand)
                        candidates.sort(reverse=True)
                    elif cand > candidates[-1]:
                        candidates[-1] = cand
                        candidates.sort(reverse=True)

        kept = []
        for score, row, stride, grid, box_out, anchor in candidates:
            x = row % grid + 0.5
            y = row // grid + 0.5
            d = [self.dfl(outputs[box_out], row, k * 16, model.output_scale[box_out])
                 for k in range(4)]
            box = ((x - d[0]) * stride, (y - d[1]) * stride,
                   (x + d[2]) * stride, (y + d[3]) * stride)
            if any(self.iou(box, it[0]) > self.nms_threshold for it in kept):
                continue
            kept.append((box, score, anchor, x, y, stride))
            if len(kept) >= self.max_people:
                break

        rx, ry, rw, rh = inputs[0].roi
        sx, sy = rw / 192.0, rh / 192.0
        scale = model.output_scale[3]
        zero = model.output_zero_point[3]

        results = []
        for box, score, anchor, ax, ay, stride in kept:
            x1, y1 = max(0.0, box[0]), max(0.0, box[1])
            x2, y2 = min(192.0, box[2]), min(192.0, box[3])
            if x2 <= x1 or y2 <= y1:
                continue
            left, top = rx + int(x1 * sx), ry + int(y1 * sy)
            right, bottom = rx + int(x2 * sx), ry + int(y2 * sy)
            if right <= left or bottom <= top:
                continue
            points = []
            for k in range(17):
                px = (int(outputs[3][0, anchor, k * 3]) - zero) * scale
                py = (int(outputs[3][0, anchor, k * 3 + 1]) - zero) * scale
                conf = self.sigmoid((int(outputs[3][0, anchor, k * 3 + 2]) - zero) * scale)
                px = (px * 2.0 + ax - 0.5) * stride
                py = (py * 2.0 + ay - 0.5) * stride
                if not (0.0 <= px < 192.0 and 0.0 <= py < 192.0):
                    conf = 0.0
                points.append((rx + int(px * sx), ry + int(py * sy), conf))
            results.append(((left, top, right - left, bottom - top), score, points))
        return results


sensor.reset()
sensor.set_pixformat(sensor.RGB565)
sensor.set_framesize(sensor.QVGA)
sensor.skip_frames(time=1000)

model = ml.Model(MODEL_PATH, postprocess=YoloV8Pose())
print(model)

clock = time.clock()
last = time.ticks_ms()
while True:
    clock.tick()
    img = sensor.snapshot()
    people = model.predict([img])
    for rect, score, points in people:
        img.draw_rectangle(rect, color=(0, 255, 0), thickness=2)
        img.draw_string((rect[0], max(0, rect[1] - 10)), "person %.2f" % score,
                        color=(0, 255, 0))
        for a, b in SKELETON:
            if points[a][2] >= KEYPOINT_THRESHOLD and points[b][2] >= KEYPOINT_THRESHOLD:
                img.draw_line((points[a][0], points[a][1],
                               points[b][0], points[b][1]),
                              color=(0, 255, 255), thickness=2)
        for x, y, conf in points:
            if conf >= KEYPOINT_THRESHOLD:
                img.draw_circle((x, y, 2), color=(255, 255, 0), fill=True)
    if time.ticks_diff(time.ticks_ms(), last) >= 1000:
        print("Pose", len(people), "people  %.1f fps" % clock.fps())
        last = time.ticks_ms()
