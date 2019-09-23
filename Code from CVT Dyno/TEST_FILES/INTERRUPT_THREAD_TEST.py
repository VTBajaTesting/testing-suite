# Test code to determine if the interrupt service routine is executed in the same thread as the main loop

import RPi.GPIO as GPIO
import time

int_pin = 40
time_start = time.time()

def init_GPIO():
    GPIO.setmode(GPIO.BOARD)
    GPIO.setwarnings(False)
    GPIO.setup(int_pin, GPIO.IN)

def isr(channel):
    print('Interrupt Detected')
    time.sleep(10)

def init_interrupt():
    GPIO.add_event_detect(int_pin, GPIO.FALLING, callback = isr, bouncetime = 10)

if __name__ == '__main__':
    init_GPIO()
    init_interrupt()
    while True:
        time_cur = time.time()-time_start
        print(time_cur)
        time.sleep(1)
