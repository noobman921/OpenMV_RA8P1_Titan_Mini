# ResNet8 CIFAR10 分类 - SD Card 版本
# 模型: models_vela/resnet8/  (Ethos-U55-256, vela 5.2.0)
# 拷到 SD: /sdcard/models/resnet8_sdcard_test.tflite + resnet8_vela.txt
import ml
import sensor
import time
from ulab import numpy as np

MODEL_PATH = "/sdcard/models/resnet8_sdcard_test.tflite"

sensor.reset()
sensor.set_pixformat(sensor.RGB565)
sensor.set_framesize(sensor.QVGA)
sensor.skip_frames(time=1000)

model = ml.Model(MODEL_PATH)
print(model)
print("labels:", model.labels)

clock = time.clock()
last = time.ticks_ms()
while True:
    clock.tick()
    img = sensor.snapshot()
    scores = model.predict([img])[0]
    # best = int(np.argmax(scores))
    # 1. 确保 scores 是一维的，并转换成普通 Python 列表
    if hasattr(scores, "flatten"):
        scores = scores.flatten()
    scores_list = list(scores)  # 强制转成普通列表，避免 ulab 数组的索引陷阱

    # 2. 手动找到最大值的索引
    best = 0
    for i in range(1, len(scores_list)):
        if scores_list[i] > scores_list[best]:
            best = i

    # 3. 安全检查：确保索引在 labels 范围内
    if model.labels and best < len(model.labels):
        name = model.labels[best]
    else:
        name = str(best)  # 如果越界，直接显示数字索引，方便调试
    name = model.labels[best] if model.labels else str(best)

    img.draw_string((5, 5), "%s %.2f" % (name, scores[best]), color=(0, 255, 0))

    if time.ticks_diff(time.ticks_ms(), last) >= 1000:
        print("%s %.2f  %.1f fps" % (name, scores[best], clock.fps()))
        last = time.ticks_ms()
