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

def sanitize_data_3d(data):
    '''Get data formatted and sort it for each node
    Input   array (599400, 7)
    Ooutput array (600,7,999)
    '''
    san_data = np.zeros((SEQ_SIZE, FIX_SIZE, EXP_SIZE), dtype=int)
    print(san_data.shape)

    for exp in range(0,EXP_SIZE):
        for fix in range(0,FIX_SIZE):
            for seq in range(0,SEQ_SIZE):
                san_data[seq,fix,exp] = data[seq+SEQ_SIZE*exp,fix]   

    return san_data

data = sanitize_data_3d(data_formatted)

# print(data[:,:,-1])
#%%
# Rapid Repetitions for node 4
# select node 4
data4 = data[:,3,:]
print(data4.shape)

def apply_rr_to_node(data_of_node, min_rr=0, max_rr=6):
    '''Transforms the data of a wes to an wireless vector 
    and calculates the output data for given repetitions

    data_node (600,999) input vector of data of selected node
    min_rr              minumum rapid repetition, default = 0
    max_rr              maximum rapid repetition, default = 6

    rr_data (600,999,6)
    '''

    rr_data = np.zeros((SEQ_SIZE, EXP_SIZE, max_rr-min_rr), dtype=int)

    for rep in range(min_rr, max_rr):
        for exp in range(0, EXP_SIZE):
            i = 0
            for seq in range(0, SEQ_SIZE):
                if (np.count_nonzero(data_of_node[seq:seq+rep+1, exp])):
                    rr_data[i, exp, rep] = 1
                i = i + 1
                if (i==SEQ_SIZE):
                    break

    return rr_data


rr_data_node3 = apply_rr_to_node(data[:,3,:])
print ((rr_data_node3).shape)
print(rr_data_node3[0:100,8,0])

#%% Calculate success ratio if using RR
REP_SIZE = 6

def calculate_rr_success_ratio(rr_data_of_node):
    '''given rapid repetion array
    calculates successratio for each experiemnt

    Input  (600,999,6) data array with adapted rr for one node
    Output (999, 6)    success ratio of each sequence
    '''
    rr_success_ratio_of_node = np.zeros((EXP_SIZE, REP_SIZE))

    for exp in range(0,EXP_SIZE):
        for rep in range(0,REP_SIZE):
            count_is_zero = np.count_nonzero(rr_data_of_node[:,exp,rep]==0)
            rr_success_ratio_of_node[exp, rep] = 1 - count_is_zero/SEQ_SIZE
    
    return rr_success_ratio_of_node

sr_result_node3 = calculate_rr_success_ratio(rr_data_node3)

print(sr_result_node3[:10,0])
print(sr_result_node3[:10,1])
print(sr_result_node3[:10,2])
#%% For Control: Success Ratio of data4 
# success_ratio_data4 = np.zeros((EXP_SIZE))

# for exp in range(0,SEQ_SIZE):
#     count_is_zero = np.count_nonzero(data4[:SEQ_SIZE, exp]==0)
#     success_ratio_data4[exp] = 1 - count_is_zero/SEQ_SIZE
# print(success_ratio_data4[0])

# success_ratio_data4 = np.mean(data4[:SEQ_SIZE], axis=0)
# print(success_ratio_data4[0])
#%%
#success ratio for a given node broadcast
def boxplot_sr_rapid_repetition_node_x(rr_data):
    ''' Plot Boxplot for given node with all Rapid Repetitions'''

    ax1.set_xlabel('Rapid Repetitions', color='black')
    ax1.set_ylabel('Success Ratio in %')

    ax1.boxplot(100*rr_data[::10,:], flierprops=flierprop_purp, medianprops=medianprops_purp)

    plt.figure(num=None, figsize=None, dpi=800)
    plt.show()
    plt.close()

    # plt.savefig('/home/walther/Documents/bachelor/Plot2/Graphs/RR_variation_Node4.pdf', dpi=1000)
    return

flierprop_purp = dict(marker='s', markerfacecolor='purple', markersize=3)
medianprops_purp = dict(linestyle='-', color='purple')

fig, ax1 = plt.subplots()
print(sr_result_node3.shape)
boxplot_sr_rapid_repetition_node_x(sr_result_node3)

#%%%%
# Try to show success rate of all nodes with diffenrent repetitions
rr_of_all_nodes = np.zeros((SEQ_SIZE ,EXP_SIZE, 6, FIX_SIZE), dtype=int)
sc_of_all_nodes = np.zeros((EXP_SIZE, 6, FIX_SIZE), dtype=int)

for node in range(0,1):
    rr_of_all_nodes[:,:,:,node] = apply_rr_to_node(data[:,node,:])
    sc_of_all_nodes[:,:,node]   = calculate_rr_success_ratio(rr_of_all_nodes[:,:,:,node])

#%%
# SEQ, EXP, RRs, NODE
print(rr_of_all_nodes[:200, 8, 0, 3])

# Success-Ratio, RRs, NODE
print(sc_of_all_nodes[0:10, 0, 3])

a = calculate_rr_success_ratio(rr_of_all_nodes[:,:,:,node])
boxplot_sr_rapid_repetition_node_x(a)

#%%
#success ratio for all nodes using rr
def boxplot_sr_rapid_repetition_multi_node(rr_data_x, rr):
    ''' Plot Boxplot for given node with all Rapid Repetitions'''

    ax1.set_xlabel('Rapid Repetitions', color='black')
    ax1.set_ylabel('Success Ratio in %')

    ax1.boxplot(100*rr_data_x[::10,rr,:], flierprops=flierprop_purp, medianprops=medianprops_purp)

    plt.figure(num=None, figsize=None, dpi=800)
    plt.show()
    plt.close()

    # plt.savefig('/home/walther/Documents/bachelor/Plot2/Graphs/RR_variation_Node4.pdf', dpi=1000)
    return

fig, ax1 = plt.subplots()
boxplot_sr_rapid_repetition_multi_node(sc_of_all_nodes, 0)
