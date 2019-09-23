# Test code to measure a single rpm

import RPi.GPIO as GPIO
import time

hall_pin = 11
time_trip = 0
time_last = 0
RPM = 0
num_data = 10
data = [0.0]*num_data
time_data = [0.0]*num_data
n = 0

def init_GPIO():
    GPIO.setmode(GPIO.BOARD)
    GPIO.setwarnings(False)
    GPIO.setup(hall_pin, GPIO.IN)

def isr(channel, time_trip):
    global hall_pin, time_last, data, time_data, n
    RPM = 60/(time_trip-time_last)
    time_last = time_trip
    data[n] = RPM
    time_data[n] = time_trip
    print(RPM)
    n = n+1

def init_interrupt():
    GPIO.add_event_detect(hall_pin, GPIO.FALLING, callback = isr(time.time()), bouncetime = 10)

if __name__ == '__main__':
    init_GPIO()
    init_interrupt()
    while n < num_data:
        pass
    
GPIO.cleanup()
