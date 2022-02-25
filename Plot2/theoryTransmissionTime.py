#%%
from codecs import backslashreplace_errors
import numpy as np
import matplotlib.pyplot as plt
from pylab import *

# setup data
x  = np.arange(1,11,1)
x2 = np.arange(20,420,20)

uc_transmissiontime_20byte        = np.zeros((10), dtype=int)
bc_transmissiontime_20byte        = np.zeros((10), dtype=int)
bc_transmissiontime_200byte_const = np.zeros((10), dtype=int)
# bc_transmissiontime_up_to_400     = np.zeros((20), dtype=int)

for i in x:
    uc_transmissiontime_20byte[i-1]        = 940 * i
    bc_transmissiontime_20byte[i-1]        = (28+20*i)*8 + 50 + 144 +48
    bc_transmissiontime_200byte_const[i-1] = (28+200)*8 + 50 + 144 +48

# second_bc = 0
# for i in x2:
#     if (220 == i):
#         second_bc = second_bc + 1
#     bc_transmissiontime_up_to_400[int(i/20)-1] = (28+20*i/20)*8 + 50 + 144 +48 + ((28+200)*8 + 50 + 144 +48)*second_bc

print(uc_transmissiontime_20byte)
print(bc_transmissiontime_20byte)
print(bc_transmissiontime_200byte_const)
# print(bc_transmissiontime_up_to_400)

#%%
def plot_bc20_vs_bc200(ax):
    # with nice boxes
    # ax.plot(x, uc_transmissiontime_20byte/1000, 'bs-', label='Unicast', markerfacecolor='none', linewidth=1, color='forestgreen')
    ax.plot(x, uc_transmissiontime_20byte/1000, '-', label='Unicast', markerfacecolor='none', linewidth=1.3, color='teal')
    ax.plot(x, bc_transmissiontime_20byte/1000, '-', label='Broadcast', markerfacecolor='none', linewidth=1.3, color='darkmagenta')
    ax.set_xlabel('Number of WES')
    ax.set_ylabel('Transmission Time in μs')
    ax.legend(loc='best', frameon=False)

    # plt.xlim(1,10)
    plt.ylim(0,10)
    plt.grid(True)

    fig.savefig('Graphs/bc_uc_transmissiontime_analytic.pdf', bbox_inches='tight')  
    fig.show()  

fig, ax = plt.subplots()
plot_bc20_vs_bc200(ax)
fig.tight_layout()

#%% Broadcast increasing up to 400 Byte

def plot_bc_up_to_200(ax):
    bc_hack = np.zeros((11))
    for i in range(0,10):
        bc_hack[i+1] = bc_transmissiontime_20byte[i]
    bc_hack[0] = 474

    ax.plot(x2[:10], bc_transmissiontime_20byte/1000, label='Broadcast', markerfacecolor='none', linewidth=1.3, color='darkmagenta')
    ax.plot(x2[9:],  bc_hack/1000 + 2.066,                               markerfacecolor='none', linewidth=1.3, color='darkmagenta')

    ax.set_xlabel('Payload in Byte')
    ax.set_ylabel('Total Transmission Time in μs')
    ax.legend(loc='best', frameon=False)

    # plt.xlim(0,420)
    plt.ylim(0,10)
    plt.grid(True)

    fig.savefig('Graphs/bc_analytic.pdf', bbox_inches='tight')  
    fig.show()  

fig, ax = plt.subplots()
plot_bc_up_to_200(ax)
fig.tight_layout()