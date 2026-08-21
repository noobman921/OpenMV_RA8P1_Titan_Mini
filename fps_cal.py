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
