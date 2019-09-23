from pathlib import Path
import csv
import sys
import RPi.GPIO as GPIO
import smbus
from tkinter import *
import time
import numpy
import _thread
import matplotlib.pyplot as plt

time_read = 3

P_RPM = 0
S_RPM = 0
strain = 0
checksum = 0
good = 0
brake_temp = 0

MOS_pin = 33
DPDT_pin = 16
servo_pin = 12
kill_pin = 18
reset_pin = 22

arduino_add = 0x03
ir_add = 0x5a
MLX90614_TOBJ1 = 0x07

log_rate = 150
log_period = 1./log_rate

data_index = 0
rpm_min = 1200.

wait_status = 1
status = 'Initializing'
bgcolor = 'green'
        
def init_I2C():
    return smbus.SMBus(bus = 1)

def init_GPIO():
    global servo
    GPIO.setmode(GPIO.BOARD)
    GPIO.setwarnings(False)
    
    GPIO.setup(MOS_pin, GPIO.OUT)
    GPIO.setup(DPDT_pin, GPIO.OUT)
    GPIO.setup(servo_pin, GPIO.OUT)
    GPIO.setup(kill_pin, GPIO.OUT)
    GPIO.setup(reset_pin, GPIO.OUT)
            
    GPIO.output(MOS_pin, 0)
    GPIO.output(DPDT_pin, 0)
    GPIO.output(kill_pin, 0)
    GPIO.output(reset_pin, 0)

    servo = GPIO.PWM(servo_pin, 50)
    servo.start(11)

def reset_arduino():
    GPIO.output(reset_pin, 1)
    time.sleep(.25)
    GPIO.output(reset_pin, 0)

def get_cal_data():
    global cal_b, cal_m
    f = open('/home/pi/Documents/DYNO/FINAL_CODE/cal_values.csv')
    csv_f = csv.reader(f)
    for row in csv_f:
        cal_m = float(row[0])
        cal_b = float(row[1])

def get_values(arduino_add):
    global P_RPM, P_RPM_timestamp, S_RPM, S_RPM_timestamp, strain, strain_timestamp
    try:
        val = bus.read_i2c_block_data(arduino_add, 0, 20)
    except:
        val = [0]*20
        val[19] = 1
    checksum = val[18]*256+val[19]
    if sum(val[0:18]) == checksum:
        P_RPM = val[0]*256+val[1]
        P_RPM_timestamp = val[2]*16777216+val[3]*65536+val[4]*256+val[5]
        S_RPM = val[6]*256+val[7]
        S_RPM_timestamp = val[8]*16777216+val[9]*65536+val[10]*256+val[11]
        strain = val[12]*256+val[13]
        strain_timestamp = val[14]*16777216+val[15]*65536+val[16]*256+val[17]

def step_brake(time_retract):
    global S_RPM, MOS_pin, DPDT_pin
    GPIO.output(MOS_pin, 0)
    GPIO.output(DPDT_pin, 1)
    GPIO.output(MOS_pin, 1)
    time.sleep(time_retract)
    GPIO.output(MOS_pin, 0)
            
def read_temp():
    global ir_add, MLX90614_TOBJ1
    try:
        temp = bus.read_word_data(ir_add, MLX90614_TOBJ1)
    except:
        temp = 0
    return temp*0.02-273.15

def extend_15():
    GPIO.output(MOS_pin, 0)
    GPIO.output(DPDT_pin, 0)
    GPIO.output(MOS_pin, 1)
    time.sleep(15.)
    GPIO.output(MOS_pin, 0)

def wait_button():
    global wait_status
    wait_status = 0

def E_STOP():
    global status, servo, root, window_open
    status = 'EMERGENCY STOP'
    servo.ChangeDutyCycle(11)
    GPIO.output(MOS_pin, 0)
    GPIO.output(kill_pin, 1)
    extend_15()
    window_open = 0
    root.destroy()
    GPIO.cleanup()
    sys.exit()
    
def initialize_window():
    global root, status, P_RPM, S_RPM, strain, data_index, window_open, bgcolor
    root.status_label = Label(text = str(status)+'\n', font = (None, 36))
    root.status_label.pack()
    root.blank_space1 = Label(text = '          ', background = bgcolor, font = (None, 36))
    root.blank_space1.pack()
    root.P_RPM_label = Label(text = 'Engine RPM', font = (None, 36))
    root.P_RPM_label.pack()
    root.P_RPM_value = Label(text = str(P_RPM), font = (None, 36))
    root.P_RPM_value.pack()
    root.S_RPM_label = Label(text = 'Secondary RPM', font = (None, 36))
    root.S_RPM_label.pack()
    root.S_RPM_value = Label(text = str(S_RPM), font = (None, 36))
    root.S_RPM_value.pack()
    root.setpoint_label = Label(text = 'Data Index', font = (None, 36))
    root.setpoint_label.pack()
    root.setpoint_value = Label(text = str(data_index), font = (None, 36))
    root.setpoint_value.pack()
    root.strain_label = Label(text = 'Torque (RAW)', font = (None, 36))
    root.strain_label.pack()
    root.strain_value = Label(text = str(strain), font = (None, 36))
    root.strain_value.pack()
    root.temp_label = Label(text = 'Rotor Temp (C)', font = (None, 36))
    root.temp_label.pack()
    root.temp_value = Label(text = str(read_temp()), font = (None, 36))
    root.temp_value.pack()

    root.blank_space2 = Label(text = '          ', background = bgcolor, font = (None, 36))
    root.blank_space2.pack()

    root.wait_go_button = Button(text = 'GO', font = (None, 36), command = wait_button)
    root.wait_go_button.pack()
    root.blank_space3 = Label(text = '          ', background = bgcolor, font = (None, 36))
    root.blank_space3.pack()
    root.abortButton = Button(text = 'EMERGENCY STOP', font = (None, 36), command = E_STOP)
    root.abortButton.pack()
    root.attributes('-fullscreen', True)
    window_open = 1

def update_display():
    global root, status, P_RPM, S_RPM, strain, data_index, bgcolor
    root.status_label.config(text = str(status))
    root.P_RPM_value.config(text = str(P_RPM))
    root.S_RPM_value.config(text = str(S_RPM))
    root.strain_value.config(text = str(strain))
    root.setpoint_value.config(text = str(data_index))
    temp = read_temp()
    root.temp_value.config(text = str(round(temp, 1)))
    if temp < 125:
        bgcolor = 'green'
    elif temp >= 125 and temp < 150:
        bgcolor = 'orange'
    elif temp >= 150 and temp < 175:
        bgcolor = 'red'
    root.configure(background = bgcolor)
    root.blank_space1.configure(background = bgcolor)
    root.blank_space2.configure(background = bgcolor)
    root.blank_space3.configure(background = bgcolor)
    if window_open:
        root.after(1000, update_display)
    else:
        root.destroy()

def main_thread():
    global wait_status, status, P_RPM, S_RPM, strain, P_RPM_timestamp, \
           S_RPM_timestamp, strain_timestamp, arduino_add, sample_period, \
           P_log, P_time_log, S_log, S_time_log, str_log, str_time_log, \
           rpm_min, rpm_max, MOS_pin, DPDT_pin, data_index, cal_b, cal_m

    def write_data():
        global cal_b, cal_m, data_index, P_log, P_time_log, S_log, \
               S_time_log, str_log, str_time_log
       
        def write_file(file, var, var_time, num_data):
            open(str(file), 'a')
            with open(str(file), 'a') as dataFile:
                writer = csv.writer(dataFile, quoting = csv.QUOTE_NONE)
                line = 0
                while line < num_data:
                    writer.writerow([var_time[line], var[line]])
                    line = line+1
                dataFile.close

        num_data = len(P_log)
        directory = '/media/usb1/'

        filename = directory+'P_'+str(data_index)+'.csv'
        file = Path(filename)
        write_file(file, P_log, P_time_log, num_data)

        filename = directory+'S_'+str(data_index)+'.csv'
        file = Path(filename)
        write_file(file, S_log, S_time_log, num_data)
        
        torque_log = [(float(x)-cal_b)/cal_m for x in str_log]
        filename = directory+'TORQUE_'+str(data_index)+'.csv'
        file = Path(filename)
        write_file(file, torque_log, str_time_log, num_data)

    get_values(arduino_add)
    step_brake(3.5)
    status = 'WAITING FOR ENGINE'
    while P_RPM < 1500:
        get_values(arduino_add)
        time.sleep(1)
    status = 'PRESS GO TO RUN'
    while wait_status:
        pass
    wait_status = 1
    servo.ChangeDutyCycle(4)
    status = 'INCREASING THROTTLE'
    time.sleep(10)
    status = 'RUNNING'
        
    cooldown_status = 0
    get_values(arduino_add)
    while (S_RPM >= rpm_min or S_RPM == 0):
        P_log = []
        P_time_log = []
        S_log = []
        S_time_log = []
        str_log = []
        str_time_log = []

        status = 'SETTLING'
        step_brake(0.05)
        status = 'LOCKED'
        time.sleep(4)

        status = 'READING'
        time_next = time.time()+time_read
        time_log=  time.time()+log_period
        while time.time() <= time_next:
            while time.time() <= time_log:
                pass
            get_values(arduino_add)
            P_log.append(P_RPM)
            P_time_log.append(P_RPM_timestamp)
            S_log.append(S_RPM)
            S_time_log.append(S_RPM_timestamp)
            str_log.append(strain)
            str_time_log.append(strain_timestamp)
            time_log = time_log+log_period

        write_data()
        data_index = data_index+1
        
    status = 'TEST_COMPLETED'
    E_STOP()

if __name__ == '__main__':
    init_GPIO()
    bus = init_I2C()
    reset_arduino()
    time.sleep(2)
    get_cal_data()
    root = Tk()
    initialize_window()
    update_display()
    _thread.start_new_thread(main_thread, ())
    
    root.mainloop()

    GPIO.cleanup()
