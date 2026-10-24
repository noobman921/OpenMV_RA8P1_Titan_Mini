# U-Net / MobileNetV2 宠物分割（128x128）- SD Card 版本
# 模型: models_vela/unet_mobilenetv2_128/  (Ethos-U55-256, vela 5.2.0)
# 拷到 SD: /sdcard/models/unet_pet_sdcard_test.tflite + .txt
#
# ⚠️ 该模型 5.97 MB，超过本工程 GC 的 SDRAM 区容量（0x681C2000..0x68600000
#    = 4.25 MB），ml.Model() 会因分配失败报 "Failed to allocate tensors"。
#    要用它需要先改 omvmain.c 的 OMV_GC_SDRAM_END 或改用独立静态区。
#    其余 12 个模型都在容量内。
import image
import ml
import sensor
import time
from ulab import numpy as np

MODEL_PATH = "/sdcard/models/unet_pet_sdcard_test.tflite"
MASK_W = 128
MASK_H = 128


def pet_mask_callback(model, inputs, outputs):
    # 三个通道共用同一 scale/zero，所以对 UINT8 取 argmax 与反量化后等价。
    # 在回调里直接读 NPU 缓冲，避免 192 KiB 的浮点拷贝。
    return np.array(np.argmax(outputs[0][0], axis=2), dtype=np.uint8)


sensor.reset()
sensor.set_pixformat(sensor.RGB565)
sensor.set_framesize(sensor.QVGA)
sensor.skip_frames(time=1000)

model = ml.Model(MODEL_PATH, postprocess=pet_mask_callback)
print(model)

# 调色板：0=宠物绿，1=背景透明，2=边界黄
colors = image.Image(256, 1, image.RGB565)
alpha = image.Image(256, 1, image.GRAYSCALE)
colors.set_pixel((0, 0), (0, 255, 0))
colors.set_pixel((2, 0), (255, 255, 0))
alpha.set_pixel((0, 0), 112)
alpha.set_pixel((2, 0), 192)

clock = time.clock()
last = time.ticks_ms()
while True:
    clock.tick()
    img = sensor.snapshot()
    classes = model.predict([img])
    mask = image.Image(MASK_W, MASK_H, image.GRAYSCALE, buffer=classes)
    img.draw_image(mask, 0, 0, x_scale=img.width() / MASK_W,
                   y_scale=img.height() / MASK_H,
                   color_palette=colors, alpha_palette=alpha)
    if time.ticks_diff(time.ticks_ms(), last) >= 1000:
        print("U-Net pet segmentation  %.1f fps" % clock.fps())
        last = time.ticks_ms()
