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
    uc_transmissiontime_20byte[i-1]        = 1270 * i
    bc_transmissiontime_20byte[i-1]        = (28+20*i)*8 + 50 + 144 +48 +20*16.5
    bc_transmissiontime_200byte_const[i-1] = (28+200)*8 + 50 + 144 +48+20*16.5

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
    plt.ylim(0,13)
    plt.grid(True)

    # fig.savefig('Graphs/bc_uc_transmissiontime_analytic.pdf', bbox_inches='tight')  
    fig.show()  

fig, ax = plt.subplots()
plot_bc20_vs_bc200(ax)
fig.tight_layout()

#%% Broadcast increasing up to 400 Byte

def plot_bc_up_to_200(ax):
    bc_hack = np.zeros((11))
    for i in range(0,10):
        bc_hack[i+1] = bc_transmissiontime_20byte[i]
    bc_hack[0] = 765

    ax.plot(x2[:10], bc_transmissiontime_20byte/1000, label='Broadcast', markerfacecolor='none', linewidth=1.3, color='darkmagenta')
    ax.plot(x2[9:],  bc_hack/1000 + 2.066,                               markerfacecolor='none', linewidth=1.3, color='darkmagenta')

    ax.set_xlabel('Payload in Byte')
    ax.set_ylabel('Total Transmission Time in μs')
    ax.legend(loc='best', frameon=False)

    # plt.xlim(0,420)
    plt.ylim(0,10)
    plt.grid(True)

    # fig.savefig('Graphs/bc_analytic.pdf', bbox_inches='tight')  
    fig.show()  

fig, ax = plt.subplots()
plot_bc_up_to_200(ax)
fig.tight_layout()

#%% Transmission Time for delayed repetition
def plot_dr(ax):

    dr = [0,1,2,3,4,5]

    rr_0 = np.zeros((6), dtype=int)
    rr_1 = np.zeros((6), dtype=int)
    rr_2 = np.zeros((6), dtype=int)
    rr_3 = np.zeros((6), dtype=int)

    for i in range(0,6):
        rr_0[i] = bc_transmissiontime_200byte_const[0]
        rr_1[i] = bc_transmissiontime_200byte_const[0] * (2+i)
        rr_2[i] = bc_transmissiontime_200byte_const[0] * (3+2*i)
        rr_3[i] = bc_transmissiontime_200byte_const[0] * (4+3*i)

    # ax.plot(x, rr_0/1000, markerfacecolor='none', linewidth=1.3, color='darkmagenta')
    # ax.plot(x, rr_1/1000, markerfacecolor='none', linewidth=1.3, color='darkblue')
    # ax.plot(x, rr_2/1000, markerfacecolor='none', linewidth=1.3, color='blue')

    ax.plot(dr, rr_0/1000, '-', label='RR=0', markerfacecolor='none', linewidth=1.3, color='darkmagenta')
    ax.plot(dr, rr_1/1000, '-', label='RR=1', markerfacecolor='none', linewidth=1.3, color='darkblue')
    ax.plot(dr, rr_2/1000, '-', label='RR=2', markerfacecolor='none', linewidth=1.3, color='blue')
    ax.plot(dr, rr_3/1000, '-', label='RR=3', markerfacecolor='none', linewidth=1.3, color='royalblue')

    ax.set_xlabel('Delay of Repetition')
    ax.set_ylabel('Latency in μs')
    ax.legend(loc='best', frameon=False)

    # plt.xlim(1,10)
    # plt.ylim(0,13)
    plt.grid(True)

    # fig.savefig('Graphs/bc_dr.pdf', bbox_inches='tight')  
    fig.show()  

fig, ax = plt.subplots()
plot_dr(ax)
fig.tight_layout()