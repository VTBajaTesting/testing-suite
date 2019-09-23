import RPi.GPIO as GPIO
import time

DPDT_pin = 18
MOS_pin = 22

GPIO.setmode(GPIO.BOARD)
GPIO.setup(DPDT_pin, GPIO.OUT)
GPIO.setup(MOS_pin, GPIO.OUT)

GPIO.output(DPDT_pin, 1)
time.sleep(0.025)

GPIO.output(MOS_pin, 1)
time.sleep(2)

GPIO.output(MOS_pin, 0)
GPIO.output(DPDT_pin, 0)

GPIO.cleanup()
