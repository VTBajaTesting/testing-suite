import pigpio
import time

SCK = 18
DOUT = 27
count = 0
max_count = 10
tick_last = 0

pi = pigpio.pi()

def init_GPIO():
    global pi, cbTOR
    pi.set_mode(SCK, pigpio.OUTPUT)
    pi.set_mode(DOUT, pigpio.INPUT)
    pi.write(SCK, 1)
    time.sleep(1)
    cbTOR = pi.callback(DOUT, pigpio.RISING_EDGE, isr)

def isr(channel, value_int, tick):
    global count, cbTOR
    time_start = time.time()
    cbTOR.cancel()
    bit = 0
    #value_bit = ''
    while bit <= 15:
       pi.write(SCK, 0)
       pi.write(SCK, 1)
       #value_bit = value_bit+str(pi.read(DOUT))
       bit = bit+1
    bit = 0
    #error = 0
    while bit <= 7:
        pi.write(SCK, 0)
        pi.write(SCK, 1)
        #if bit == 2:
            #error = pi.read(DOUT)
        bit = bit+1
    #print(int(value_bit,2))
    count = count+1
    cbTOR = pi.callback(DOUT, pigpio.RISING_EDGE, isr)
    print(time.time()-time_start)
    

if __name__ == '__main__':
    print('initializing')
    init_GPIO()
    

    while count < max_count:
        pass
    
