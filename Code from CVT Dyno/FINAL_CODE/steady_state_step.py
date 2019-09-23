from dyno_main_funcs import *
from tkinter import *
import time
import smbus
import _thread
from scipy.interpolate import interp1d
import os
import glob

P_RPM = 0
S_RPM = 0
ADC_raw = 0
P_time = 0
S_time = 0
ADC_time = 0
error = 0
data_index = 0

complete = 0

time_read = 3
sample_rate = 150.
log_period = 1/sample_rate

cal_b = 0
cal_m = 0

status = 'INITIALIZING'
wait_status = 1

def run(rpm_min):
    global servo, bus, complete
    # Main function to run the steady-state test case.
    # The minimum desired secondary RPM must be passed as an input.

    def delete_files():
        # Function to delete old data files from the flash drive.
        # All files not in a directory will be removed.

        for filename in glob.glob('/media/usb1/P_*'):
            os.remove(filename)
        for filename in glob.glob('/media/usb1/S_*'):
            os.remove(filename)
        for filename in glob.glob('/media/usb1/ADC_*'):
            os.remove(filename)
        for filename in glob.glob('/media/usb1/TORQUE_*'):
            os.remove(filename)

    def wait_button():
        global wait_status
        # Function to execute when the "GO" button is pressed.
        # This sets a flag to allow execution to continue,
        
        wait_status = 0

    def stst_window():
        global stst, servo, bus, status, wait_status, P_RPM, S_RPM, ADC_raw, data_index
        # Function to initialize the GUI.

        bgcolor = 'white'
        stst = Tk()
        stst.status_label = Label(text = str(status)+'\n', font = (None, 36))
        stst.status_label.pack()
        stst.blank_space1 = Label(text = '          ', background = bgcolor, font = (None, 36))
        stst.blank_space1.pack()
        stst.P_RPM_label = Label(text = 'Engine RPM', font = (None, 36))
        stst.P_RPM_label.pack()
        stst.P_RPM_value = Label(text = str(P_RPM), font = (None, 36))
        stst.P_RPM_value.pack()
        stst.S_RPM_label = Label(text = 'Secondary RPM', font = (None, 36))
        stst.S_RPM_label.pack()
        stst.S_RPM_value = Label(text = str(S_RPM), font = (None, 36))
        stst.S_RPM_value.pack()
        stst.setpoint_label = Label(text = 'Data Index', font = (None, 36))
        stst.setpoint_label.pack()
        stst.setpoint_value = Label(text = str(data_index), font = (None, 36))
        stst.setpoint_value.pack()
        stst.strain_label = Label(text = 'Torque (RAW)', font = (None, 36))
        stst.strain_label.pack()
        stst.strain_value = Label(text = str(ADC_raw), font = (None, 36))
        stst.strain_value.pack()
        stst.temp_label = Label(text = 'Rotor Temp (C)', font = (None, 36))
        stst.temp_label.pack()
        [temp, colorhex] = read_temp(bus)
        stst.temp_value = Label(text = str(temp), font = (None, 36))
        stst.temp_value.pack()

        stst.blank_space2 = Label(text = '          ', background = bgcolor, font = (None, 36))
        stst.blank_space2.pack()

        stst.wait_go_button = Button(text = 'GO', font = (None, 36), command = wait_button)
        stst.wait_go_button.pack()
        stst.blank_space3 = Label(text = '          ', background = bgcolor, font = (None, 36))
        stst.blank_space3.pack()
        stst.abortButton = Button(text = 'EMERGENCY STOP', font = (None, 36), command = lambda: E_STOP(servo, stst))
        stst.abortButton.pack()
        stst.attributes('-fullscreen', True)

    def update_display():
        global stst, servo, bus, status, wait_status, P_RPM, S_RPM, ADC_raw, data_index, complete
        # Function to update the data labels with "live" values.
        if complete == 0:
            # Update data values
            stst.status_label.config(text = str(status))
            stst.P_RPM_value.config(text = str(P_RPM))
            stst.S_RPM_value.config(text = str(S_RPM))
            stst.strain_value.config(text = str(ADC_raw))
            stst.setpoint_value.config(text = str(data_index))
            # Read temperature of brake rotor
            [temp, colorhex] = read_temp(bus)
            stst.temp_value.config(text = str(round(temp, 1)))
            # Update background color based on the rotor temperature
            stst.configure(bg = colorhex)
            stst.blank_space1.configure(background = colorhex)
            stst.blank_space2.configure(background = colorhex)
            stst.blank_space3.configure(background = colorhex)
            stst.after(1000, update_display)
        else:
            E_STOP(servo, stst)

    def test_thread():
        global stst, servo, bus, status, wait_status, P_RPM, S_RPM, ADC_raw, data_index, complete
        # The GUI window is an infinite loop and blocks further execution in the main thread.
        # This thread allows execution of the test code to continue in the background.
        # No inputs are passed and no variables are returned.

        def write_data(cal_b, cal_m, P_time_log, P_log, S_time_log, S_log, ADC_time_log, ADC_log):
            # This function writes three separate data files for each brake increment.
            # Calibration values and data lists are passed as inputs.

            write_file(P_time_log, P_log, 'P_'+str(data_index))
            write_file(S_time_log, S_log, 'S_'+str(data_index))
            write_file(ADC_time_log, ADC_log, 'ADC_'+str(data_index))
            # Convert raw ADC values to torque using calibration data
            TORQUE = convert_ADC_raw_torque(ADC_log, cal_b, cal_m)
            write_file(ADC_time_log, TORQUE, 'TORQUE_'+str(data_index))

        servo = set_throttle(servo, 0)
        # Get calibration data from file
        [cal_b, cal_m] = get_cal_data()
        # Call values from arduino
        [P_time, P_RPM, S_time, S_RPM, ADC_time, ADC_raw, error] = get_values(bus)
        # Retry transmission until successful
        while error == 1:
            [P_time, P_RPM, S_time, S_RPM, ADC_time, ADC_raw, error] = get_values(bus)
        # Move brake to initial position
        step_brake(3)
        # Wait for engine to start
        status = 'WAITING FOR ENGINE'
        while P_RPM < 1300:
            [P_time, P_RPM, S_time, S_RPM, ADC_time, ADC_raw, error] = get_values(bus)
            while error == 1:
                [P_time, P_RPM, S_time, S_RPM, ADC_time, ADC_raw, error] = get_values(bus)
            time.sleep(0.1)
        # Wait for user input to begin test
        wait_status = 1
        status = 'PRESS GO TO RUN'
        while wait_status:
           pass
        wait_status = 1
        # Set throttle to full
        servo = set_throttle(servo, 100)
        status = 'INCREASING THROTTLE'
        # Wait for system to stabilize
        for x in range(0, 10):
            [P_time, P_RPM, S_time, S_RPM, ADC_time, ADC_raw, error] = get_values(bus)
            while error == 1:
                [P_time, P_RPM, S_time, S_RPM, ADC_time, ADC_raw, error] = get_values(bus)
            time.sleep(1)
        # Continue test until secondary RPM decreases below the minimum desired value
        while S_RPM >= rpm_min or S_RPM == 0:
            # Initialize logging variables
            P_log = []
            P_time_log = []
            S_log = []
            S_time_log = []
            ADC_log = []
            ADC_time_log = []
            # Increment brake
            step_brake(0.05)
            # Lock brake and allow system to settle
            status = 'LOCKED'
            for x in range(0, 4):
                [P_time, P_RPM, S_time, S_RPM, ADC_time, ADC_raw, error] = get_values(bus)
                while error == 1:
                    [P_time, P_RPM, S_time, S_RPM, ADC_time, ADC_raw, error] = get_values(bus)
                time.sleep(1)
            # Record data for specified time interval
            status = 'READING'
            time_next = time.time()+time_read
            time_log = time.time()+log_period
            while time.time() <= time_next:
                while time.time() <= time_log:
                    pass
                [P_time, P_RPM, S_time, S_RPM, ADC_time, ADC_raw, error] = get_values(bus)
                # Append values to logging array if transmission was successful
                if error == 0:
                    P_log.append(P_RPM)
                    P_time_log.append(P_time)
                    S_log.append(S_RPM)
                    S_time_log.append(S_time)
                    ADC_log.append(ADC_raw)
                    ADC_time_log.append(ADC_time)
                    time_log = time_log+log_period
            # Write data arrays to files
            write_data(cal_b, cal_m, P_time_log, P_log, S_time_log, S_log, ADC_time_log, ADC_log)
            data_index = data_index+1
            while error == 1:
                [P_time, P_RPM, S_time, S_RPM, ADC_time, ADC_raw, error] = get_values(bus)
        status = 'TEST_COMPLETED'
        complete = 1

    # Delete files from previous data runs
    delete_files()
    # Initialize GPIO and I2C
    bus = init_I2C()
    servo = init_GPIO()
    reset_arduino()
    _thread.start_new_thread(test_thread, ())
    stst_window()
    update_display()
    stst.mainloop()

if __name__ == '__main__':
    run(1200)
    
