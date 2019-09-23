# test code to use the interrupt function to turn on and LED

import RPi.GPIO as GPIO

import time

switch_pin = 40
led_pin = 38


def init_GPIO():
    GPIO.setmode(GPIO.BOARD)
    GPIO.setwarnings(False)
    GPIO.setup(led_pin, GPIO.OUT)
    GPIO.setup(switch_pin, GPIO.IN)

def isr(channel):
    global switch_pin, led_pin
    GPIO.output(led_pin,1)
    while GPIO.input(switch_pin):
        time.sleep(.001)
    GPIO.output(led_pin,0)

def init_interrupt():
    GPIO.add_event_detect(switch_pin, GPIO.FALLING, callback = isr, bouncetime = 10)

if __name__ == '__main__':
    init_GPIO()
    init_interrupt()
    while True:
        time.sleep(1)
    
