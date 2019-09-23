import RPi.GPIO as GPIO
import time

SCK = 11
DOUT = 13
count = 0
max_count = 100

def init_GPIO():
    GPIO.setmode(GPIO.BOARD)
    GPIO.setwarnings(False)
    GPIO.setup(SCK, GPIO.OUT)
    GPIO.setup(DOUT, GPIO.IN)

def isr(channel):
    global count, value
    bit = 0
    value_bit = ''
    while bit <= 15:
        GPIO.output(SCK,0)
        GPIO.output(SCK,1)
        value_bit = value_bit+str(GPIO.input(DOUT))
        bit = bit+1
    bit = 0
    while bit <= 7:
        GPIO.output(SCK,0)
        GPIO.output(SCK,1)
        bit = bit+1
    value = int(value_bit,2)
    print(value)
    count = count+1

def init_interrupt():
    GPIO.add_event_detect(DOUT, GPIO.RISING, callback = isr)
    

if __name__ == '__main__':
    print('initializing')
    time_last = time.time()
    init_GPIO()
    GPIO.output(SCK, 1)
    init_interrupt()
    print('ready')
    while count < max_count:
        pass
    GPIO.cleanup()
