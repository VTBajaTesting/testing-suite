from dyno_main_funcs import *
import smbus
import matplotlib.pyplot as plt
from tkinter import *

# System dimensions in meters.
brake_dia = 0.3083
wire_dia = 0.00254
ratio = 32./61.
torque = 0.

torque_log = []
ADC_log = []

def run(num_data):
    global bus
    # Main function to determine strain gage calibration values.
    # The number of data points to collect for each torque is an input.

    def save_cal_data(torque_log, ADC_log):
        # Function to save calibration data to file
        slope, intercept, r_value, p_value, std_error = stats.linregress(torque_log, ADC_log)
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

    def collect():
        global calt, bus, torque, torque_log, ADC_log
        # Function to collect ADC data from the arduino and append values to running data vectors.
        mass_added = float(calt.mass_entry.get())
        torque = torque+(mass_added*9.81*(brake_dia+wire_dia))/2
        num_good = 0
        ADC_time_last = 0
        while num_good < num_data:
            [P_time, P_RPM, S_time, S_RPM, ADC_time, ADC_raw, error] = get_values(bus)
            # Confirm transmission was successful and not a duplicate data point
            if error == 0 and not ADC_time == ADC_time_last:
                torque_log.append(torque)
                ADC_log.append(ADC_raw)
                num_good = num_good+1
        plt.scatter(torque_log, ADC_log)
        plt.xlabel('Torque (N*m)')
        plt.ylabel('ADC Raw Output')
        mng = plt.get_current_fig_manager()
        mng.full_screen_toggle()
        plt.show(block = False)
        time.sleep(3)
        plt.close()

    def save_quit():
        global calt, torque_log, ADC_log
        # Function to save calibration data and close the window
        write_file(torque_log, ADC_log, 'CAL_DATA')
        save_cal_data(torque_log, ADC_log)
        calt.destroy()

    def no_save():
        global calt
        calt.quit()
        calt.destroy()

    def calt_window():
        global calt, bus
        # Function to initialize the GUI.

        calt = Tk()
        calt.blank_space1 = Label(text = '          ', font = (None, 36))
        calt.blank_space1.pack()
        calt.mass_label = Label(text = 'Mass Added to Rotor (kg):', font = (None, 36))
        calt.mass_label.pack()
        calt.mass_entry = Entry(font = (None, 36))
        calt.mass_entry.pack()
        calt.blank_space2 = Label(text = '          ', font = (None, 36))
        calt.blank_space2.pack()
        calt.torque_label = Label(text = 'Current Torque', font = (None, 36))
        calt.torque_label.pack
        calt.torque_value = Label(text = '0', font = (None, 36))
        calt.torque_value.pack
        calt.update_button = Button(text = 'Collect Data', font = (None, 36), command = collect)
        calt.update_button.pack()
        calt.save_button = Button(text = 'Save Data', font = (None, 36), command = save_quit)
        calt.save_button.pack()
        calt.blank_space3 = Label(text = '          ', font = (None, 36))
        calt.blank_space3.pack()
        calt.quit_button = Button(text = 'Quit Without Saving', font = (None, 36), command = no_save)
        calt.quit_button.pack()
        calt.attributes('-fullscreen', True)

    bus = init_I2C()
    servo = init_GPIO()
    reset_arduino()
    calt_window()
    calt.mainloop()

if __name__ == '__main__':
    run(500)
