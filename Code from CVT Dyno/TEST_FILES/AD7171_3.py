import RPi.GPIO as GPIO
import time

SCK = 11
DOUT = 13
max_count = 5

def init_GPIO():
    GPIO.setmode(GPIO.BOARD)
    GPIO.setwarnings(False)
    GPIO.setup(SCK, GPIO.OUT)
    GPIO.setup(DOUT, GPIO.IN, pull_up_down = GPIO.PUD_UP)

if __name__ == '__main__':
    init_GPIO()
    count = 0
    GPIO.output(SCK, 1)
    while count < max_count:
        while GPIO.input(DOUT):
            print('detected')
            time.sleep(0.002)
            count = count+1
