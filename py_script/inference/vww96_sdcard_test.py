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
    scores = model.predict([img])[0]
    best = int(np.argmax(scores))
    name = model.labels[best] if model.labels else str(best)

    img.draw_string((5, 5), "%s %.2f" % (name, scores[best]), color=(0, 255, 0))

    if time.ticks_diff(time.ticks_ms(), last) >= 1000:
        print("%s %.2f  %.1f fps" % (name, scores[best], clock.fps()))
        last = time.ticks_ms()
