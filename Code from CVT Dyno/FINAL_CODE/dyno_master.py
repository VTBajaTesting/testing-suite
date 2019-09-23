# Dyno master code containing functions for all possible test cases.
# Script will run at startup and will not terminate until interrupted.
from dyno_main_funcs import *
import steady_state_step
import transient
import cal_torque
from subprocess import call
from tkinter import *

def run_steady():
    global root
    # Run steady-state code

    rpm_min = float(root.rpm_entry_steady.get())
    root.quit()
    root.destroy()
    steady_state_step.run(rpm_min)
    startup()

def run_transient():
    global root
    # Run transient code
    
    rpm_min = float(root.rpm_entry_tran.get())
    root.quit()
    root.destroy()
    transient.run(rpm_min)
    startup()
    

def run_cal():
    global root
    # Run calibration code

    num_data = float(root.num_entry.get())
    root.quit()
    root.destroy()
    cal_torque.run(num_data)
    startup()

def shutdown():
    global root
    # Shutdown the RPi
    
    root.destroy()
    call('sudo shutdown -h now', shell = True)

def initialize_window():
    global root
    # Initialize GUI main window

    root = Tk()
    root.blank_space1 = Label(text = '     ', bg = 'black', font = (None, 30))
    root.blank_space1.grid(row = 0, column = 0, sticky = W)
    root.blank_space2 = Label(text = '     ', bg = 'black', font = (None, 30))
    root.blank_space2.grid(row = 0, column = 3, sticky = W)
    path = '/home/pi/Documents/DYNO/FINAL_CODE/BAJA_LOGO.gif'
    baja_logo = PhotoImage(file = path)
    root.baja_logo = Label(root, image = baja_logo)
    root.baja_logo.photo = baja_logo
    root.baja_logo.grid(row = 0, column = 1, columnspan = 2, sticky = W)
    root.title = Label(text = 'VT Baja CVT Dynamometer', bg = 'black', fg = 'white', font = (None, 40))
    root.title.grid(row = 1, column = 1, columnspan = 2, sticky = W)
    root.steady = Label(text = '\nSteady State', bg = 'black', fg = 'white', font = (None, 30))
    root.steady.grid(row = 2, column = 1, sticky = W)
    root.rpm_min_steady = Label(text = 'Minimum RPM', bg = 'black', fg = 'white', font = (None, 30))
    root.rpm_min_steady.grid(row = 3, column = 1, sticky = W)
    root.rpm_entry_steady = Entry(width = 10, bg = 'white',font = (None, 30))
    root.rpm_entry_steady.insert(END, '1200')
    root.rpm_entry_steady.grid(row = 3, column = 2, sticky = W)
    root.steady_run = Button(text = 'Run', font = (None, 30), command = run_steady)
    root.steady_run.grid(row = 4, column = 1, sticky = W)

    root.transient = Label(text = '\nTransient', bg = 'black', fg = 'white', font = (None, 30))
    root.transient.grid(row = 5, column = 1, sticky = W)
    root.rpm_min_tran = Label(text = 'Minimum RPM', bg = 'black', fg = 'white', font = (None, 30))
    root.rpm_min_tran.grid(row = 6, column = 1, sticky = W)
    root.rpm_entry_tran = Entry(width = 10, bg = 'white',font = (None, 30))
    root.rpm_entry_tran.insert(END, '1200')
    root.rpm_entry_tran.grid(row = 6, column = 2, sticky = W)
    root.tran_run = Button(text = 'Run', font = (None, 30), command = run_transient)
    root.tran_run.grid(row = 7, column = 1, sticky = W)
    
    root.calibrate = Label(text = '\nCalibrate Torque', bg = 'black', fg = 'white', font = (None, 30))
    root.calibrate.grid(row = 8, column = 1, sticky = W)
    root.num_data = Label(text = 'Number of Data Points', bg = 'black', fg = 'white', font = (None, 30))
    root.num_data.grid(row = 9, column = 1, sticky = W)
    root.num_entry = Entry(width = 10, bg = 'white', font = (None, 30))
    root.num_entry.insert(END, '500')
    root.num_entry.grid(row = 9, column = 2, sticky = W)
    root.cal_run = Button(text = 'Run', font = (None, 30), command = run_cal)
    root.cal_run.grid(row = 10, column = 1, sticky = W)
    root.quitbutton = Button(text = 'QUIT', font = (None, 30), command = root.quit)
    root.quitbutton.grid(row = 8, column = 4)
    root.shutdown = Button(text = 'SHUTDOWN', font = (None, 30), command = shutdown)
    root.shutdown.grid(row = 10, column = 4, )
    root.configure(background = 'black')
    root.attributes('-fullscreen', True)

def startup():
    global root
    initialize_window()
    root.mainloop()
    
if __name__ == '__main__':
    startup()
