import csv
import os
import numpy
import matplotlib.pyplot as plt
import matplotlib

def get_filenames(prefix):
    path = os.sys.path[0]
    suffix = '.csv'
    filenames = os.listdir(path)
    return[filename for filename in filenames if filename.endswith(suffix) and filename.startswith(prefix)]

def get_data(file):
    varout = []
    index = 0
    path = os.path.join(os.sys.path[0], file)
    f = open(path)
    reader = csv.reader(f)
    data = list(list(rec) for rec in csv.reader(f))
    numel = len(data)
    for index in range(1, len(data)):
        if not data[index][1] == data[index-1][1]:
            varout.append(float(data[index][1]))
    return varout

def get_engine_data(file):
    path = os.path.join(os.sys.path[0], file)
    f = open(path)
    reader = csv.reader(f)
    data = list(list(rec) for rec in csv.reader(f))
    numel = len(data)
    RPM = []
    HP = []
    for index in range(0,numel):
        RPM.append(float(data[index][0]))
        HP.append(float(data[index][1]))
    return RPM, HP

def get_power(x, RPM, HP):
    power = numpy.interp(x, RPM, HP)
    return power

def write_results(file):
    path = os.path.join(os.sys.path[0], file)
    f = open(path, 'a')
    with open(path, 'a') as dataFile:
        writer = csv.writer(dataFile, quoting = csv.QUOTE_NONE)
        line = 0
        while line < len(P_avg):
            writer.writerow([P_avg[line], S_avg[line], T_avg[line], POWER[line], eff[line]])
            line = line+1
        dataFile.close

if __name__ == '__main__':
    P_list = get_filenames('P_')
    P_list_num = []
    for file in P_list:
        num1 = file.replace('P_', '')
        num2 = num1.replace('.csv', '')
        P_list_num.append(num2)
    P_list_num.sort(key = int)
    P_files = []
    for num in P_list_num:
        P_files.append('P_'+str(num)+'.csv')
    S_list = get_filenames('S_')
    S_list_num = []
    for file in S_list:
        num1 = file.replace('S_', '')
        num2 = num1.replace('.csv', '')
        S_list_num.append(num2)
    S_list_num.sort(key = int)
    S_files = []
    for num in S_list_num:
        S_files.append('S_'+str(num)+'.csv')
    T_list = get_filenames('TORQUE_')
    T_list_num = []
    for file in T_list:
        num1 = file.replace('TORQUE_', '')
        num2 = num1.replace('.csv', '')
        T_list_num.append(num2)
    T_list_num.sort(key = int)
    T_files = []
    for num in T_list_num:
        T_files.append('TORQUE_'+str(num)+'.csv')
    P_avg = []
    S_avg = []
    T_avg = []

    for file in P_files:
        P_RPM = get_data(file)
        if not file == P_list[0]:
            P_avg.append(sum(P_RPM)/len(P_RPM))

    for file in S_files:
        S_RPM = get_data(file)
        if not file == S_list[0]:
            S_avg.append(sum(S_RPM)/len(S_RPM))

    for file in T_files:
        TORQUE = get_data(file)
#        for index in range(0, len(TORQUE)):
#            wrong = TORQUE[index]
#            ADC_raw = wrong*108.1881+35144.7
#            TORQUE[index] = (ADC_raw-34760)/108.1881
        if not file == T_list[0]:
            T_avg.append(sum(TORQUE)/len(TORQUE))

    POWER = []
    index = 0
    for n in S_avg:
        POWER.append(n/60*2*3.14159*T_avg[index]*0.00134102)
        index = index+1

    fig1 = plt.figure(1)
    plt.scatter(S_avg, P_avg)
    plt.xlabel('Secondary RPM')
    plt.ylabel('Primary RPM')
    fig1.savefig('P_RPMvsS_RPM.png')
    fig2 = plt.figure(2)
    plt.scatter(S_avg, T_avg)
    plt.xlabel('Secondary RPM')
    plt.ylabel('Torque (N*m)')
    fig2.savefig('TORvsS_RPM.png')
    fig3 = plt.figure(3)
    plt.scatter(S_avg, POWER)
    plt.xlabel('Secondary RPM')
    plt.ylabel('Power (Hp)')
    fig3.savefig('HPvsS_RPM.png')

    [RPM, HP] = get_engine_data('engine_power_RPM.csv')
    eff = []
    num_points = len(P_avg)
    for index in range(0, num_points):
        power_in = get_power(P_avg[index], RPM, HP)
        power_out = S_avg[index]/60*2*3.14159*T_avg[index]*0.00134102
        eff.append(power_out/power_in)

    fig4 = plt.figure(4)
    plt.scatter(S_avg, eff)
    plt.ylim(0, 1)
    plt.xlabel('Secondary RPM')
    plt.ylabel('Overall Efficiency')
    fig4.savefig('EFFvsS_RPM.png')

    write_results('results.csv')
    
    plt.show()
