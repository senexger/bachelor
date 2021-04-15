# To add a new cell, type '# %%'
# To add a new markdown cell, type '# %% [markdown]'
# %%

import numpy as np
from matplotlib import pyplot as plt

SEND_REPETITION = 160

def successPercentage(counter):
    return 100/(SEND_REPETITION/(counter))

array = np.genfromtxt("/home/walther/Documents/bachelor/Data/broad_successratio/successratio_broadcast_F100_04-14-00:18:29.csv", delimiter=",", dtype="int")
array.shape = (2, 3, 160)

# %%
counter = [0,0,0,0,0]

for i in range(0,SEND_REPETITION):
    if (array[0,0,i]):
        counter[0] += 1
    if (array[0,0,i] and array[0,1,i]):
        counter[1] += 1
    if (array[0,0,i] and array[0,1,i]) and array[0,2,i]:
        counter[2] += 1
    # if (array[0,0,i] and array[0,1,i]) and array[0,2,i] and array[0,3,i]:
    #     counter[3] += 1
    # if (array[0,0,i] and array[0,1,i]) and array[0,2,i] and array[0,3,i] and array[0,4,i]:
    #     counter[4] += 1

counter[3] = counter[2]
counter[4] = counter[2]

successPercentageArray = [0,0,0,0,0]

for i in range(0,5):
    print("counter" + str(i) + " = " + str(counter[i]))
    print("success ratio 1: " + str(successPercentage(counter[i])))
    successPercentageArray[i] = successPercentage(counter[i])

print(successPercentageArray)

# %%



x = ["#1","#2","#3","#4","#5"]
plt.bar(x, counter, label='success ratio for each node')  # Plot some data on the (implicit) axes.
plt.xlabel('Current node')
plt.ylabel('Sucess ratio in %')
plt.title("Success ratio for each node")
plt.legend()

# %%
x = [0,1,2,3,4]
plt.bar(x, successPercentageArray, label='success ratio logic and accumulation')  # Plot some data on the (implicit) axes.
plt.xlabel('Current node')
plt.ylabel('Sucess ratio in %')
plt.title("Success ratio for X nodes")
plt.legend()