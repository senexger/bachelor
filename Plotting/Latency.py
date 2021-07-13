# To add a new cell, type '# %%'
# To add a new markdown cell, type '# %% [markdown]'

# %%
import numpy as np
import pandas as pd
import matplotlib.ticker as tck
from matplotlib import pyplot as plt
from scipy import stats

SEND_REPETITION = 200

# array_2 = np.genfromtxt("", delimiter=",", dtype="int")
array1 = np.genfromtxt("/home/walther/Documents/bachelor/Data/latency/unicast/latency_bc0_r200_wait2.csv", delimiter=",", dtype="int")
print(array1[:])
array2 = np.genfromtxt("/home/walther/Documents/bachelor/Data/latency/unicast/latency_bc0_r200_wait3.csv", delimiter=",", dtype="int")
print(array2[:])
array3 = np.genfromtxt("/home/walther/Documents/bachelor/Data/latency/unicast/latency_bc0_r200_wait4.csv", delimiter=",", dtype="int")
print(array3[:])
# array3= np.genfromtxt("/home/walther/Documents/bachelor/Data/broad_successratio/broadcast/far_bc1_size200_r40_rr1_wait0.csv", delimiter=",", dtype="int")
# array3 = np.delete(array3, -1)
# array3 = np.delete(array3, -1)
# print(array3[1:]) 

# %%

def plotting(array):
    x=np.arange(SEND_REPETITION)

    ax1.set_xlabel('Sequence')
    ax1.set_ylabel('Latency in ms', color='blue')
    # ax1.tick_params(axis='y', labelcolor='C0')
    # plt.xticks(np.arange(0, 41, 5))
    # plt.yticks(np.arange(0, 4, 1.0))
    # plt.autoscale(False)
    ax1.plot(x, array/1000, linestyle='none', marker='.', markersize=10, color='blue')

    ax2 = ax1.twinx()  # instantiate a second axes that shares the same x-axis
    
    ax2.set_ylabel('Success Ratio in %', color='red')  # we already handled the x-label with ax1
    ax2.tick_params(axis='y', labelcolor='red')
    # plt.yticks(np.arange(100, 2000, 500.0))
    # plt.autoscale(False)
    ax2.hlines(y=100, xmin=0, xmax=200, linewidth=2, color='red')
    plt.yticks(np.arange(0, 101, 10.0))

    fig.tight_layout()  # otherwise the right y-label is slightly clipped
    plt.savefig('/home/walther/Documents/bachelor/Graphs/latencyOverUnicast2ms.png', dpi=1000)
    plt.show()
    plt.close()

fig, ax1 = plt.subplots()
plotting(array1)
# %%

def box_plotting(array1, array2, array3):
    array = [array1/1000, array2/1000, array3/1000]
    x=np.arange(SEND_REPETITION)

    ax1.set_xlabel('Delay in ms')
    ax1.set_ylabel('Latency in ms')
    # plt.autoscale(False)

    ax1.boxplot(array)
    plt.xticks([1,2,3], ['2','3','4'])
    plt.savefig('/home/walther/Documents/bachelor/Graphs/latencyUnicast234.png', dpi=300)
    plt.show()

fig, ax1 = plt.subplots()
box_plotting(array1,array2,array3)
# %%
def latencySR_box_plotting(array1):
    bc_rr1 = np.array([SEND_REPETITION])
    bc_rr1.fill(4)
    bc_rr2 = np.array([SEND_REPETITION])
    bc_rr2.fill(8)
    bc_rr3 = np.array([SEND_REPETITION])
    bc_rr3.fill(12)
    array = [array1/1000, bc_rr1, bc_rr2, bc_rr3]
    x=np.arange(SEND_REPETITION)

    ax1.set_xlabel('Delay in ms')
    ax1.set_ylabel('Latency in ms')
    # plt.autoscale(False)


    ax1.boxplot(array)
    plt.xticks([1,2,3,4], ['unicast','rr 1','rr 2','rr 3'])
    plt.savefig('/home/walther/Documents/bachelor/Graphs/latencySR.png', dpi=500)
    plt.show()


fig, ax1 = plt.subplots()
latencySR_box_plotting(array3)
# %%
def latency_plot():

    array_time = [4,4,8,12]
    array_sr = [100,93,99,100]

    ax1.set_xlabel('Delay in ms')
    ax1.set_ylabel('Latency in ms', color='b')
    ax1.tick_params(axis='y', labelcolor='b')
    # plt.autoscale(False)

    ax1.plot(array_time, linestyle='none', marker='.', markersize=10, color='b')
    for i in range(1,9):
        ax1.text(0,i*4, str(i), style='italic')
        ax1.plot(i*4, linestyle='none', marker='.', markersize=10, color='b')


    ax2 = ax1.twinx()  # instantiate a second axes that shares the same x-axis
    
    ax2.set_ylabel('Success Ratio in %', color='red')  # we already handled the x-label with ax1
    ax2.tick_params(axis='y', labelcolor='r')
    plt.yticks(np.arange(0, 101, 10.0))
    plt.autoscale(False)
    ax2.plot(array_sr, linestyle='none', marker='.', markersize=10, color='r')

    fig.tight_layout()  # otherwise the right y-label is slightly clipped
    plt.xticks([0,1,2,3], ['unicast','rr 1','rr 2','rr 3'])
    plt.savefig('/home/walther/Documents/bachelor/Graphs/latencySRPlot.png', dpi=1000)
    plt.show()


fig, ax1 = plt.subplots()
latency_plot()
