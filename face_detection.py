import sensor
import image
import time

sensor.reset()
sensor.set_framesize(sensor.VGA)
sensor.set_pixformat(sensor.RGB565)
sensor.skip_frames(time=1000)
img = sensor.snapshot()
face_cascade = image.HaarCascade("/rom/haarcascade_frontalface.cascade", stages=25)
# face_cascade = image.HaarCascade("frontalface", stages=25)
while True:
    img = sensor.snapshot()
    t = time.ticks_ms()
    faces = img.find_features(face_cascade)
    t = time.ticks_ms() - t
    print(len(faces))
    print("cal time: %d" % t)
