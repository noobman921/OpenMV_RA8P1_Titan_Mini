# VWW96 人体存在分类 - SD Card 版本
# 模型: models_vela/vww96/  (Ethos-U55-256, vela 5.2.0)
# 拷到 SD: /sdcard/models/vww96_sdcard_test.tflite + vww96_vela.txt
import ml
import sensor
import time
from ulab import numpy as np

MODEL_PATH = "/sdcard/models/vww96_sdcard_test.tflite"

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
    # scores = model.predict([img])[0]
    # best = int(np.argmax(scores))
    # name = model.labels[best] if model.labels else str(best)

    # img.draw_string((5, 5), "%s %.2f" % (name, scores[best]), color=(0, 255, 0))

    # if time.ticks_diff(time.ticks_ms(), last) >= 1000:
    #     print("%s %.2f  %.1f fps" % (name, scores[best], clock.fps()))
    #     last = time.ticks_ms()

    scores = model.predict([img])[0]

    # 1. 确保是一维，并转成普通 Python 列表，彻底避开 ulab ndarray 的格式化陷阱
    if hasattr(scores, "flatten"):
        scores = scores.flatten()
    scores_list = list(scores)

    # 2. 纯 Python 找最大值索引，不依赖 ulab 的 argmax
    best = 0
    for i in range(1, len(scores_list)):
        if scores_list[i] > scores_list[best]:
            best = i

    # 3. 取值显式转 float，并做标签越界保护
    val = float(scores_list[best])
    if model.labels and best < len(model.labels):
        name = model.labels[best]
    else:
        name = str(best)

    img.draw_string((5, 5), "%s %.2f" % (name, val), color=(0, 255, 0))

    if time.ticks_diff(time.ticks_ms(), last) >= 1000:
        print("%s %.2f  %.1f fps" % (name, val, clock.fps()))
        last = time.ticks_ms()
