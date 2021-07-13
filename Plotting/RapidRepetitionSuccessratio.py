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
    plt.subplots(figsize=(10, 7))
    for groupSize in range(0,7):
        tmp = [0,0,0]
        for rr in [1,2,3]:
            tmp[rr-1] = np.mean(accomulation(array, groupSize, rr)) * 100
            plt.text(rr, tmp[rr-1], str(groupSize+1), style='italic')
        
        plt.plot([1,2,3], tmp, marker='.', markersize=10, color='r')

    plt.text(0,100, "1-7", style='italic')
    plt.plot(0,100, linestyle='none', marker='.', markersize=10, color='r')

    fig.tight_layout()  # otherwise the right y-label is slightly clipped
    plt.xticks([0,1,2,3], ['uc','bc rr 1','bc rr 2','bc rr 3'])
    # plt.savefig('/home/walther/Documents/bachelor/Graphs/latencySRPlot.png', dpi=1000)
    plt.title('Success ratio for unicast and broadcast with 1-3 rapid repitions')
    plt.ylabel('Success ratio in %')
    plt.xlabel('Type of Transmission')
    plt.grid()
    plt.show()

def successRatio_plot(array):


    std_error = np.std(array, axis=(0,2), ddof=1) / np.sqrt(np.size(array) / 100)
    print(std_error)
    mean = np.mean(array, axis=(0,2))
    print(mean)

    plt.subplots(figsize=(10, 7))
    x = ["#1","#2","#3","#4","#5","#6", "#7"]
    plt.errorbar(x, mean*100, yerr=std_error*100, fmt='-o', color='b', markersize=8, linestyle='none', capsize=7)
    plt.axis((0,SLAVE_COUNT, 0, 105))
    # axes.set_ylim([ymin,ymax])
    plt.minorticks_on()
    plt.tick_params(axis='x', which='minor', bottom=False) # no x ticks
    plt.title('Sucess ratio for each node')
    plt.ylabel('success ratio in %')
    plt.xlabel('node id')
    plt.grid()
    fig.patch.set_facecolor('xkcd:white')

    # plt.savefig('/home/walther/Documents/bachelor/Graphs/successRatioPerNode.png', dpi=300)
    plt.show()

print(decodeRaw(arraySR, 1))
# grouping_plot(arraySR)
# successRatio_plot(arraySR)