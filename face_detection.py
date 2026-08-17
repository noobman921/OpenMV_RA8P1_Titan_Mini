# Face Detection Example (Haar Cascade - Embedded)
# ================================================
# Uses the frontalface cascade data embedded in firmware.
# No filesystem needed! Just compile and run.
#
# Usage:
#   image.HaarCascade("frontalface")       <- loads from embedded C array
#   image.HaarCascade("frontalface", stages=25)  <- limit stages for speed

import sensor
import image

# Reset sensor
sensor.reset()

# RGB565 works - find_features internally converts to grayscale
sensor.set_pixformat(sensor.RGB565)

# VGA = 640x480
sensor.set_framesize(sensor.VGA)

# Skip a few frames to let auto-exposure stabilize
sensor.skip_frames(time=2000)

# Load the embedded frontal face cascade
# "frontalface" is compiled into firmware, no file needed
# stages=25 uses all 25 stages (full accuracy, slower)
# stages=15 uses fewer stages (faster, more false positives)
face_cascade = image.HaarCascade("frontalface", stages=25)

while True:

    # Capture image
    img = sensor.snapshot()

    # Detect faces
    # threshold: detection confidence (0.0-1.0), lower = more sensitive
    # scale_factor: image pyramid scaling (1.1-2.0), lower = more scales = slower
    faces = img.find_features(face_cascade, threshold=0.5, scale_factor=1.5)

    # Draw rectangles around detected faces (red in RGB565)
    for r in faces:
        img.draw_rectangle(r, color=(255, 0, 0))

    # Print FPS and face count
    print("Faces: %d" % (len(faces)))
