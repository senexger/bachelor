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
data = np.zeros((SEQ_SIZE, FIX_SIZE, EXP_SIZE), dtype=int)
print(data.shape)

for exp in range(0,EXP_SIZE):
    for fix in range(0,FIX_SIZE):
        for seq in range(0,SEQ_SIZE):
            data[seq,fix,exp] = data_formatted[seq+SEQ_SIZE*exp,fix]   

#%% Success Ratio Rapid Repetition
RR=6

# shrink data to rr = 1
# Merges for each rr all transmissions of the same sequence to 1 or 0
data_rr = np.zeros((RR,100,FIX_SIZE,EXP_SIZE),dtype=int)

for exp in range(0,EXP_SIZE):
    for fix in range(0,FIX_SIZE):
        for seq in range(0,int(SEQ_SIZE/6)):
            for rr in range(0,RR):
                # TODO Ugliest code ever!
                if (rr==0):
                    data_rr[rr,seq,fix,exp] = data[seq,fix,exp]
                if (rr==1):
                    if (data[2*seq, fix, exp] or data[2*seq+1, fix, exp]):
                        data_rr[rr,seq,fix,exp] = 1
                if (rr==2):
                    if (data[3*seq, fix, exp] or data[3*seq+1, fix, exp] or data[3*seq+2, fix, exp]):
                        data_rr[rr,seq,fix,exp] = 1
                if (rr==3):
                    if (data[4*seq, fix, exp] or data[4*seq+1, fix, exp] or data[4*seq+2, fix, exp] or data[4*seq+3, fix, exp]):
                        data_rr[rr,seq,fix,exp] = 1
                if (rr==4):
                    if (data[5*seq, fix, exp] or data[5*seq+1, fix, exp] or data[5*seq+2, fix, exp] or data[5*seq+3, fix, exp] or data[5*seq+4, fix, exp]):
                        data_rr[rr,seq,fix,exp] = 1
                if (rr==5):
                    if (data[6*seq, fix, exp] or data[6*seq+1, fix, exp] or data[6*seq+2, fix, exp] or data[6*seq+3, fix, exp] or data[6*seq+4, fix, exp] or data[6*seq+5, fix, exp]):
                        data_rr[rr,seq,fix,exp] = 1
                if (rr==6):
                    if (data[7*seq, fix, exp] or data[7*seq+1, fix, exp] or data[7*seq+2, fix, exp] or data[7*seq+3, fix, exp] or data[7*seq+4, fix, exp] or data[7*seq+6, fix, exp] or data[7*seq+6, fix, exp]):
                        data_rr[rr,seq,fix,exp] = 1


sr_array = np.zeros((RR, EXP_SIZE,FIX_SIZE))

for exp in range(0,EXP_SIZE):
    for fix in range(0,FIX_SIZE):
        for rr in range(0,RR):
            count_is_zero = np.count_nonzero(data_rr[rr,:,fix,exp]==0)
            sr_array[rr,exp,fix] = 100 - count_is_zero


#%%
print(data[0:40,3,1])
#%%
#success ratio for each node broadcast
def sr_per_fixture_broadcast(data):
    sr_array = np.zeros((EXP_SIZE,FIX_SIZE))

    for exp in range(0,EXP_SIZE):
        for fix in range(0,FIX_SIZE):
            count_is_zero = np.count_nonzero(data[:,fix,exp]==0)
            sr_array[exp,fix] = 100* (1 - count_is_zero/SEQ_SIZE)
            # print(1- count_is_zero/SEQ_SIZE)

    ax1.set_xlabel('Fixture enum.', color='black')
    ax1.set_ylabel('Success Ratio in %') 
    
    ax1.boxplot(sr_array[::10,:], flierprops=flierprop_purp, medianprops=medianprops_purp)

    plt.figure(num=None, figsize=None, dpi=800)
    plt.show()
    
    # fig.savefig('Graphs/SR_per_fixture_broadcast.pdf', bbox_inches='tight') 
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


#%%
#success ratio for node 3 broadcast with rapid repetition

def sr_per_node_bc_rr(array):

    ax1.set_xlabel('Fixture enum.', color='black')
    ax1.set_ylabel('Success Ratio in %') 
    
    # ax1.boxplot(sr_array[0,::10,:], flierprops=flierprop_purp, medianprops=medianprops_purp)
    ax1.boxplot(sr_array[1,::10,:], flierprops=flierprop_purp, medianprops=medianprops_purp)

    plt.figure(num=None, figsize=None, dpi=799)
    plt.show()
    
    fig.savefig('Graphs/SR_per_fixture_rr1.pdf', bbox_inches='tight') 
    plt.close()

flierprop_purp = dict(marker='s', markerfacecolor='purple', markersize=3)
medianprops_purp = dict(linestyle='-', color='purple')

fig, ax1 = plt.subplots()
sr_per_node_bc_rr(sr_array)
#%%
#success ratio for given node  with rr [0,6]

def sr_of_node_bc_rr(array, node):

    ax1.set_xlabel('Number of Rapid Rapid Repetitions', color='black')
    ax1.set_ylabel('Success Ratio in %') 

    boxarray = [sr_array[0,::10,node], sr_array[1,::10,node], sr_array[2,::10,node], sr_array[3,::10,node], sr_array[4,::10,node], sr_array[5,::10,node]]
    
    ax1.boxplot(boxarray, flierprops=flierprop_purp, medianprops=medianprops_purp, positions=[0,1,2,3,4,5])

    plt.figure(num=None, figsize=None, dpi=799)
    plt.show()
    
    fig.savefig('Graphs/SR_of_node4_rr.pdf', bbox_inches='tight') 
    plt.close()

flierprop_purp = dict(marker='s', markerfacecolor='purple', markersize=3)
medianprops_purp = dict(linestyle='-', color='purple')

fig, ax1 = plt.subplots()
sr_of_node_bc_rr(sr_array, 3)