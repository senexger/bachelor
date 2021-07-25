
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

#%%
def grouping_plot(array): 

    fig.tight_layout()  # otherwise the right y-label is slightly clipped
    fig.set_size_inches(9, 8)

    plt.xticks(np.arange(0, 7, step=1))
    plt.yticks(np.arange(0, 110, 10.0))
    plt.minorticks_on()
    plt.title('Sucess ratio for grouping\n by an Rapid Repetition of 3\n')
    plt.grid()
    plt.savefig('/home/walther/Documents/bachelor/Graphs/grouping.png', dpi=2000)
    plt.show()

fig, ax1 = plt.subplots()
grouping_plot(arraySR)
#%%