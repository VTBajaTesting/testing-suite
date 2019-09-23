# Test code, syntax for GPIO pins as outputs

import RPi.GPIO as GPIO
import time

LED_PIN = 40
freq = 1.0
duty = 50
time_on = (1/freq)*duty/100
time_off = (1/freq)*(100-duty)/100

def init_GPIO():
    GPIO.setmode(GPIO.BOARD)
    GPIO.setwarnings(False)
    GPIO.setup(LED_PIN,GPIO.OUT)

def LED_ON():
    GPIO.output(LED_PIN, GPIO.HIGH)
    time.sleep(time_on)

def LED_OFF():
    GPIO.output(LED_PIN, GPIO.LOW)
    time.sleep(time_off)

if __name__ == '__main__':
    init_GPIO()
    while True:
        LED_ON()
        LED_OFF()
