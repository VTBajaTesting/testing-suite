import RPi.GPIO as GPIO
import time

hall_P_pin = 7

GPIO.setmode(GPIO.BOARD)
GPIO.setup(hall_P_pin, GPIO.IN)
try:
    while True:
        print(GPIO.input(hall_P_pin))
        time.sleep(0.5)
finally:
    GPIO.cleanup()
    print('CLEAN')
