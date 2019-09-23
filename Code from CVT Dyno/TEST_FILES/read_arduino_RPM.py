import smbus
import time

address = 0x04
cmd = 0x00

def init_arduino():
    return smbus.SMBus(bus = 1)

def read_val(address, cmd):
    val = bus.read_word_data(address, cmd)
    return val


if __name__ == '__main__':
    
    bus = init_arduino()
    while True:
        val = read_val(address, cmd)
        print(val)
        time.sleep(0.25)
