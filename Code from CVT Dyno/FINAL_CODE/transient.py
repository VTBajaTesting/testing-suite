from dyno_main_funcs import *
from tkinter import *
import time
import smbus
import _thread
from scipy.interpolate import interp1d

P_RPM = 0
S_RPM = 0
ADC_raw = 0
P_time = 0
S_time = 0
ADC_time = 0
error = 0

complete = 0

sample_rate = 150.
log_period = 1/sample_rate

time_upshift = 15

cal_b = 0
cal_m = 0

status = 'INITIALIZING'
wait_status = 1

def run(rpm_min):
    global servo, bus
    # Main function to run the transient test case.
    # The minimum desired secondary RPM must be passed as an input.

    def wait_button():
        global wait_status
        # Function to execute when the "GO" button is pressed.
        # This sets a flag to allow execution to continue,
        
        wait_status = 0

    def tran_window():
        global tran, servo, bus, status, wait_status, P_RPM, S_RPM, ADC_raw
        # Function to initialize the GUI.

        bgcolor = 'white'
        tran = Tk()
        tran.status_label = Label(text = str(status)+'\n', font = (None, 36))
        tran.status_label.pack()
        tran.blank_space1 = Label(text = '          ', background = bgcolor)
        tran.blank_space1.pack()
        tran.P_RPM_label = Label(text = 'Engine RPM', font = (None, 36))
        tran.P_RPM_label.pack()
        tran.P_RPM_value = Label(text = str(P_RPM), font = (None, 36))
        tran.P_RPM_value.pack()
        tran.S_RPM_label = Label(text = 'Secondary RPM', font = (None, 36))
        tran.S_RPM_label.pack()
        tran.S_RPM_value = Label(text = str(S_RPM), font = (None, 36))
        tran.S_RPM_value.pack()
        tran.strain_label = Label(text = 'Torque (RAW)', font = (None, 36))
        tran.strain_label.pack()
        tran.strain_value = Label(text = str(ADC_raw), font = (None, 36))
        tran.strain_value.pack()
        tran.temp_label = Label(text = 'Rotor Temp (C)', font = (None, 36))
        tran.temp_label.pack()
        [temp, colorhex] = read_temp(bus)
        tran.temp_value = Label(text = str(temp), font = (None, 36))
        tran.temp_value.pack()

        tran.blank_space2 = Label(text = '          ', background = bgcolor, font = (None, 36))
        tran.blank_space2.pack()

        tran.wait_go_button = Button(text = 'GO', font = (None, 36), command = wait_button)
        tran.wait_go_button.pack()
        tran.blank_space3 = Label(text = '          ', background = bgcolor, font = (None, 36))
        tran.blank_space3.pack()
        tran.abortButton = Button(text = 'EMERGENCY STOP', font = (None, 36), command = lambda: E_STOP(servo, tran))
        tran.abortButton.pack()
        tran.attributes('-fullscreen', True)

    def update_display():
        global tran, servo, bus, status, wait_status, P_RPM, S_RPM, ADC_raw, complete
        # Function to update the data labels with "live" values.

        if complete == 0:
            # Update data values
            tran.status_label.config(text = str(status))
            tran.P_RPM_value.config(text = str(P_RPM))
            tran.S_RPM_value.config(text = str(S_RPM))
            tran.strain_value.config(text = str(ADC_raw))
            # Read temperature of brake rotor
            [temp, colorhex] = read_temp(bus)
            tran.temp_value.config(text = str(round(temp, 1)))
            # Update background color based on the rotor temperature
            tran.configure(bg = colorhex)
            tran.blank_space1.configure(background = colorhex)
            tran.blank_space2.configure(background = colorhex)
            tran.blank_space3.configure(background = colorhex)
            tran.after(1000, update_display)
        else:
            E_STOP(servo, tran)

    def test_thread():
        global tran, servo, bus, status, wait_status, P_RPM, S_RPM, ADC_raw, complete
        # The GUI window is an infinite loop and blocks further execution in the main thread.
        # This thread allows execution of the test code to continue in the background.
        # No inputs are passed and no variables are returned.

        def write_data(cal_b, cal_m, P_time_log, P_log, S_time_log, S_log, ADC_time_log, ADC_log):
            # This function writes three separate data files for each brake increment.
            # Calibration values and data lists are passed as inputs.

            # Find next index for data file
            directory = '/media/usb1/'
            index = 0
            filename = directory+'P_TRAN_'+str(index)+'.csv'
            file = Path(filename)
            while file.isfile():
                index = index+1
                filename = directory+'P_TRAN_'+str(index)+'.csv'
                file = Path(filename)
            # Write files
            write_file(P_time_log, P_log, 'P_TRAN_'+str(index))
            write_file(S_time_log, S_log, 'S_TRAN_'+str(index))
            write_file(ADC_time_log, ADC_log, 'ADC_TRAN_'+str(index))
            # Convert raw ADC values to torque using calibration data
            TORQUE = convert_ADC_raw_torque(ADC_log, cal_b, cal_m)
            write_file(ADC_time_log, TORQUE, 'TORQUE_TRAN_'+str(index))

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
        # Initialize logging variables
        P_log = []
        P_time_log = []
        S_log = []
        S_time_log = []
        ADC_log = []
        ADC_time_log = []
        # Increment brake
        apply_brake()
        status = 'DOWNSHIFTING'
        time_log = time.time()+log_period
        while S_RPM >= rpm_min or error == 1:
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
        status = 'UPSHIFTING'
        release_brake()
        time_end = time.time()+time_upshift
        time_log = time.time()+log_period
        while time.time() <= time_end:
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
        status = 'TEST_COMPLETED'
        complete = 1

    # Initialize GPIO and I2C
    bus = init_I2C()
    servo = init_GPIO()
    reset_arduino()
    _thread.start_new_thread(test_thread, ())
    tran_window()
    update_display()
    tran.mainloop()

if __name__ == '__main__':
    run(1200)
    
