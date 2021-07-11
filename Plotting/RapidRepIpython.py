# To add a new cell, type '# %%'
# To add a new markdown cell, type '# %% [markdown]'

# %%
import numpy as np
import pandas as pd
import matplotlib.ticker as tck
from matplotlib import pyplot as plt
from scipy import stats

SEND_REPETITION = 40

# array_2 = np.genfromtxt("", delimiter=",", dtype="int")
array1 = np.genfromtxt("/home/walther/Documents/bachelor/Data/broad_successratio/broadcast/far_bc1_size200_r40_rr1_wait0.csv", delimiter=",", dtype="int")
array1 = np.delete(array1, -1)
array1 = np.delete(array1, -1)
print(array1[1:])
array2= np.genfromtxt("/home/walther/Documents/bachelor/Data/broad_successratio/broadcast/far_bc1_size200_r40_rr2_wait0.csv", delimiter=",", dtype="int")
array2 = np.delete(array2, -1)
array2 = np.delete(array2, -1)
print(array2[1:])
array3= np.genfromtxt("/home/walther/Documents/bachelor/Data/broad_successratio/broadcast/far_bc1_size200_r40_rr1_wait0.csv", delimiter=",", dtype="int")
array3 = np.delete(array3, -1)
array3 = np.delete(array3, -1)
print(array3[1:]) 

def decodeBinary(array, rr):
    decodeArray = np.zeros(SEND_REPETITION)
    arr = array[1:]
    for i in range (0, SEND_REPETITION):
        n = 0
        for j in range (0,rr):
            n += arr[i] % 2
            arr[i] = arr[i] // 2
        decodeArray[i] = n
    return(decodeArray)

def extractTimeFromBinary(array):
    extractTimeArray = np.zeros(SEND_REPETITION)
    for i in range (0, SEND_REPETITION):
        extractTimeArray[i] = array[0]
    return extractTimeArray

def plotting(array):
    x=np.arange(SEND_REPETITION)

    ax1.set_xlabel('Nr. of repitition')
    ax1.set_ylabel('Successfull repititions', color='C0')
    ax1.tick_params(axis='y', labelcolor='C0')
    plt.xticks(np.arange(0, 41, 5))
    plt.yticks(np.arange(0, 4, 1.0))
    plt.autoscale(False)
    ax1.plot(x, decodeBinary(array,3), linestyle='none', marker='.', markersize=10, color='C0')

    ax2 = ax1.twinx()  # instantiate a second axes that shares the same x-axis
    
    ax2.set_ylabel('Transmission time in ms', color='C1')  # we already handled the x-label with ax1
    ax2.tick_params(axis='y', labelcolor='C1')
    plt.yticks(np.arange(100, 2000, 500.0))
    plt.autoscale(False)
    ax2.hlines(y=array[0]/1000, xmin=0, xmax= 40, linewidth=2, color='C1')

    fig.tight_layout()  # otherwise the right y-label is slightly clipped
    plt.show()
    # plt.close()
    
fig, ax1 = plt.subplots()

plotting(array1)
fig, ax1 = plt.subplots()
plotting(array2)
fig, ax1 = plt.subplots()
plotting(array3)

#%% Unicast vs Broadcast

array1 = np.genfromtxt("/home/walther/Documents/bachelor/Data/broad_successratio/broadcast/far_bc1_size200_r40_rr1_wait0.csv", delimiter=",", dtype="int")
array1 = np.delete(array1, -1)
array1 = np.delete(array1, -1)
print(array1[0])
array2= np.genfromtxt("/home/walther/Documents/bachelor/Data/broad_successratio/broadcast/far_bc1_size200_r40_rr2_wait0.csv", delimiter=",", dtype="int")
array2 = np.delete(array2, -1)
array2 = np.delete(array2, -1)
print(array2[0])
array3= np.genfromtxt("/home/walther/Documents/bachelor/Data/broad_successratio/broadcast/far_bc1_size200_r40_rr3_wait0.csv", delimiter=",", dtype="int")
array3 = np.delete(array3, -1)
array3 = np.delete(array3, -1)
print(array3[0]) 

u_array1 = np.genfromtxt("/home/walther/Documents/bachelor/Data/broad_successratio/unicast/far_uc0_size10_r40_rr1_wait0.csv", delimiter=",", dtype="int")
u_array1 = np.delete(u_array1, -1)
u_array1 = np.delete(u_array1, -1)
print(array1[0])
u_array2= np.genfromtxt("/home/walther/Documents/bachelor/Data/broad_successratio/unicast/far_uc0_size50_r40_rr1_wait0.csv", delimiter=",", dtype="int")
u_array2 = np.delete(u_array2, -1)
u_array2 = np.delete(u_array2, -1)
print(array2[0])
u_array3= np.genfromtxt("/home/walther/Documents/bachelor/Data/broad_successratio/unicast/far_uc0_size200_r40_rr1_wait0.csv", delimiter=",", dtype="int")
u_array3 = np.delete(u_array3, -1)
u_array3 = np.delete(u_array3, -1)
print(array3[0]) 


fig=plt.figure()
ax=fig.add_subplot(111)

bc_array = [u_array1[0]/1000, array1[0]/1000, array2[0]/1000, array3[0]/1000]
x = [0,1,2,3]

ax.plot(x, bc_array, linestyle='none', c='b',marker=".",label='transmission time', markersize=10)
# ax.plot(x, array3[0], linestyle='none', c='g',marker=".",label='Node 2')
# ax.plot(x, array[0,2,:], linestyle='none', c='b',marker=".",label='Node 3')
# ax.plot(x, array[0,3,:], linestyle='none', c='y',marker=".",label='Node 4')

plt.title("Broadcast Rapid Repitition")
plt.ylabel('transmission time in ms')
plt.xlabel('number of rapid repititions')
# plt.yticks([0,1], ["fail", "success"])
plt.xticks([0,1,2,3], ["unicast \n10 Byte", "rr 1\n200 Byte", "rr 2\n200 Byte", "rr 3\n200 Byte"])
fig.patch.set_facecolor('xkcd:white')
plt.savefig('/home/walther/Documents/bachelor/Graphs/100broadcastsOverTime.png', dpi=300)
plt.show()
#%%

fig=plt.figure()
ax=fig.add_subplot(111)

bc_array = [u_array1[0]/1000*20, array1[0]/1000, array2[0]/1000, array3[0]/1000]
x = [0,1,2,3]

ax.plot(x, bc_array, linestyle='none', c='b',marker=".",label='transmission time', markersize=10)
# ax.plot(x, array3[0], linestyle='none', c='g',marker=".",label='Node 2')
# ax.plot(x, array[0,2,:], linestyle='none', c='b',marker=".",label='Node 3')
# ax.plot(x, array[0,3,:], linestyle='none', c='y',marker=".",label='Node 4')

plt.title("Broadcast Rapid Repitition")
plt.ylabel('transmission time in ms')
plt.xlabel('number of rapid repititions')
# plt.yticks([0,1], ["fail", "success"])
plt.xticks([0,1,2,3], ["unicast \n20*10 Byte", "rr 1\n200 Byte", "rr 2\n200 Byte", "rr 3\n200 Byte"])
fig.patch.set_facecolor('xkcd:white')
plt.savefig('/home/walther/Documents/bachelor/Graphs/100broadcastsOverTime.png', dpi=300)
plt.show()