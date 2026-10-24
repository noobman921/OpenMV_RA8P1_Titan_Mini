# MoveNet 单人姿态估计（17 关键点）- SD Card 版本
# 模型: models_vela/movenet_singlepose_192/  (Ethos-U55-256, vela 5.2.0)
# 拷到 SD: /sdcard/models/movenet_sdcard_test.tflite
import ml
import sensor
import time
from ml.postprocessing.mediapipe import MoveNet

MODEL_PATH = "/sdcard/models/movenet_sdcard_test.tflite"
THRESHOLD = 0.4

# COCO 顺序的关键点连线
BODY_LINES = ((0, 1), (0, 2), (1, 3), (2, 4), (0, 5), (0, 6), (5, 6),
              (5, 7), (7, 9), (6, 8), (8, 10), (5, 11), (6, 12), (11, 12),
              (11, 13), (13, 15), (12, 14), (14, 16))

sensor.reset()
sensor.set_pixformat(sensor.RGB565)
sensor.set_framesize(sensor.QVGA)
sensor.skip_frames(time=1000)

model = ml.Model(MODEL_PATH, postprocess=MoveNet(threshold=THRESHOLD))
print(model)

clock = time.clock()
last = time.ticks_ms()
while True:
    clock.tick()
    img = sensor.snapshot()
    count = 0
    for rect, score, keypoints in model.predict([img]):
        img.draw_rectangle(rect, color=(0, 255, 0), thickness=2)
        for a, b in BODY_LINES:
            if keypoints[a][2] >= THRESHOLD and keypoints[b][2] >= THRESHOLD:
                img.draw_line((int(keypoints[a][0]), int(keypoints[a][1]),
                               int(keypoints[b][0]), int(keypoints[b][1])),
                              color=(0, 255, 255), thickness=2)
        for i in range(keypoints.shape[0]):
            if keypoints[i][2] >= THRESHOLD:
                img.draw_circle((int(keypoints[i][0]), int(keypoints[i][1]), 2),
                                color=(255, 255, 0), fill=True)
        count += 1
    if time.ticks_diff(time.ticks_ms(), last) >= 1000:
        print("MoveNet", count, "people  %.1f fps" % clock.fps())
        last = time.ticks_ms()
