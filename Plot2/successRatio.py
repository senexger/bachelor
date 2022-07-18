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

    print(np.average(sr_array, axis=0))

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
#success ratio for node 3 broadcast with rapid repetition
# TODO This mastermind function should do the DR Job, just by getting a shifted input!

def sr_per_node_bc_rr(array, rr):

    ax1.set_xlabel('Fixture enum.', color='black')
    ax1.set_ylabel('Success Ratio in %') 
    
    # ax1.boxplot(sr_array[0,::10,:], flierprops=flierprop_purp, medianprops=medianprops_purp)
    ax1.boxplot(sr_array[rr,::10,:], flierprops=flierprop_purp, medianprops=medianprops_purp)

    print(np.average(sr_array[rr,:,:], axis=0))
    print(np.average(np.average(sr_array[rr,:,:], axis=0)))

    plt.figure(num=None, figsize=None, dpi=799)
    plt.show()
    
    # fig.savefig('Graphs/SR_per_fixture_rrX.pdf', bbox_inches='tight') 
    plt.close()

flierprop_purp = dict(marker='s', markerfacecolor='purple', markersize=3)
medianprops_purp = dict(linestyle='-', color='purple')

fig, ax1 = plt.subplots()
sr_per_node_bc_rr(sr_array, 3)
#%%
#success ratio for given node  with rr [0,6]

def sr_of_node_bc_rr(array, node):

    ax1.set_xlabel('Number of Rapid Rapid Repetitions', color='black')
    ax1.set_ylabel('Success Ratio in %') 

    boxarray = [sr_array[0,::10,node], sr_array[1,::10,node], sr_array[2,::10,node], sr_array[3,::10,node], sr_array[4,::10,node], sr_array[5,::10,node]]

    ax1.boxplot(boxarray, flierprops=flierprop_purp, medianprops=medianprops_purp, positions=[0,1,2,3,4,5])

    plt.figure(num=None, figsize=None, dpi=799)
    plt.show()
    
    # fig.savefig('Graphs/SR_of_node4_rr.pdf', bbox_inches='tight') 
    plt.close()

flierprop_purp = dict(marker='s', markerfacecolor='purple', markersize=3)
medianprops_purp = dict(linestyle='-', color='purple')

fig, ax1 = plt.subplots()
sr_of_node_bc_rr(sr_array, 3)

#%% Delayed Repetitions
# dummyData = np.zeros((600,7,999), dtype=int)

# for exp in range(0,EXP_SIZE):
#     for fix in range(0,FIX_SIZE):
#         for seq in range(0,SEQ_SIZE):
#             dummyData[seq,fix,exp] = seq

# print(dummyData[0:10,0,0])

#%%


def ugly_dr1(rr):
    data_dr = np.zeros((600,7,999), dtype=int)
    # TODO fix EXP_SIZE + FIX_SIZE
    for exp in range(0,EXP_SIZE):
        for fix in range(0,FIX_SIZE):
            for seq in range(0,SEQ_SIZE):
                if rr == 0:
                    return data
                if rr == 1:
                    if (seq%4 == 0):
                        data_dr[seq, fix, exp] = data[seq, fix, exp]
                    if (seq%4 == 1):
                        data_dr[seq, fix, exp] = data[seq+1, fix, exp]
                    if (seq%4 == 2):
                        data_dr[seq, fix, exp] = data[seq-1, fix, exp]
                    if (seq%4 == 3):
                        data_dr[seq, fix, exp] = data[seq, fix, exp]
                if rr == 2:
                    if (seq%6 == 0):
                        data_dr[seq, fix, exp] = data[seq, fix, exp]
                    if (seq%6 == 1):
                        data_dr[seq, fix, exp] = data[seq+2, fix, exp]
                    if (seq%6 == 2):
                        data_dr[seq, fix, exp] = data[seq-1, fix, exp]
                    if (seq%6 == 3):
                        data_dr[seq, fix, exp] = data[seq+1, fix, exp]
                    if (seq%6 == 4):
                        data_dr[seq, fix, exp] = data[seq-2, fix, exp]
                    if (seq%6 == 5):
                        data_dr[seq, fix, exp] = data[seq, fix, exp]
                if rr == 3:
                    if (seq%8 == 0):
                        data_dr[seq, fix, exp] = data[seq, fix, exp]
                    if (seq%8 == 1):
                        data_dr[seq, fix, exp] = data[seq+3, fix, exp]
                    if (seq%8 == 2):
                        data_dr[seq, fix, exp] = data[seq-1, fix, exp]
                    if (seq%8 == 3):
                        data_dr[seq, fix, exp] = data[seq+2, fix, exp]
                    if (seq%8 == 4):
                        data_dr[seq, fix, exp] = data[seq-2, fix, exp]
                    if (seq%8 == 5):
                        data_dr[seq, fix, exp] = data[seq+1, fix, exp]
                    if (seq%8 == 6):
                        data_dr[seq, fix, exp] = data[seq-3, fix, exp]
                    if (seq%8 == 7):
                        data_dr[seq, fix, exp] = data[seq, fix, exp]
    return data_dr

def ugly_dr2(rr):
    data_dr = np.zeros((600,7,999), dtype=int)
    # TODO fix EXP_SIZE + FIX_SIZE
    for exp in range(0,EXP_SIZE):
        for fix in range(0,FIX_SIZE):
            for seq in range(0,SEQ_SIZE-20):
                if rr == 0:
                    return data
                if rr == 1:
                    if (seq%6 == 0):
                        data_dr[seq, fix, exp] = data[seq, fix, exp]
                    if (seq%6 == 1):
                        data_dr[seq, fix, exp] = data[seq+1, fix, exp]
                    if (seq%6 == 2):
                        data_dr[seq, fix, exp] = data[seq+2, fix, exp]
                    if (seq%6 == 3):
                        data_dr[seq, fix, exp] = data[seq-2, fix, exp]
                    if (seq%6 == 4):
                        data_dr[seq, fix, exp] = data[seq-1, fix, exp]
                    if (seq%6 == 5):
                        data_dr[seq, fix, exp] = data[seq, fix, exp]
                if rr == 2:
                    if (seq%9 == 0):
                        data_dr[seq, fix, exp] = data[seq, fix, exp]
                    if (seq%9 == 1):
                        data_dr[seq, fix, exp] = data[seq+2, fix, exp]
                    if (seq%9 == 2):
                        data_dr[seq, fix, exp] = data[seq+4, fix, exp]
                    if (seq%9 == 3):
                        data_dr[seq, fix, exp] = data[seq-2, fix, exp]
                    if (seq%9 == 4):
                        data_dr[seq, fix, exp] = data[seq, fix, exp]
                    if (seq%9 == 5):
                        data_dr[seq, fix, exp] = data[seq+2, fix, exp]
                    if (seq%9 == 6):
                        data_dr[seq, fix, exp] = data[seq-4, fix, exp]
                    if (seq%9 == 7):
                        data_dr[seq, fix, exp] = data[seq-2, fix, exp]
                    if (seq%9 == 8):
                        data_dr[seq, fix, exp] = data[seq, fix, exp]
                if rr == 3:
                    if (seq%12 == 0):
                        data_dr[seq, fix, exp] = data[seq, fix, exp]
                    if (seq%12 == 1):
                        data_dr[seq, fix, exp] = data[seq+3, fix, exp]
                    if (seq%12 == 2):
                        data_dr[seq, fix, exp] = data[seq+6, fix, exp]
                    if (seq%12 == 3):
                        data_dr[seq, fix, exp] = data[seq-2, fix, exp]
                    if (seq%12 == 4):
                        data_dr[seq, fix, exp] = data[seq+1, fix, exp]
                    if (seq%12 == 5):
                        data_dr[seq, fix, exp] = data[seq+4, fix, exp]
                    if (seq%12 == 6):
                        data_dr[seq, fix, exp] = data[seq-4, fix, exp]
                    if (seq%12 == 7):
                        data_dr[seq, fix, exp] = data[seq-1, fix, exp]
                    if (seq%12 == 8):
                        data_dr[seq, fix, exp] = data[seq+2, fix, exp]
                    if (seq%12 == 9):
                        data_dr[seq, fix, exp] = data[seq-6, fix, exp]
                    if (seq%12 == 10):
                        data_dr[seq, fix, exp] = data[seq-3, fix, exp]
                    if (seq%12 == 11):
                        data_dr[seq, fix, exp] = data[seq, fix, exp]
    return data_dr

def ugly_dr3(rr):
    data_dr = np.zeros((600,7,999), dtype=int)
    # TODO fix EXP_SIZE + FIX_SIZE
    for exp in range(0,EXP_SIZE):
        for fix in range(0,FIX_SIZE):
            for seq in range(0,SEQ_SIZE-20):
                if rr == 0:
                    return data
                if rr == 1:
                    if (seq%8 == 0):
                        data_dr[seq, fix, exp] = data[seq, fix, exp]
                    if (seq%8 == 1):
                        data_dr[seq, fix, exp] = data[seq+1, fix, exp]
                    if (seq%8 == 2):
                        data_dr[seq, fix, exp] = data[seq+2, fix, exp]
                    if (seq%8 == 3):
                        data_dr[seq, fix, exp] = data[seq+3, fix, exp]
                    if (seq%8 == 4):
                        data_dr[seq, fix, exp] = data[seq-3, fix, exp]
                    if (seq%8 == 5):
                        data_dr[seq, fix, exp] = data[seq-2, fix, exp]
                    if (seq%8 == 6):
                        data_dr[seq, fix, exp] = data[seq-1, fix, exp]
                    if (seq%8 == 7):
                        data_dr[seq, fix, exp] = data[seq, fix, exp]
                if rr == 2:
                    if (seq%12 == 0):
                        data_dr[seq, fix, exp] = data[seq, fix, exp]
                    if (seq%12 == 1):
                        data_dr[seq, fix, exp] = data[seq+2, fix, exp]
                    if (seq%12 == 2):
                        data_dr[seq, fix, exp] = data[seq+4, fix, exp]
                    if (seq%12 == 3):
                        data_dr[seq, fix, exp] = data[seq+6, fix, exp]
                    if (seq%12 == 4):
                        data_dr[seq, fix, exp] = data[seq-3, fix, exp]
                    if (seq%12 == 5):
                        data_dr[seq, fix, exp] = data[seq-1, fix, exp]
                    if (seq%12 == 6):
                        data_dr[seq, fix, exp] = data[seq+1, fix, exp]
                    if (seq%12 == 7):
                        data_dr[seq, fix, exp] = data[seq+3, fix, exp]
                    if (seq%12 == 8):
                        data_dr[seq, fix, exp] = data[seq-6, fix, exp]
                    if (seq%12 == 9):
                        data_dr[seq, fix, exp] = data[seq-4, fix, exp]
                    if (seq%12 == 10):
                        data_dr[seq, fix, exp] = data[seq-2, fix, exp]
                    if (seq%12 == 11):
                        data_dr[seq, fix, exp] = data[seq, fix, exp]
                if rr == 3:
                    if (seq%16 == 0):
                        data_dr[seq, fix, exp] = data[seq, fix, exp]
                    if (seq%16 == 1):
                        data_dr[seq, fix, exp] = data[seq+3, fix, exp]
                    if (seq%16 == 2):
                        data_dr[seq, fix, exp] = data[seq+6, fix, exp]
                    if (seq%16 == 3):
                        data_dr[seq, fix, exp] = data[seq+9, fix, exp]
                    if (seq%16 == 4):
                        data_dr[seq, fix, exp] = data[seq-3, fix, exp]
                    if (seq%16 == 5):
                        data_dr[seq, fix, exp] = data[seq, fix, exp]
                    if (seq%16 == 6):
                        data_dr[seq, fix, exp] = data[seq+2, fix, exp]
                    if (seq%16 == 7):
                        data_dr[seq, fix, exp] = data[seq+5, fix, exp]
                    if (seq%16 == 8):
                        data_dr[seq, fix, exp] = data[seq-5, fix, exp]
                    if (seq%16 == 9):
                        data_dr[seq, fix, exp] = data[seq-2, fix, exp]
                    if (seq%16 == 10):
                        data_dr[seq, fix, exp] = data[seq, fix, exp]
                    if (seq%16 == 11):
                        data_dr[seq, fix, exp] = data[seq+3, fix, exp]
                    if (seq%16 == 12):
                        data_dr[seq, fix, exp] = data[seq-9, fix, exp]
                    if (seq%16 == 13):
                        data_dr[seq, fix, exp] = data[seq-6, fix, exp]
                    if (seq%16 == 14):
                        data_dr[seq, fix, exp] = data[seq-3, fix, exp]
                    if (seq%16 == 15):
                        data_dr[seq, fix, exp] = data[seq, fix, exp]
    return data_dr

# #%%
# data_dr0= ugly_dr3(0)
# data_dr1= ugly_dr3(1)
# data_dr2= ugly_dr3(2)
# data_dr3= ugly_dr3(3)
# print(data_dr0  [0:20,3,1])
# print(data_dr1  [0:20,3,1])
# print(data_dr2  [0:20,3,1])
# print(data_dr3  [0:20,3,1])

#%%
RR=6

data_rr0_dr = ugly_dr3(0)
data_rr1_dr = ugly_dr3(1)
data_rr2_dr = ugly_dr3(2)
data_rr3_dr = ugly_dr3(3)

# shrink data to rr = 1
# Merges for each rr all transmissions of the same sequence to 1 or 0
data_rr = np.zeros((RR,100,FIX_SIZE,EXP_SIZE),dtype=int)

for exp in range(0,EXP_SIZE):
    for fix in range(0,FIX_SIZE):
        for seq in range(0,int(SEQ_SIZE/6)):
            for rr in range(0,RR):

                # TODO Ugliest code ever!
                if (rr==0):
                    data_rr[rr,seq,fix,exp] = data_rr0_dr[seq,fix,exp]
                if (rr==1):
                    if (data_rr1_dr[2*seq, fix, exp] or data_rr1_dr[2*seq+1, fix, exp]):
                        data_rr[rr,seq,fix,exp] = 1
                if (rr==2):
                    if (data_rr2_dr[3*seq, fix, exp] or data_rr2_dr[3*seq+1, fix, exp] or data_rr2_dr[3*seq+2, fix, exp]):
                        data_rr[rr,seq,fix,exp] = 1
                if (rr==3):
                    if (data_rr3_dr[4*seq, fix, exp] or data_rr3_dr[4*seq+1, fix, exp] or data_rr3_dr[4*seq+2, fix, exp] or data_rr3_dr[4*seq+3, fix, exp]):
                        data_rr[rr,seq,fix,exp] = 1


sr_array_dr3 = np.zeros((RR, EXP_SIZE,FIX_SIZE))

# TODO skip last 20 for delayed repetition
for exp in range(0,EXP_SIZE):
    for fix in range(0,FIX_SIZE):
        for rr in range(0,4):
            count_is_zero = np.count_nonzero(data_rr[rr,:,fix,exp]==0)
            sr_array_dr3[rr,exp,fix] = 100 - count_is_zero

#%%----------------------------------------------------------------
# Plot it

def sr_per_node_bc_rr(rr):

    ax1.set_xlabel('Delayed Repetition', color='black')
    ax1.set_ylabel('Success Ratio in %') 
    

    boxarray2 = [sr_array[2,::10,3], sr_array_dr1[2,::10,3], sr_array_dr2[2,::10,3], sr_array_dr3[2,::10,3]]
    boxarray3 = [sr_array[3,::10,3], sr_array_dr1[3,::10,3], sr_array_dr2[3,::10,3], sr_array_dr3[3,::10,3]]
    # ax1.boxplot(boxarray2, flierprops=flierprop_purp, medianprops=medianprops_purp, positions=[0,1,2,3])
    ax1.boxplot(boxarray3, flierprops=flierprop_purp, medianprops=medianprops_purp, positions=[0,1,2,3])

    # ax1.boxplot(sr_array[0,::10,:], flierprops=flierprop_purp, medianprops=medianprops_purp)
    # ax1.boxplot(sr_array[rr,::10,:], flierprops=flierprop_purp, medianprops=medianprops_purp)

    # print(np.average(sr_array[rr,:,:], axis=0))
    # print(np.average(np.average(sr_array[rr,:,:], axis=0)))

    plt.figure(num=None, figsize=None, dpi=800)
    plt.show()
    
    fig.savefig('Graphs/SR_per_DR.pdf', bbox_inches='tight') 
    plt.close()

flierprop_purp = dict(marker='s', markerfacecolor='purple', markersize=3)
medianprops_purp = dict(linestyle='-', color='purple')

fig, ax1 = plt.subplots()
sr_per_node_bc_rr(0)
#%%
def plot():
    # data_a = [[1,2,5], [5,7,2,2,5], [7,2,5]]
    # data_b = [[6,4,2], [1,2,5,3,2], [2,3,5,1]]
    data_a = [sr_array[1,::10,3], sr_array_dr1[1,::10,3], sr_array_dr2[1,::10,3], sr_array_dr3[1,::10,3]]
    data_b = [sr_array[2,::10,3], sr_array_dr1[2,::10,3], sr_array_dr2[2,::10,3], sr_array_dr3[2,::10,3]]
    data_c = [sr_array[3,::10,3], sr_array_dr1[3,::10,3], sr_array_dr2[3,::10,3], sr_array_dr3[3,::10,3]]

    dr0 = [sr_array[1,::10,3], sr_array[2,::10,3], sr_array[4,::10,3]]
    dr1 = [sr_array_dr1[1,::10,3], sr_array_dr1[2,::10,3], sr_array_dr1[3,::10,3]]
    dr2 = [sr_array_dr2[1,::10,3], sr_array_dr2[2,::10,3], sr_array_dr2[3,::10,3]]
    dr3 = [sr_array_dr3[1,::10,3], sr_array_dr3[2,::10,3], sr_array_dr3[3,::10,3]]

    ticks = ['1', '2', '3']

    def set_box_color(bp, color):
        plt.setp(bp['boxes'], color=color)
        plt.setp(bp['whiskers'], color=color)
        plt.setp(bp['caps'], color=color)
        plt.setp(bp['medians'], color=color)

    bp0 = plt.boxplot(dr0, positions=np.array(range(len(dr0)))*2.0-0.6, widths=0.3, flierprops=flierprop_purp)
    bp1 = plt.boxplot(dr1, positions=np.array(range(len(dr1)))*2.0-0.2, widths=0.3, flierprops=flierprop_purp)
    bp2 = plt.boxplot(dr2, positions=np.array(range(len(dr2)))*2.0+0.2, widths=0.3, flierprops=flierprop_purp)
    bp3 = plt.boxplot(dr3, positions=np.array(range(len(dr3)))*2.0+0.6, widths=0.3, flierprops=flierprop_purp)
    set_box_color(bp3, 'purple')
    set_box_color(bp2, 'green')
    set_box_color(bp1, 'blue')
    set_box_color(bp0, 'red')

    ax1.set_xlabel('Rapid Repetition', color='black')
    ax1.set_ylabel('Success Ratio in %') 
    # draw temporary red and blue lines and use them to create a legend
    plt.plot([], c='purple',    label='DR=0')
    plt.plot([], c='blue', label='DR=1')
    plt.plot([], c='green', label='DR=2')
    plt.plot([], c='red', label='DR=3')
    plt.legend()

    plt.xticks(range(0, len(ticks) * 2, 2), ticks)

    plt.tight_layout()
    plt.figure(num=None, figsize=None, dpi=800)
    plt.show()
    
    fig.savefig('Graphs/SR_per_DR.pdf', bbox_inches='tight') 
    plt.close()

fig, ax1 = plt.subplots()
plot()