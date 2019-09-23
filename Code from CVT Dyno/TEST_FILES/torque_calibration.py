from pathlib import Path
import csv
import RPi.GPIO as GPIO
import smbus
import time
import matplotlib.pyplot as plt
from scipy import stats

strain = 0
strain_last = 0
strain_log = []
mass = 0
torque_log = []
mass_add = 0
timestamp = 0
timestamp_last = 0
checksum = 0
count_good = 0
num_data = 500
reset_pin = 22

brake_dia = 0.3083
wire_dia = 0.00254
ratio = 32./61.

arduino_add = 0x03
sample_sate = 150

def init_I2C():
    return smbus.SMBus(bus = 1)

def init_GPIO():
    GPIO.setmode(GPIO.BOARD)
    GPIO.setwarnings(False)
    GPIO.setup(reset_pin, GPIO.OUT)
    GPIO.output(reset_pin, 0)

def reset_arduino():
    GPIO.output(reset_pin, 1)
    time.sleep(0.1)
    GPIO.output(reset_pin, 0)
    time.sleep(5)

def get_torque():
    global count_good, torque, timestamp_last
    try:
        val = bus.read_i2c_block_data(arduino_add,0,20)
    except:
        val = [0]*20
        val[19] = 1
    checksum = val[18]*256+val[19]
    if sum(val[0:18]) == checksum:
        strain = val[12]*256+val[13]
        timestamp = val[14]*16777216+val[15]*65536+val[16]*256+val[17]
        if not timestamp == timestamp_last:
            count_good = count_good+1
            strain_log.append(strain)
            torque_log.append(float(torque))
            timestamp_last = timestamp

def write_file():
    directory = '/media/usb1/'
    mydir = Path(directory)
    if mydir.is_dir():
        index = 0
        filename = directory+'cal_data_'+str(index)+'.csv'
        file = Path(filename)
        while file.is_file():
            index = index+1
            filename = directory+'cal_data_'+str(index)+'.csv'
            file = Path(filename)
    open(str(file), 'a')
    with open(str(file), 'a') as dataFile:
        writer = csv.writer(dataFile, quoting = csv.QUOTE_NONE)
        line = 0
        while line < len(torque_log):
            writer.writerow([torque_log[line], strain_log[line]])
            line = line+1

def save_cal_data():
    slope, intercept, r_value, p_value, std_error = stats.linregress(torque_log, strain_log)
    directory = '/home/pi/Documents/DYNO/FINAL_CODE/'
    mydir = Path(directory)
    if mydir.is_dir():
        filename = directory+'cal_values.csv'
        file = Path(filename)
    
    open(str(file), 'a')
    with open(str(file), 'a') as dataFile:
        writer = csv.writer(dataFile, quoting = csv.QUOTE_NONE)
        line = 0
        writer.writerow([slope, intercept, r_value*r_value])

if __name__ == '__main__':
    global torque
    init_GPIO()
    reset_arduino()
    plt.ion()
    plt.show()
    bus = init_I2C()
    user_continue = input('Continue? Y/N \n')
    while user_continue == 'Y' or user_continue == 'y':
        mass_add = float(input('Mass Added to Hanging Weight (kg):\n'))
        input('Press Enter to Record Data')
        mass = mass+mass_add
        torque = mass*9.81*(brake_dia+wire_dia)/2*ratio
        while count_good < num_data:
            get_torque()
        count_good = 0
        plt.scatter(torque_log,strain_log)
        plt.draw()
        user_continue = input('Continue? Y/N \n')
    save_data = input('Save Data? Y/N\n');
    if save_data == 'Y' or save_data == 'y':
        write_file()
        save_cal_data()
    GPIO.cleanup()
        

            
