# To add a new cell, type '# %%'
# To add a new markdown cell, type '# %% [markdown]'

# %%
import numpy as np
import pandas as pd
from matplotlib import pyplot as plt
import matplotlib.ticker as tck

SEND_REPETITION = 100
TEST_REPETITION = 290
SLAVE_COUNT     = 4

array = np.genfromtxt("/home/walther/Documents/bachelor/Data/without9.csv", delimiter=",", dtype="int")
array = np.reshape(array, (TEST_REPETITION, SLAVE_COUNT, SEND_REPETITION))
print(array)

# SLAVE_COUNT     = 2
# SEND_REPETITION = 10
# TEST_REPETITION = 3

# array = np.genfromtxt("/home/walther/Documents/bachelor/Data/testFile.csv", delimiter=",", dtype="int")
# array.shape = (TEST_REPETITION, SLAVE_COUNT, SEND_REPETITION)



# %%
# success ratio for each node explecit
x=np.arange(SEND_REPETITION)
 
fig=plt.figure()
ax=fig.add_subplot(111)
 
ax.plot(x, array[0,0,:], linestyle='none', c='r',marker=".",label='Node 1')
ax.plot(x, array[0,1,:], linestyle='none', c='g',marker=".",label='Node 2')
ax.plot(x, array[0,2,:], linestyle='none', c='b',marker=".",label='Node 3')
ax.plot(x, array[0,3,:], linestyle='none', c='y',marker=".",label='Node 4')

plt.show()



# %%
# success ratio for each node explecit
def successPercentage(counter):
    return 100/(SEND_REPETITION/(counter))

sum_successPercentageArray = np.zeros(SLAVE_COUNT)

for i in range(0, SLAVE_COUNT):
    for j in range (0, SEND_REPETITION):
        sum_successPercentageArray[i] += successPercentage(np.sum(array[i,j,:]))

mean_successPercentageArray = np.divide(sum_successPercentageArray, SEND_REPETITION)
print(mean_successPercentageArray)

x = ["#1","#2","#3","#4"]
plt.plot(x, mean_successPercentageArray, marker=".", markersize=12, linestyle='none', label='success ratio for each node')

plt.minorticks_on()
plt.tick_params(axis='x', which='minor', bottom=False) # no x ticks
plt.xlabel('Current node')
plt.ylabel('Sucess ratio in %')
plt.title("Success ratio for each node - one example run")
plt.legend()
plt.grid()



# %% success ratio for each node confidence
mean = np.mean(array, axis=(0,2))
print(mean)
std = np.std(array, axis=(0,2))
print(std)

x = ["#1","#2","#3","#4"]
plt.errorbar(x, mean, yerr=std, fmt='-o', color='b', markersize=8, linestyle='none', capsize=7)
plt.minorticks_on()
plt.tick_params(axis='x', which='minor', bottom=False) # no x ticks
plt.ylabel('Sucess ratio in %')
plt.xlabel('Node id')
plt.legend()
plt.grid()
plt.show()



# %%
# success ratio, if packet has to arrive for all nodes
counter = [0,0,0,0]
successPercentageArray = np.zeros(SLAVE_COUNT)

for i in range(0,SEND_REPETITION):
    if (array[0,0,i]):
        counter[0] += 1
    if (array[0,0,i] and array[0,1,i]):
        counter[1] += 1
    if (array[0,0,i] and array[0,1,i]) and array[0,2,i]:
        counter[2] += 1
    if (array[0,0,i] and array[0,1,i]) and array[0,2,i] and array[0,3,i]:
        counter[3] += 1
    # if (array[0,0,i] and array[0,1,i]) and array[0,2,i] and array[0,3,i] and array[0,4,i]:
    #     counter[4] += 1

for i in range(0,SLAVE_COUNT):
    print("counter" + str(i) + " = " + str(counter[i]))
    print("success ratio 1: " + str(successPercentage(counter[i])))
    successPercentageArray[i] = successPercentage(counter[i])

x=np.arange(SLAVE_COUNT)
plt.minorticks_on()
plt.bar(x, successPercentageArray, label='success ratio logic and accumulation')
plt.xlabel('Current node')
plt.ylabel('Sucess ratio in %')
plt.title("Success ratio for X nodes")
plt.legend()

# %%
# success ratio with confidence interval
# TEST_REPETITION, SLAVE_COUNT, SEND_REPETITION))
print(array[0,0,:].size)

accomulation_sum = np.sum(array, axis=1)
print(accomulation_sum)

accomulation = np.zeros((SLAVE_COUNT, TEST_REPETITION))
super_mean = np.zeros(4)
super_std  = np.zeros(4)

for i in range(0,TEST_REPETITION):
    for s in range(0, SLAVE_COUNT):
        accomulation[s][i] = np.count_nonzero(accomulation_sum[i,:] >= s)

# mean and std for every missing_node_allowed
for s in range(0, SLAVE_COUNT):
    super_mean[s] = np.mean(accomulation[s,:])
    super_std[s]  = np.std(accomulation[s,:])

print(super_mean)

x = ["4", "3", "2", "1"]
plt.errorbar(x, super_mean, yerr=super_std, fmt='-o', color='b', markersize=8, linestyle='none', capsize=7)
# plt.plot(x, accomulation, marker=".", markersize=20, linestyle="none")
plt.minorticks_on()
plt.tick_params(axis='x', which='minor', bottom=False) # no x ticks
plt.ylabel('Sucess ratio in %')
plt.xlabel('Number of nodes allowed to miss')
plt.title("Sucessfull send message to all nodes exept X")
plt.legend()
plt.grid()
plt.show()


# %%
# Success Ratio, if there is one or more nodes allowed to fail
counter = np.zeros(SLAVE_COUNT)
successWithAllowdFailArray = [0,0,0,0]

sumArray = np.zeros((SEND_REPETITION))

for i in range(0,SLAVE_COUNT):
    sumArray += (array[0,i,:])

print(sumArray)

for i in range(SLAVE_COUNT, 0, -1):
    for j in range(0, SEND_REPETITION):
        if (sumArray[j] >= i):
            counter[i-1] += 1

print(counter)

for i in range(0,SLAVE_COUNT):
    print("success ratio: " + str(successPercentage(counter[i])))
    successWithAllowdFailArray[i] = successPercentage(counter[i])

print(successWithAllowdFailArray)

x = np.arange(SLAVE_COUNT)

plt.minorticks_on()
plt.plot(x, successWithAllowdFailArray, label='Nodes allowed to fail')
plt.xlabel('Number of nodes allowed to fail')
plt.ylabel('Sucess ratio in %')
plt.title("Success ratio for X nodes")
plt.legend()