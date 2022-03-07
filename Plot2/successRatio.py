#%%
from matplotlib import pyplot as plt
import matplotlib.ticker as tck
from numpy import genfromtxt
import numpy as np
import sys

np.set_printoptions(threshold=sys.maxsize)

square_red          = dict(markerfacecolor='red', marker='s')
square_purp         = dict(markerfacecolor='purple', marker='s', size=1)
medianprops_red     = dict(linestyle='-.', linewidth=1.5, color='red')
medianprops_purp    = dict(linestyle='-.', linewidth=1.5, color='purple')

SEQ_SIZE = 600
FIX_SIZE = 7
EXP_SIZE = 999

# import data
data_formatted = genfromtxt('/home/walther/Documents/bachelor/Plot2/removeme.csv', delimiter=",", dtype="int")
# print(data_formatted.shape)

# sanitize data to 3D array
data = np.zeros((SEQ_SIZE, FIX_SIZE, EXP_SIZE))
print(data.shape)

for exp in range(0,EXP_SIZE):
    for fix in range(0,FIX_SIZE):
        for seq in range(0,SEQ_SIZE):
            data[seq,fix,exp] = data_formatted[seq+SEQ_SIZE*exp,fix]   

# print(data[:,:,-1])
#%%
#success ratio for each node broadcast
def sr_per_fixture_broadcast(data):
    sr_array = np.zeros((EXP_SIZE,FIX_SIZE))

    for exp in range(0,EXP_SIZE):
        for fix in range(0,FIX_SIZE):
            count_is_zero = np.count_nonzero(data[:,fix,exp]==0)
            sr_array[exp,fix] = 100* (1 - count_is_zero/SEQ_SIZE)

    ax1.set_xlabel('Fixture enum.', color='black')
    ax1.set_ylabel('Success Ratio in %') 
    
    ax1.boxplot(sr_array[::10,:], flierprops=flierprop_purp, medianprops=medianprops_purp)

    plt.figure(num=None, figsize=None, dpi=800)
    plt.show()
    
    # plt.savefig('/home/walther/Documents/bachelor/Plot2/Graphs/SR_per_fixture_broadcast.png', dpi=1000)
    fig.savefig('Graphs/SR_per_fixture_broadcast.pdf', bbox_inches='tight') 
    plt.close()

flierprop_purp = dict(marker='s', markerfacecolor='purple', markersize=3)
medianprops_purp = dict(linestyle='-', color='purple')

fig, ax1 = plt.subplots()
sr_per_fixture_broadcast(data)

#%%
#success ratio for each node broadcast
def sr_per_fixture_unicast():
    sr_array = np.zeros((EXP_SIZE,FIX_SIZE))

    for exp in range(0,EXP_SIZE):
        for fix in range(0,FIX_SIZE):
            sr_array[exp,fix] = 1

    ax1.set_xlabel('Fixture enum.', color='black')
    ax1.set_ylabel('Success Ratio in %') 
    
    ax1.boxplot(sr_array[::10,:], flierprops=square_red, meanprops=medianprops_purp)

    plt.show()
    plt.close()

fig, ax1 = plt.subplots()
sr_per_fixture_unicast()