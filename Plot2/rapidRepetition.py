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

# print(data[:,:,-1])
#%%
# Rapid Repetitions for node 4
# select node 4
data4 = data[:,3,:]
print(data4.shape)

def apply_rr_to_node(data_node, min_rr=0, max_rr=6):
    '''Transforms the data of a wes to an wireless vector 
    and calculates the output data for given repetitions

    data_node   input vector of data of selected node
    min_rr      minumum rapid repetition, default = 0
    max_rr      maximum rapid repetition, default = 6
    '''

    rr_data = np.zeros((SEQ_SIZE, EXP_SIZE, max_rr-min_rr), dtype=int)

    for rep in range(min_rr, max_rr):
        for exp in range(0, EXP_SIZE):
            i = 0
            for seq in range(0, SEQ_SIZE):
                if (np.count_nonzero(data_node[seq:seq+rep+1, exp])):
                    rr_data[i, exp, rep] = 1
                i = i + 1
                if (i==SEQ_SIZE):
                    break

    return rr_data

rr_data_node3 = apply_rr_to_node(data[:,3,:])
print(rr_data_node3[0:100,8,0])

#%% Calculate success ratio if using RR
REP_SIZE = 6

rr_success_ratio = np.zeros((EXP_SIZE, REP_SIZE))

for exp in range(0,EXP_SIZE):
    for rep in range(0,REP_SIZE):
        count_is_zero = np.count_nonzero(rr_data_node3[:,exp,rep]==0)
        rr_success_ratio[exp, rep] = 1 - count_is_zero/SEQ_SIZE

print(rr_success_ratio[:10,0])
print(rr_success_ratio[:10,1])
print(rr_success_ratio[:10,2])
#%% For Control: Success Ratio of data4 
# success_ratio_data4 = np.zeros((EXP_SIZE))

# for exp in range(0,SEQ_SIZE):
#     count_is_zero = np.count_nonzero(data4[:SEQ_SIZE, exp]==0)
#     success_ratio_data4[exp] = 1 - count_is_zero/SEQ_SIZE
# print(success_ratio_data4[0])

# success_ratio_data4 = np.mean(data4[:SEQ_SIZE], axis=0)
# print(success_ratio_data4[0])
#%%
#success ratio for each node broadcast
def sr_rapid_repetition_node4(rr_data):

    ax1.set_xlabel('Rapid Repetitions', color='black')
    ax1.set_ylabel('Success Ratio in %') 
    # ax1.
    
    ax1.boxplot(rr_data[::10,:], flierprops=flierprop_purp, medianprops=medianprops_purp)

    plt.figure(num=None, figsize=None, dpi=800)
    plt.show()
    plt.close()
    
    plt.savefig('/home/walther/Documents/bachelor/Plot2/Graphs/RR_variation_Node4.pdf', dpi=1000)

flierprop_purp = dict(marker='s', markerfacecolor='purple', markersize=3)
medianprops_purp = dict(linestyle='-', color='purple')

fig, ax1 = plt.subplots()
print(rr_success_ratio.shape)
sr_rapid_repetition_node4(rr_success_ratio)