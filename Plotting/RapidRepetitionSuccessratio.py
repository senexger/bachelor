# To add a new cell, type '# %%'
# To add a new markdown cell, type '# %% [markdown]'

# %%
# import data
import sys
import numpy as np
import pandas as pd
import matplotlib.ticker as tck
from matplotlib import pyplot as plt
from scipy import stats

np.set_printoptions(threshold=sys.maxsize)
#%%
TEST_REPETITION = 1000
SEND_REPETITION = 200
SLAVE_COUNT     = 7

arraySR = np.genfromtxt("/home/walther/Documents/bachelor/Data/latency/broadcast/broadcast7NodesRR3.csv", delimiter=",", dtype="int")
arraySR = np.reshape(arraySR, (TEST_REPETITION, SLAVE_COUNT, SEND_REPETITION))

fig, ax1 = plt.subplots()
#%%
def decodeBinaryArray(arr, rr):
    decodeArray = np.zeros(arr.shape, dtype=int)
    for w in range (0, TEST_REPETITION):

        for i in range (0, SEND_REPETITION):
            n = 0
            for rep in range (0,rr):
                n += arr[w][i] % 2
                arr[w][i] = arr[w][i] // 2
            decodeArray[w][i] = n
    return(decodeArray)

def decodeRaw(arr, rr):
    decodeArray = np.zeros(TEST_REPETITION*SEND_REPETITION, dtype=int)
    pos = 0
    if (rr == 1):
        for w in range (0, TEST_REPETITION):
            for i in range (0, SEND_REPETITION):
                if (arr[w][i] == 1 or arr[w][i] == 3 or arr[w][i] == 5 or arr[w][i] == 7):
                    decodeArray[pos] = 1
                pos = pos + 1
    if (rr == 2):
        for w in range (0, TEST_REPETITION):
            for i in range (0, SEND_REPETITION):
                if (arr[w][i] == 2 or arr[w][i] == 3 or arr[w][i] == 5 or arr[w][i] == 6 or arr[w][i] == 7):
                    decodeArray[pos] = 1
                pos = pos + 1
    if (rr == 3):
        for w in range (0, TEST_REPETITION):
            for i in range (0, SEND_REPETITION):
                if (arr[w][i]):
                    decodeArray[pos] = 1
                pos = pos + 1
    return(decodeArray)

def accomulation(array, groupSize, rapidRepitions):
    groups = np.zeros(np.size(arraySR[:,0,:]))
    
    for node in range(0, groupSize):
        groups = groups + decodeRaw(array[:,node,:], rapidRepitions)

    for i in range (0, np.size(arraySR[:,0,:])):
        if (groups[i] == groupSize):
            groups[i] = 1
        else:
            groups[i] = 0
    return groups

def grouping_plot(array): 
    array_time = [4,4,8,12]

    ax1.set_xlabel('type of transmission\nover group sizes from 1-7')
    ax1.set_ylabel('latency in ms', color='b')
    ax1.tick_params(axis='y', labelcolor='b')
    # plt.autoscale(False)

    ax1.plot(array_time[0], marker='.', markersize=10, color='b')
    ax1.plot([1,2,3], array_time[:3], marker='.', markersize=10, color='b')
    ax1.text(1, array_time[1], "1-7", style='italic')
    for i in range(1,8):
        ax1.text(0,i*4, str(i), style='italic')
        ax1.plot(i*4, linestyle='none', marker='.', markersize=10, color='b')


    ax2 = ax1.twinx()  # instantiate a second axes that shares the same x-axis
    
    ax2.set_ylabel('Success Ratio in %', color='red')  # we already handled the x-label with ax1
    ax2.tick_params(axis='y', labelcolor='r')
    plt.yticks(np.arange(0, 101, 10.0))
    plt.autoscale(False)
    for groupSize in range(0,7):
        tmp = [0,0,0]
        for rr in [1,2,3]:
            tmp[rr-1] = np.mean(accomulation(array, groupSize, rr)) * 100
            ax2.text(rr, tmp[rr-1], str(groupSize+1), style='italic')
        ax2.plot([1,2,3], tmp, marker='.', markersize=10, color='r')

    ax2.plot(0, 100, marker='.', markersize=10, color='r')
    ax2.text(0, 100, "1-7", style='italic')

    fig.tight_layout()  # otherwise the right y-label is slightly clipped
    fig.set_size_inches(9, 8)

    plt.xticks([0,1,2,3], ['uc','bc rr 1','bc rr 2','bc rr 3'])
    plt.minorticks_on()
    plt.title('Sucess ratio and latency\n of unicast and rapid repition broadcast transmissions\n')
    plt.grid()
    plt.savefig('/home/walther/Documents/bachelor/Graphs/SRvsRR_FINAL.png', dpi=2000)
    plt.show()

grouping_plot(arraySR)
#%%
def decodeRawArray(arr, rr):
    decodeArray = np.zeros((TEST_REPETITION*SEND_REPETITION, SLAVE_COUNT), dtype=int)
    pos = 0
    for node in range(0, SLAVE_COUNT):
        if (rr == 1):
            for w in range (0, TEST_REPETITION):
                for i in range (0, SEND_REPETITION):
                    if (arr[w][i] == 1 or arr[w][i] == 3 or arr[w][i] == 5 or arr[w][i] == 7):
                        decodeArray[pos][node] = 1
                    pos = pos + 1
        if (rr == 2):
            for w in range (0, TEST_REPETITION):
                for i in range (0, SEND_REPETITION):
                    if (arr[w][i] == 2 or arr[w][i] == 3 or arr[w][i] == 5 or arr[w][i] == 6 or arr[w][i] == 7):
                        decodeArray[pos][node] = 1
                    pos = pos + 1
        if (rr == 3):
            for w in range (0, TEST_REPETITION):
                for i in range (0, SEND_REPETITION):
                    if (arr[w][i]):
                        decodeArray[pos][node] = 1
                    pos = pos + 1
    return(decodeArray)

def successRatio_plot(array):

    std_errors = np.zeros(SLAVE_COUNT)
    means = np.zeros(SLAVE_COUNT)

    decodeRawArray

    rr = 2
    for node in range(0,SLAVE_COUNT):
        std_errors[node] = np.std((decodeRawArray(array[:,], rr)), ddof=1) / np.sqrt(np.size(np.std(decodeRaw(array[:,node,:], rr))) / 100)
        means[node] = np.mean(decodeRaw(array[:,:,:], rr))
    print(std_errors)
    print(means*100)

    plt.subplots(figsize=(10, 7))
    x = ["#1","#2","#3","#4","#5","#6", "#7"]
    plt.errorbar(x, means*100, yerr=std_errors, fmt='-o', color='b', markersize=8, linestyle='none', capsize=7)
    plt.axis((0,SLAVE_COUNT, 50, 105))
    plt.minorticks_on()
    plt.tick_params(axis='x', which='minor', bottom=False) # no x ticks
    plt.title('Sucess ratio broadcast RapidRepition = 2')
    plt.ylabel('Success Ratio in %')
    plt.xlabel('Slave ID')
    plt.grid()
    # fig.patch.set_facecolor('xkcd:white')

    plt.savefig('/home/walther/Documents/bachelor/Graphs/successRatioPerNode_RR2.png', dpi=1000)
    plt.show()

successRatio_plot(arraySR)