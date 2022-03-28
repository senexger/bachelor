# To add a new cell, type '# %%'
# To add a new markdown cell, type '# %% [markdown]'

# %%
import numpy as np
import pandas as pd
import matplotlib.ticker as tck
from matplotlib import pyplot as plt
from scipy import stats

SEND_REPETITION   = 100
TEST_REPETITION   = 290
TEST_REPETITION_2 = 57
SLAVE_COUNT       = 4

array = np.genfromtxt("/home/walther/Documents/bachelor/Data/without9.csv", delimiter=",", dtype="int")
array_1 = np.genfromtxt("/home/walther/Documents/bachelor/Data/without9 copy.csv", delimiter=",", dtype="int")
array_2 = np.genfromtxt("/home/walther/Documents/bachelor/Data/without9_now.csv", delimiter=",", dtype="int")
array_8_nodes = np.concatenate((array_1, array_2), 0)
array = np.reshape(array, (TEST_REPETITION, SLAVE_COUNT, SEND_REPETITION))
array_1 = np.reshape(array_1, (TEST_REPETITION_2, SLAVE_COUNT, SEND_REPETITION))
array_2 = np.reshape(array_2, (TEST_REPETITION_2, SLAVE_COUNT, SEND_REPETITION))
array_8_nodes = np.reshape(array_8_nodes, (TEST_REPETITION_2, SLAVE_COUNT*2, SEND_REPETITION))

# print(array_1)
# print("==========")
# print(array_8_nodes)

SEND_REPETITION   = 100
TEST_REPETITION   = 100
SLAVE_COUNT       = 6

array1 = np.genfromtxt("/home/walther/Documents/bachelor/Data/without9_successratio_broadcast_F100_Node6_05-28-04:55:54.csv", delimiter=",", dtype="int")
array2 = np.genfromtxt("/home/walther/Documents/bachelor/Data/without9_successratio_broadcast_F100_Node6_05-28-07:59:53.csv", delimiter=",", dtype="int")
array1 = np.reshape(array1, (TEST_REPETITION, SLAVE_COUNT, SEND_REPETITION))
array2 = np.reshape(array2, (TEST_REPETITION, SLAVE_COUNT, SEND_REPETITION))

# print(array)

# %%
# success ratio for each node explecit
x=np.arange(SEND_REPETITION)
 
fig=plt.figure()
ax=fig.add_subplot(111)

ax.plot(x, array1[0,1,:], linestyle='none', c='b',marker=".",label='Node 1', markersize=10)
# ax.plot(x, array2[0,1,:], linestyle='none', c='r',marker=".",label='Node 1', markersize=10)
# ax.plot(x, array[0,1,:], linestyle='none', c='g',marker=".",label='Node 2')
# ax.plot(x, array[0,2,:], linestyle='none', c='b',marker=".",label='Node 3')
# ax.plot(x, array[0,3,:], linestyle='none', c='y',marker=".",label='Node 4')

plt.title("100 broadcasts over time from Master to one Slave")
plt.ylabel('broadcast transmission')
plt.xlabel('iteration of broadcasts')
plt.yticks([0,1], ["fail", "success"])
fig.patch.set_facecolor('xkcd:white')
# plt.savefig('/home/walther/Documents/bachelor/Graphs/100broadcastsOverTime.png', dpi=300)
plt.show()


# %% success ratio for each node confidence


# std_error = stats.sem(array1, axis=(0,2))
# print(std_error)
std_error = np.std(array1, axis=(0,2), ddof=1) / np.sqrt(np.size(array1) / 100)
print(std_error)
mean = np.mean(array1, axis=(0,2))
print(mean)

plt.subplots(figsize=(10, 7))
x = ["#1","#2","#3","#4","#5","#6"]
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

# %%

# %%
# success ratio with confidence interval
# TEST_REPETITION, SLAVE_COUNT, SEND_REPETITION))

accomulation_sum = np.sum(array, axis=1)

print(accomulation_sum.shape)
print(TEST_REPETITION)

accomulation = np.zeros((SLAVE_COUNT, TEST_REPETITION))
super_mean = np.zeros(SLAVE_COUNT)
super_std  = np.zeros(SLAVE_COUNT)

for i in range(0,TEST_REPETITION):
    for s in range(0, SLAVE_COUNT):
        accomulation[s][i] = np.count_nonzero(accomulation_sum[i,:] >= s+1)

# mean and std for every missing_node_allowed
for s in range(0, SLAVE_COUNT):
    super_mean[s] = np.mean(accomulation[s,:])
    super_std[s]  = np.std(accomulation[s,:])

print(accomulation_sum)

plt.subplots(figsize=(10, 7))
x = ["5", "4", "3", "2", "1", "0"]
plt.errorbar(x, super_mean, yerr=super_std, fmt='-o', color='b', markersize=8, linestyle='none', capsize=7)
plt.axis((0,SLAVE_COUNT, 0, 105))
# axes.set_ylim([ymin,ymax])
plt.minorticks_on()
plt.tick_params(axis='x', which='minor', bottom=False) # no x ticks
plt.title("Messages successfully send to all nodes exept X")
plt.ylabel('sucess ratio in %')
plt.xlabel('number of nodes allowed to miss')
plt.grid()
fig.patch.set_facecolor('xkcd:white')
# plt.savefig('/home/walther/Documents/bachelor/Graphs/successfullX.png', dpi=300)
plt.show()
# %%
# Grouping Broadcast
# TEST_REPETITION, SLAVE_COUNT, SEND_REPETITION))

accomulation_sum = np.sum(array, axis=1)

print(accomulation_sum.shape)
print(TEST_REPETITION)

accomulation = np.zeros((SLAVE_COUNT, TEST_REPETITION))
super_mean = np.zeros(SLAVE_COUNT)
super_std  = np.zeros(SLAVE_COUNT)

for i in range(0,TEST_REPETITION):
    for s in range(0, SLAVE_COUNT):
        accomulation[s][i] = np.count_nonzero(accomulation_sum[i,:] >= s+1)

# mean and std for every missing_node_allowed
for s in range(0, SLAVE_COUNT):
    super_mean[s] = np.mean(accomulation[s,:])
    super_std[s]  = np.std(accomulation[s,:])

print(accomulation_sum)

plt.subplots(figsize=(10, 8))
x = ["5", "4", "3", "2", "1", "0"]
plt.errorbar(x, super_mean, yerr=super_std, fmt='-o', color='b', markersize=8, linestyle='none', capsize=7)
plt.axis((0,SLAVE_COUNT, 0, 105))
# axes.set_ylim([ymin,ymax])
plt.minorticks_on()
plt.tick_params(axis='x', which='minor', bottom=False) # no x ticks
plt.title("Messages successfully send to all nodes exept X")
plt.ylabel('sucess ratio in %')
plt.xlabel('number of nodes allowed to miss')
plt.grid()
fig.patch.set_facecolor('xkcd:white')
# plt.savefig('/home/walther/Documents/bachelor/Graphs/successfullX.png', dpi=300)
plt.show()

accomulation_sum = np.sum(array, axis=1)