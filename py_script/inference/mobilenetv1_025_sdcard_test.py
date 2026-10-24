# MobileNetV1 0.25 ImageNet 分类 - SD Card 版本
# 模型: models_vela/mobilenetv1_025/  (Ethos-U55-256, vela 5.2.0)
# 拷到 SD: /sdcard/models/mobilenetv1_025_sdcard_test.tflite + mobilenetv1_025_vela.txt
#
# 输入是 INT8、scale=2/255、zero=-1，与 ml 默认的"先转 RGB888 再量化"相差 1 LSB。
# 若需要与训练端逐位一致，可改用 Titan 的查表法；这里用通用路径。
import ml
import sensor
import time
from ulab import numpy as np

MODEL_PATH = "/sdcard/models/mobilenetv1_025_sdcard_test.tflite"
TOP_K = 3

sensor.reset()
sensor.set_pixformat(sensor.RGB565)
sensor.set_framesize(sensor.QVGA)
sensor.skip_frames(time=1000)

model = ml.Model(MODEL_PATH)
print(model)
print("labels:", (model.labels[0:3] if model.labels else None))

clock = time.clock()
last = time.ticks_ms()
while True:
    clock.tick()
    img = sensor.snapshot()
    scores = model.predict([img])[0]

    order = np.argsort(scores)
    text = ""
    for i in range(TOP_K):
        idx = int(order[len(order) - 1 - i])
        name = model.labels[idx] if model.labels else str(idx)
        text += "%s %.2f  " % (name, scores[idx])
    img.draw_string((5, 5), text[:36], color=(0, 255, 0))

    if time.ticks_diff(time.ticks_ms(), last) >= 1000:
        print(text, "%.1f fps" % clock.fps())
        last = time.ticks_ms()
