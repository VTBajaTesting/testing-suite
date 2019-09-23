import RPi.GPIO as GPIO
import time

GPIO.setmode(GPIO.BOARD)
GPIO.setup(33, GPIO.OUT)
GPIO.setup(16, GPIO.OUT)

try:
    GPIO.output(16, 1)
    time_end = time.time()+20
    while time.time() < time_end:
        GPIO.output(33, 0)
        GPIO.output(33, 1)
        time.sleep(0.05)
        GPIO.output(33, 0)
        time.sleep(0.75)
        
    GPIO.output(16, 0)
    time_end = time.time()+10
    GPIO.output(33, 1)
    while time.time() < time_end:
        pass

    GPIO.output(33, 0)
finally:
    GPIO.cleanup()
