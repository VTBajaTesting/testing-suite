from pathlib import Path
import csv
import RPi.GPIO as GPIO
import time

def log(channel):
    global freqlog, time_last, count
    time_cur = time.time()
    freqlog[count] = 1./(time_cur-time_last)
    time_last = time_cur
    count = count+1

pin_num = 11
num_data = 1000
time_last = time.time()
freqlog = [float]*num_data*2
count = 0
directory ="/media/usb1/"
mydir = Path(directory)

GPIO.setmode(GPIO.BOARD)
GPIO.setup(pin_num, GPIO.IN)

GPIO.add_event_detect(pin_num, GPIO.FALLING, callback = log)

while count < num_data:
    pass

if mydir.is_dir():
    print("Directory Exists")
    index = 0
    filename = directory+"hall_log"+str(index)+".csv"
    file = Path(filename)
    while file.is_file():
        index = index+1
        filename = directory+"hall_log"+str(index)+".csv"
        file = Path(filename)
    line = 0
    line_data = ''
    open(str(file), "a")
    with open(str(file), "a") as dataFile:
        writer = csv.writer(dataFile)
        while line < num_data:
            writer.writerow([str(freqlog[line])])
            line = line+1
        
    dataFile.close
else:
    print("invalid directory")
