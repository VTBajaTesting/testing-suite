import RPi.GPIO as GPIO
import time

servo_pin = 15

GPIO.setmode(GPIO.BOARD)
GPIO.setup(servo_pin, GPIO.OUT)

servo = GPIO.PWM(servo_pin, 50)
servo.start(11)
time.sleep(2)
GPIO.cleanup()
