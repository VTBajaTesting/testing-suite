import RPi.GPIO as GPIO
import time

SCK = 38
DOUT = 40
raw_str = ''
num_data = 10
index = 0
data = [None]*num_data
errorlist = [None]*num_data
time_last = time.time()
delta_t = [None]*num_data


def init_GPIO():
    GPIO.setmode(GPIO.BOARD)
    GPIO.setwarnings(False)
    GPIO.setup(SCK, GPIO.OUT)
    GPIO.setup(DOUT, GPIO.IN)
    GPIO.output(SCK, 0)

def isr(channel):
    global data, errorlist, index, SCK, DOUT, time_last
    delta_t[index] = time.time()-time_last
    time_last = time.time()
    print(time_last)
#    time.sleep(50/1000000)
#    bit_num = 0
#    raw_str = ''
#    while bit_num <= 15:
#        GPIO.output(SCK, 1)
#        GPIO.output(SCK, 0)
#        raw_str = raw_str+str(GPIO.input(DOUT))
#        bit_num = bit_num+1
#    bit_num = 0
#    while bit_num <= 7:
#        GPIO.output(SCK, 1)
#        GPIO.output(SCK, 0)
#        if bit_num == 2:
#            error = GPIO.input(DOUT)
#        bit_num = bit_num+1
#    data[index] = int(raw_str,2)
#    errorlist[index] = error
    index = index+1

def init_interrupt():
    GPIO.add_event_detect(DOUT, GPIO.FALLING, callback = isr, bouncetime = 4)

if __name__ == '__main__':
    init_GPIO()
    init_interrupt()
    print('Initialized')
    while index <= num_data-1:
        pass
    for x in range(len(delta_t)):
#        str_print = str(data[x])+'     '+str(errorlist[x])
        print(delta_t[x])
