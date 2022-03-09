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
    bc_transmissiontime_20byte[i-1]        = 1834 * i

# import data
uc_timestamps = genfromtxt('/home/walther/Documents/bachelor/Wireshark/uc_latency.csv', delimiter=",", dtype="int")
bc_timestamps = genfromtxt('/home/walther/Documents/bachelor/Wireshark/bc_latency.csv', delimiter=",", dtype="int")

#%%
def plot_real_tranmissiontime(uc, bc):
    # with nice boxes
    ax.plot(x, uc[1:9]/1000, 'bo', label='Unicast     (20 Byte)', markerfacecolor='none', linewidth=1.3, color='teal')
    ax.plot(x, uc_transmissiontime_20byte[0:8]/1000, '-', label='Unicast theory', markerfacecolor='none', linewidth=1.0, color='teal')
    ax.plot(x, bc[1:9]/1000, 'g^', label='Broadcast (160 Byte)', markerfacecolor='none', linewidth=1.3, color='darkmagenta')
    ax.plot(x, bc_transmissiontime_20byte[0:8]/1000, '-', label='Broadcast theory', markerfacecolor='none', linewidth=1.0, color='darkmagenta')
    ax.set_xlabel('Number of Transmissions')
    ax.set_ylabel('Transmission Time in μs')
    ax.legend(loc='best', frameon=False)

    # plt.xlim(1,10)
    plt.ylim(0,20)
    plt.grid(True)

    fig.savefig('Graphs/bc_uc_transmissiontime_wireshark.pdf', bbox_inches='tight')  
    fig.show()  

fig, ax = plt.subplots()
plot_real_tranmissiontime(uc_timestamps, bc_timestamps)
fig.tight_layout()