# To add a new cell, type '# %%'
# To add a new markdown cell, type '# %% [markdown]'

# %%
import numpy as np
import pandas as pd
from matplotlib import pyplot as plt
import matplotlib.ticker as tck

SEND_REPETITION   = 100
TEST_REPETITION   = 290
TEST_REPETITION_2 = 57
SLAVE_COUNT       = 4

array = np.genfromtxt("/home/walther/Documents/bachelor/Data/without9.csv", delimiter=",", dtype="int")
array_2 = np.genfromtxt("/home/walther/Documents/bachelor/Data/without9_now.csv", delimiter=",", dtype="int")
array = np.reshape(array, (TEST_REPETITION, SLAVE_COUNT, SEND_REPETITION))
array_2 = np.reshape(array_2, (TEST_REPETITION_2, SLAVE_COUNT, SEND_REPETITION))

SLAVE_COUNT = 8


# %%
# success ratio for each node explecit
x=np.arange(SEND_REPETITION)
 
fig=plt.figure()
ax=fig.add_subplot(111)
 
ax.plot(x, array[0,0,:], linestyle='none', c='b',marker=".",label='Node 1', markersize=10)
# ax.plot(x, array[0,1,:], linestyle='none', c='g',marker=".",label='Node 2')
# ax.plot(x, array[0,2,:], linestyle='none', c='b',marker=".",label='Node 3')
# ax.plot(x, array[0,3,:], linestyle='none', c='y',marker=".",label='Node 4')

plt.title("100 broadcasts over time from Master to one Slave")
plt.ylabel('broadcast transmission')
plt.xlabel('iteration of broadcasts')
plt.yticks([0,1], ["fail", "success"])
fig.patch.set_facecolor('xkcd:white')
plt.savefig('/home/walther/Documents/bachelor/Graphs/100broadcastsOverTime.png', dpi=300)
plt.show()




# %% success ratio for each node confidence
mean = np.mean(array, axis=(0,2))
mean_2 = np.mean(array_2, axis=(0,2))
mean_total = np.concatenate((mean,mean_2),0)
print(mean_total)

std = np.std(array, axis=(0,2))
std_2 = np.std(array_2, axis=(0,2))
std_total = np.concatenate((std,std_2),0)
print(std_total)


# x = ["#1","#2","#3","#4"]
x = ["#1","#2","#3","#4","#5","#6","#7","#8"]
plt.errorbar(x, mean_total*100, yerr=std_total*100, fmt='-o', color='b', markersize=8, linestyle='none', capsize=7)
plt.minorticks_on()
plt.tick_params(axis='x', which='minor', bottom=False) # no x ticks
plt.title('Sucess ratio for each node')
plt.ylabel('success ratio in %')
plt.xlabel('node id')
plt.grid()
fig.patch.set_facecolor('xkcd:white')
plt.savefig('/home/walther/Documents/bachelor/Graphs/successRatioPerNode.png', dpi=300)
plt.show()




# %%
# success ratio with confidence interval
# TEST_REPETITION, SLAVE_COUNT, SEND_REPETITION))

SLAVE_COUNT = 4

accomulation_sum = np.sum(array, axis=1)

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

print(super_mean)

x = ["3", "2", "1", "0"]
plt.errorbar(x, super_mean, yerr=super_std, fmt='-o', color='b', markersize=8, linestyle='none', capsize=7)
# plt.plot(x, accomulation, marker=".", markersize=20, linestyle="none")
plt.minorticks_on()
plt.tick_params(axis='x', which='minor', bottom=False) # no x ticks
plt.title("Messages successfully send to all nodes exept X")
plt.ylabel('sucess ratio in %')
plt.xlabel('number of nodes allowed to miss')
plt.grid()
fig.patch.set_facecolor('xkcd:white')
plt.savefig('/home/walther/Documents/bachelor/Graphs/successfullX.png', dpi=300)
plt.show()