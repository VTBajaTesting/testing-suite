# Script to import DYNO data file and remove error points from the data set
import csv
import matplotlib.pyplot as plt

# Function to import .csv fils into list
def import_to_list(filename):
    with open(filename, 'r') as dataFile:
        reader = csv.reader(dataFile)
        return [list(map(int, rec)) for rec in csv.reader(dataFile, delimiter = ',')]

# Function to check data list and remove bad points
def check_values(varx, vary, lower, upper):
    x = []
    y = []
    n = 0
    length = len(var)
    time_min = var[0][0]
    time_max = var[length-1][0]
    while n < length:
        if var[n][0] < time_min or var[n][0] > time_max \
           or var[n][1] < lower or var[n][1] > upper:
            pass
        else:
            x.append(varx[n])
            y.append(vary[n])
        n = n+1
    return x, y

def generate_plot():
    [P_RPM_x, P_RPM_y]  = check_values(P_RPM_raw, 0, 10000)
    [S_RPM_x, S_RPM_y] = check_values(S_RPM_raw, 0, 10000)
    [STRAIN_x, STRAIN_y] = check_values(STRAIN_raw, 30000, 50000)
    fig, axes = plt.subplots(2, 2)
    axes[0, 0].scatter(P_RPM_x, P_RPM_y)
    axes[1, 0].scatter(S_RPM_x, S_RPM_y)
    axes[0, 1].scatter(STRAIN_x, STRAIN_y)
    plt.show()

if __name__ == '__main__':
    P_RPM_raw = import_to_list('P_RPM4.csv')
    S_RPM_raw = import_to_list('S_RPM4.csv')
    STRAIN_raw = import_to_list('STRAIN4.csv')
    [P_RPM_x, P_RPM_y]  = check_values(P_RPM_raw, 0, 10000)
    [S_RPM_x, S_RPM_y] = check_values(S_RPM_raw, 0, 10000)
    [STRAIN_x, STRAIN_y] = check_values(STRAIN_raw, 30000, 50000)
    fig, axes = plt.subplots(2, 2)
    axes[0, 0].scatter(P_RPM_x, P_RPM_y)
    axes[1, 0].scatter(S_RPM_x, S_RPM_y)
    axes[0, 1].scatter(STRAIN_x, STRAIN_y)
    mng = plt.get_current_fig_manager()
    #mng.window.state('zoomed')
    
    plt.show()
    
