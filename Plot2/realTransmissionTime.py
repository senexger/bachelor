#%%
from matplotlib import pyplot as plt
import matplotlib.ticker as tck
from numpy import genfromtxt
import numpy as np
import sys

np.set_printoptions(threshold=sys.maxsize)

uc_transmissiontime_20byte        = np.zeros((10), dtype=int)
bc_transmissiontime_20byte        = np.zeros((10), dtype=int)
x  = np.arange(1,9,1)

for i in x:
    uc_transmissiontime_20byte[i-1]        = 1111 * i
    # bc_transmissiontime_20byte[i-1]        = 1834 * i
    bc_transmissiontime_20byte[i-1]        = (28+20*i)*8 + 50 + 144 +48+140

print(bc_transmissiontime_20byte)
# import data
uc_timestamps = genfromtxt('/home/walther/Documents/bachelor/Wireshark/uc_latency.csv', delimiter=",", dtype="int")
bc_timestamps = genfromtxt('/home/walther/Documents/bachelor/Wireshark/bc_latency.csv', delimiter=",", dtype="int")

#%%
def plot_real_tranmissiontime(uc, bc):
    bc_const_theory     = np.zeros(bc.shape)
    bc_const_measurment = np.zeros(bc.shape)
    for i in range (0,9):
        bc_const_theory[i] = bc[1]
        bc_const_measurment[i] = None
    bc_const_measurment[8] = bc[1]

    # with nice boxes
    ax.plot(x, uc[1:9]/1000, 'bo', label='Unicast measurment (20 byte)', markerfacecolor='none', linewidth=1.3, color='teal')
    ax.plot(x, uc_transmissiontime_20byte[0:8]/1000, '-', label='Unicast theory (20 byte)', markerfacecolor='none', linewidth=1.0, color='teal')
    ax.plot(x, bc_const_measurment[1:9]/1000, 'g^', label='Broadcast measurment (160 byte)', markerfacecolor='none', linewidth=1.3, color='darkmagenta')
    ax.plot(x, bc_transmissiontime_20byte[0:8]/1000, '-', label='Broadcast theory (20-160 byte)', markerfacecolor='none', linewidth=1.0, color='darkmagenta')
    ax.set_xlabel('Number of Stations')
    ax.set_ylabel('Update Interval in μs')
    ax.legend(loc='best', frameon=False)

    # plt.xlim(1,10)
    plt.ylim(0,10)
    plt.grid(True)

    fig.savefig('Graphs/bc_uc_transmissiontime_wireshark.pdf', bbox_inches='tight')  
    fig.show()  

fig, ax = plt.subplots()
plot_real_tranmissiontime(uc_timestamps, bc_timestamps)
fig.tight_layout()