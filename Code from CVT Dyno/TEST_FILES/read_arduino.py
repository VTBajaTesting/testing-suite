from pathlib import Path
import csv
import smbus
import time

address = 0x03
max_data = 2000
index = 0
P_RPM = [[int]*2 for i in range(max_data)]
S_RPM = [[int]*2 for i in range(max_data)]
STRAIN = [[int]*2 for i in range(max_data)]

def init_arduino():
    return smbus.SMBus(bus = 1)

def read_block(address):
    try:
        val = bus.read_i2c_block_data(address, 0, 18)
    except:
        val = [0]*18
    return val

def write_data(filename, max_data, variable):
    directory = '/media/usb1/'
    mydir = Path(directory)
    filename = directory+filename
    file = Path(filename)
    open(str(file), 'a')
    with open(str(file), 'a') as dataFile:
        writer = csv.writer(dataFile, quoting = csv.QUOTE_NONE)
        line = 0
        while line < max_data:
            writer.writerow(variable[line])
            line = line+1
        dataFile.close


if __name__ == '__main__':
    
    bus = init_arduino()
    while index < max_data:
        val = read_block(address)
        rpm_P = val[0]*256+val[1]
        rpm_P_timestamp = val[2]*16777216+val[3]*65536+val[4]*256+val[5]
        rpm_S = val[6]*256+val[7]
        rpm_S_timestamp = val[8]*16777216+val[9]*65536+val[10]*256+val[11]
        strain = val[12]*256+val[13]
        strain_timestamp = val[14]*16777216+val[15]*65536+val[16]*256+val[17]
        P_RPM[index][0] = rpm_P_timestamp
        P_RPM[index][1] = rpm_P
        S_RPM[index][0] = rpm_S_timestamp
        S_RPM[index][1] = rpm_S
        STRAIN[index][0] = strain_timestamp
        STRAIN[index][1] = strain
        index = index+1
        time.sleep(0.01)

    write_data('P_RPM_100.csv', max_data, P_RPM)
    write_data('S_RPM_100.csv', max_data, S_RPM)
    write_data('STRAIN_100.csv', max_data, STRAIN)
