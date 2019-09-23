# Test code to create a GUI
# Code modified from example found at www.youtube.com/watch?v=_lSNIrR1nZU

# This will be the input screen for the steady state dyno test


from tkinter import *
from tkinter.font import Font
from subprocess import call

def click_exit():
    main_window.destroy()
    exit()
def click_shutdown():
    main_window.destroy()
    call('sudo shutdown -h now', shell = True)
def click_reboot():
    main_window.destroy()
    call('sudo reboot', shell = True)

# Open main window
main_window = Tk()
main_window.attributes('-fullscreen', True)
main_window.title('Baja CVT Dyno Steady State')
main_window.configure(background = 'black')

main_window.option_add('*Font', 'Times 22')

# Empty left column for spacing
Label (main_window, text = '          ', bg = 'black', fg = 'white', \
       font = 'Times 18 bold') .grid(row = 0, column = 0, sticky = W)

# Add picture
baja_logo = PhotoImage(file = '~/Documents/DYNO/TEST_FILES/BAJA_BW_2.gif')
Label (main_window, image = baja_logo, bg = 'black') .grid(row = 0, column = 1, columnspan = 2,sticky = W)

# Add text and text boxes
Label (main_window, text = 'CVT DYNAMOMETER', \
       bg = 'black',fg = 'white',) .grid(row = 1, column = 1, columnspan = 2, sticky = W)
Label (main_window, text = 'STEADY STATE TEST PARAMETERS \n', \
       bg = 'black',fg = 'white',) .grid(row = 2, column = 1, columnspan = 2, sticky = W)

Label (main_window, text = 'Maximum Secondary RPM', bg = 'black', fg = 'white') .grid(row = 3, column = 1, sticky = W)
max_default = StringVar(main_window, value = '6800')
max_S_RPM = Entry(main_window, textvariable = max_default, width = 10, bg = 'white')
max_S_RPM.grid(row = 4, column = 1, sticky = W)

Label (main_window, text = '\nMinumum Secondary RPM', bg = 'black', fg = 'white') .grid(row = 5, column = 1, sticky = W)
min_default = StringVar(main_window, value = '1100')
min_S_RPM = Entry(main_window, textvariable = min_default, width = 10, bg = 'white')
min_S_RPM.grid(row = 6, column = 1, sticky = W)

Label (main_window,text = '\nNumber of Increments', bg = 'black', fg = 'white') .grid(row = 7, column = 1, sticky = W)
num_increment_default = StringVar(main_window, value = '10')
num_increment = Entry(main_window, textvariable = num_increment_default, width = 10, bg = 'white')
num_increment.grid(row = 8, column = 1, sticky = W)

Label (main_window,text = '\nSettling Time (seconds)', bg = 'black', fg = 'white') .grid(row = 9, column = 1, sticky = W)
settle_default = StringVar(main_window, value = '5')
settle = Entry(main_window, textvariable = settle_default, width = 10, bg = 'white')
settle.grid(row = 10, column = 1, sticky = W)

Button(main_window, text = 'NEXT', width = 18, fg = 'black', state = DISABLED) .grid(row = 5, column = 2,sticky = E)

Button(main_window, text = 'EXIT TO DESKTOP', width = 18, fg = 'black', command = click_exit) .grid(row = 8, column = 2, sticky = E)

Button(main_window, text = 'SHUTDOWN', width = 18, fg = 'black', command = click_shutdown) .grid(row = 9, column = 2, sticky = E)

Button(main_window, text = 'RESTART', width = 18, fg = 'black', command = click_reboot) .grid(row = 10, column = 2, sticky = E)


# Main loop
main_window.mainloop()
