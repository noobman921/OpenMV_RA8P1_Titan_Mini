import sensor, time
sensor.reset()
sensor.set_pixformat(sensor.RGB565)
sensor.set_framesize(sensor.VGA)
sensor.skip_frames(time=2000)
t = time.ticks_ms()
n = 0
while time.ticks_ms() - t < 5000:
    sensor.snapshot()
    n += 1
print("纯snapshot fps:", n / 5.0)


# import sensor
# import image
# import time

# sensor.reset()
# sensor.set_framesize(sensor.VGA)
# sensor.set_pixformat(sensor.RGB565)
# sensor.skip_frames(time=1000)
# img = sensor.snapshot()
# face_cascade = image.HaarCascade("/rom/haarcascade_frontalface.cascade", stages=25)
# t = time.ticks_ms()
# n = 0
# while time.ticks_ms() - t < 5000:
#     a = time.ticks_ms()
#     img = sensor.snapshot()
#     faces = img.find_features(face_cascade)
#     a = time.ticks_ms() - a
#     print("frontalface time: ", a)
#     n += 1
# print("frontalface fps:", n / 5.0)
