import RPi.GPIO as GPIO
import time

hall_S_pin = 11

GPIO.setmode(GPIO.BOARD)
GPIO.setup(hall_S_pin, GPIO.IN)
try:
    while True:
        print(GPIO.input(hall_S_pin))
        time.sleep(0.5)
finally:
    GPIO.cleanup()
    print('CLEAN')
