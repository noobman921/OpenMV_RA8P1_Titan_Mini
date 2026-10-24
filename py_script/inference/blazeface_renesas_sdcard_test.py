# BlazeFace（Renesas INT8）人脸检测 - SD Card 版本
# 模型: models_vela/blazeface128/  (Ethos-U55-256, vela 5.2.0)
# 拷到 SD: /sdcard/models/blazeface_renesas_sdcard_test.tflite
#
# 与 blazeface_sdcard_test.py 用的是**不同来源**的 BlazeFace：
#   - 那个是 OpenMV 官方的 blazeface_front_128（FLOAT32 接口）
#   - 这个是 Renesas INT8 量化的 blazeface128，四个输出头顺序不同
import ml
import sensor
import time
from ml.postprocessing.mediapipe import BlazeFace

MODEL_PATH = "/sdcard/models/blazeface_renesas_sdcard_test.tflite"
THRESHOLD = 0.4

sensor.reset()
sensor.set_pixformat(sensor.RGB565)
sensor.set_framesize(sensor.QVGA)
sensor.set_windowing((240, 240))
sensor.skip_frames(time=1000)

model = ml.Model(MODEL_PATH, postprocess=BlazeFace(threshold=THRESHOLD))
print(model)

clock = time.clock()
last = time.ticks_ms()
while True:
    clock.tick()
    img = sensor.snapshot()
    count = 0
    for rect, score, keypoints in model.predict([img]):
        img.draw_rectangle(rect, color=(0, 255, 0), thickness=2)
        for i in range(keypoints.shape[0]):
            img.draw_circle((int(keypoints[i][0]), int(keypoints[i][1]), 2),
                             color=(255, 0, 0), thickness=1)
        count += 1
    if time.ticks_diff(time.ticks_ms(), last) >= 1000:
        print("BlazeFace-INT8", count, "faces  %.1f fps" % clock.fps())
        last = time.ticks_ms()
