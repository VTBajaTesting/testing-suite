import smbus
from tkinter import *
import time
import _thread
import matplotlib.pyplot as plt

P_RPM = 0
S_RPM = 0
ADC_raw = 0
brake_temp = 0

sample_rate = 150
sample_period = 1./sample_rate
log_flag = 0

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

def get_values(arduino_add):
    try:
        val = bus.read_i2c_block_data(arduino_add, 0, 20)
    except:
        val = [0]*20
        val[19] = 1
    return val
        
def read_temp():
    global ir_add, MLX90614_TOBJ1
    temp = bus.read_word_data(ir_add, MLX90614_TOBJ1)
    return temp*0.02-273.15

def update_throttle(throttle):
    global servo
    duty = numpy.interp(float(throttle), [0., 100.],[10., 4.])
    servo.ChangeDutyCycle(duty)

def extend():
    GPIO.output(MOS_pin, 0)
    GPIO.output(DPDT_pin, 0)
    time.sleep(0.025)
    GPIO.output(MOS_pin, 1)
    time.sleep(0.25)
    GPIO.output(MOS_pin, 0)

def extend_10():
    GPIO.output(MOS_pin, 0)
    GPIO.output(DPDT_pin, 0)
    time.sleep(0.025)
    GPIO.output(MOS_pin, 1)
    time.sleep(10.)
    GPIO.output(MOS_pin, 0)

def retract():
    GPIO.output(MOS_pin, 0)
    GPIO.output(DPDT_pin, 1)
    time.sleep(0.025)
    GPIO.output(MOS_pin, 1)
    time.sleep(0.25)
    GPIO.output(MOS_pin, 0)
    GPIO.output(DPDT_pin, 0)
    time.sleep(0.025)

def log_data():
    global log_flag, root
    log_flag = not log_flag
    if log_flag:
        root.log_data_button.config(text = 'STOP LOGGING')
    else:
        root.log_data_button.config(text = 'START LOGGING')

def check_values(varx, vary, lower, upper):
    x = []
    y = []
    n = 0
    length = len(varx)
    time_min = varx[0]
    time_max = varx[length-1]
    while n < length:
        if varx[n] < time_min or varx[n] > time_max \
           or vary[n] < lower or vary[n] > upper:
            pass
        else:
            x.append(varx[n])
            y.append(vary[n])
        n = n+1
    return x, y

def generate_plot(P_log, P_time_log, S_log, S_time_log, str_log, str_time_log):
    [P_RPM_x, P_RPM_y]  = check_values(P_time_log, P_log, 0, 10000)
    [S_RPM_x, S_RPM_y] = check_values(S_time_log, S_log, 0, 10000)
    [STRAIN_x, STRAIN_y] = check_values(str_time_log, str_log, 30000, 50000)
    fig, axes = plt.subplots(2, 2)
    axes[0, 0].scatter(P_RPM_x, P_RPM_y)
    axes[1, 0].scatter(S_RPM_x, S_RPM_y)
    axes[0, 1].scatter(STRAIN_x, STRAIN_y)
    plt.show()

def shutdown():
    global servo, root
    servo.ChangeDutyCycle(11)
    GPIO.output(MOS_pin, 0)
    GPIO.output(kill_pin, 1)
    extend_10()
    root.quit()

def quit_fcn():
    global root, log_flag, window_open, P_log, P_time_log, S_log, \
           S_time_log, str_log, str_time_log
    servo.ChangeDutyCycle(11)
    GPIO.output(MOS_pin, 0)
    GPIO.output(kill_pin, 1)
    
    log_flag = 0
    window_open = 0
    save_data = input('Save data?  Y/N \n')
    if save_data == 'Y' or save_data == 'y':
        directory = '/media/usb1/'
        mydir = Path(directory)
        if mydir.is_dir():
            index = 0
            filename = directory+'P_RPM'+str(index)+'.csv'
            file = Path(filename)
            while file.is_file():
                index = index+1
                filename = directory+'P_RPM'+str(index)+'.csv'
                file = Path(filename)
            
            open(str(file), 'a')
            with open(str(file), 'a') as dataFile:
                writer = csv.writer(dataFile, quoting = csv.QUOTE_NONE)
                line = 0
                while line < len(P_time_log):
                    writer.writerow([P_time_log[line], P_log[line]])
                    line = line+1
                dataFile.close
            
            filename = directory+'S_RPM'+str(index)+'.csv'
            file = Path(filename)
            open(str(file), 'a')
            with open(str(file), 'a') as dataFile:
                writer = csv.writer(dataFile, quoting = csv.QUOTE_NONE)
                line = 0
                while line < len(S_time_log):
                    writer.writerow([S_time_log[line], S_log[line]])
                    line = line+1
                dataFile.close

            filename = directory+'STRAIN'+str(index)+'.csv'
            file = Path(filename)
            open(str(file), 'a')
            with open(str(file), 'a') as dataFile:
                writer = csv.writer(dataFile, quoting = csv.QUOTE_NONE)
                line = 0
                while line < len(str_time_log):
                    writer.writerow([str_time_log[line], str_log[line]])
                    line = line+1
                dataFile.close
            generate_plot(P_log, P_time_log, S_log, S_time_log, str_log, str_time_log)
    
def initialize_window():
    global root, P_RPM, S_RPM, strain, window_open
    bgcolor = 'green'
    root.P_RPM_label = Label(text = 'Engine RPM', font = (None, 28))
    root.P_RPM_label.pack()
    root.P_RPM_value = Label(text = str(P_RPM), font = (None, 28))
    root.P_RPM_value.pack()
    root.S_RPM_label = Label(text = 'Secondary RPM', font = (None, 28))
    root.S_RPM_label.pack()
    root.S_RPM_value = Label(text = str(S_RPM), font = (None, 28))
    root.S_RPM_value.pack()
    root.strain_label = Label(text = 'Torque (RAW)', font = (None, 28))
    root.strain_label.pack()
    root.strain_value = Label(text = str(strain), font = (None, 28))
    root.strain_value.pack()
    root.temp_label = Label(text = 'Rotor Temp (C)', font = (None, 28))
    root.temp_label.pack()
    root.temp_value = Label(text = str(0), font = (None, 28))
    root.temp_value.pack()

    root.blank_space1 = Label(text = '          ', background = bgcolor, font = (None, 28))
    root.blank_space1.pack()
    
    root.throttle_label = Label(text = 'THROTTLE', font = (None, 28))
    root.throttle_label.pack()
    root.throttle_slider = Scale(root, from_=0, to=100, length = 400, orient = HORIZONTAL, font = (None, 28), command = update_throttle)
    root.throttle_slider.set(0)
    root.throttle_slider.pack()
    root.blank_space2 = Label(text = '          ', background = bgcolor, font = (None, 28))
    root.blank_space2.pack()
        
    root.extend_button = Button(text = 'EXTEND ACTUATOR (0.25s)', font = (None, 28), command = extend)
    root.extend_button.pack()
    root.extend_10_button = Button(text = 'EXTEND ACTUATOR (10s)', font = (None, 28), command = extend_10)
    root.extend_10_button.pack()
    root.retract_button = Button(text = 'RETRACT ACTUATOR (0.25s)', font = (None, 28), command = retract)
    root.retract_button.pack()
    root.log_data_button = Button(text = 'START DATA LOGGING', font = (None, 28), command = log_data)
    root.log_data_button.pack()
    root.blank_space3 = Label(text = '          ', background = bgcolor, font = (None, 28))
    root.blank_space3.pack()
    root.abortButton = Button(text = 'ABORT', font = (None, 28), command = shutdown)
    root.abortButton.pack()
    root.quitButton = Button(text = 'Quit', font = (None, 28), command = quit_fcn)
    root.quitButton.pack()
    root.attributes('-fullscreen', True)
    window_open = 1

def update_display():
    global root, P_RPM, S_RPM, strain, bgcolor
    root.P_RPM_value.config(text = str(P_RPM))
    root.S_RPM_value.config(text = str(S_RPM))
    root.strain_value.config(text = str(strain))
    try:
        temp = read_temp()
    except:
        temp = 0; 
    root.temp_value.config(text = str(round(temp, 1)))
    if temp < 100:
        bgcolor = 'green'
    elif temp >= 100 and temp < 125:
        bgcolor = 'orange'
    elif temp >= 125 and temp < 150:
        bgcolor = 'red'
    else:
        shutdown()
    root.configure(background = bgcolor)
    root.blank_space1.configure(background = bgcolor)
    root.blank_space2.configure(background = bgcolor)
    root.blank_space3.configure(background = bgcolor)
    if window_open:
        root.after(1000, update_display)
    else:
        root.destroy()

def loop_thread():
    global P_RPM, S_RPM, strain, arduino_add, sample_period, log_flag, P_log, \
           P_time_log, S_log, S_time_log, str_log, str_time_log, window_open, errcount
    errcount = 
    time_sample = time.time()+sample_period
    P_log = []
    P_time_log = []
    S_log = []
    S_time_log = []
    str_log = []
    str_time_log = []
    while window_open:
        while time.time() < time_sample:
            pass
        val = get_values(arduino_add)
        P_RPM = val[0]*256+val[1]
        P_RPM_timestamp = val[2]*16777216+val[3]*65536+val[4]*256+val[5]
        S_RPM = val[6]*256+val[7]
        S_RPM_timestamp = val[8]*16777216+val[9]*65536+val[10]*256+val[11]
        strain = val[12]*256+val[13]
        strain_timestamp = val[14]*16777216+val[15]*65536+val[16]*256+val[17]
        checksum = val[18]*256+val[19]
        if sum(val[0:18]) == checksum:
            good = 1
        else:
            good = 0
            errcount = errcount+1
            print(str(errcount))
        if log_flag and good:
            P_log.append(P_RPM)
            P_time_log.append(P_RPM_timestamp)
            S_log.append(S_RPM)
            S_time_log.append(S_RPM_timestamp)
            str_log.append(strain)
            str_time_log.append(strain_timestamp)
        time_sample = time_sample+sample_period

if __name__ == '__main__':
    init_GPIO()
    reset_arduino()
    bus = init_I2C()
    root = Tk()
    initialize_window()
    update_display()
    _thread.start_new_thread(loop_thread, ())
    
    root.mainloop()

    GPIO.cleanup()
