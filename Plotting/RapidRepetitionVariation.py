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

#%%
# 3 dim array to vector per node
def array_To_Vector(array, node):
    arrayToVector = np.zeros(SEND_REPETITION*TEST_REPETITION, dtype=int)
    for i in range(0, TEST_REPETITION):
        for j in range(0, SEND_REPETITION):
            arrayToVector[j+i*SEND_REPETITION] = array[i,node,j]
    return arrayToVector

def decode_array_to_vector(vector):
    vectorDecoded = np.zeros(len(vector)*3, dtype=int)
    for i in range(0, len(vector)*3, 3):
        if vector[i//3] == 0:
            vectorDecoded[i]   = 0
            vectorDecoded[i+1] = 0
            vectorDecoded[i+2] = 0
        elif vector[i//3] == 1:
            vectorDecoded[i]   = 1
            vectorDecoded[i+1] = 0
            vectorDecoded[i+2] = 0
        elif vector[i//3] == 2:
            vectorDecoded[i]   = 0
            vectorDecoded[i+1] = 1
            vectorDecoded[i+2] = 0
        elif vector[i//3] == 3:
            vectorDecoded[i]   = 1
            vectorDecoded[i+1] = 1
            vectorDecoded[i+2] = 0
        elif vector[i//3] == 4:
            vectorDecoded[i]   = 0
            vectorDecoded[i+1] = 0
            vectorDecoded[i+2] = 1
        elif vector[i//3] == 5:
            vectorDecoded[i]   = 1
            vectorDecoded[i+1] = 0
            vectorDecoded[i+2] = 1
        elif vector[i//3] == 6:
            vectorDecoded[i]   = 0
            vectorDecoded[i+1] = 1
            vectorDecoded[i+2] = 1
        elif vector[i//3] == 7:
            vectorDecoded[i]   = 1
            vectorDecoded[i+1] = 1
            vectorDecoded[i+2] = 1
        else:
            print("ERROR")
    return vectorDecoded

def vector_modulation(vector, M):
    modulation = np.zeros((3,len(vector)//3), dtype=int)

    for i in range(0,len(vector)):
        offset = (i // (M*3)) * M
        x      = (i // M) % 3
        y      = (i % M)
    
        modulation[x][y+offset] = vector[i]
    # print('rr1=',modulation[0,:])
    # print('rr2=',modulation[1,:])
    # print('rr3=',modulation[2,:])
    return modulation

def vector_success_ratio(vector):
    success = np.zeros(np.shape(vector)[1], dtype=int)

    for i in range(0, len(success)):
        if (vector[0:3,i].any()):
            success[i] = 1

    return (success)
        
def array_to_success_vector(array, node, modulation):
    array1D = array_To_Vector(array, node)
    # print('array_To_Vector\n', array1D[401:422])

    # skip first 200, because measurement was broken
    arrayDecoded = decode_array_to_vector(array1D[200:])
    # print('decode_array_to_vecor\n',arrayDecoded)

    vectorModulation = vector_modulation(arrayDecoded, modulation)
    # print('vector_modulation\n',vectorModulation)

    return vector_success_ratio(vectorModulation)

################################
for node in [4]:
    # array, node, M
    successVector = array_to_success_vector(arraySR[:,:,:], node, SEND_REPETITION*3)
    # print(successVector)
    total = (np.size(successVector))
    nonzero = (np.count_nonzero(successVector))

    print (1/(total/nonzero))
# %%
def plot_success_bar_for_node(array, node):
    std_errors = np.zeros(SLAVE_COUNT)
    means = np.zeros(SLAVE_COUNT)

    for M in range(1,7):
        std_errors[M-1] = np.std(array_to_success_vector(array, node, M), ddof=1) / np.sqrt(np.size(np.std(array_to_success_vector(array, node, M))) / 100)
        means[M-1]      = np.mean(array_to_success_vector(array, node, M))
    std_errors[6] = np.std(array_to_success_vector(array, node, SEND_REPETITION*3), ddof=1) / np.sqrt(np.size(np.std(array_to_success_vector(array, node, SEND_REPETITION*3))) / 100)
    means[6]      = np.mean(array_to_success_vector(array, node, SEND_REPETITION*3))
    
    print(std_errors)
    print(means*100)

    plt.subplots(figsize=(10, 7))
    x = ["#1","#2","#3","#4","#5","#6", "N"]
    plt.errorbar(x, means*100, yerr=std_errors, fmt='-o', color='b', markersize=8, linestyle='none', capsize=7)
    plt.axis((0,SLAVE_COUNT, 95, 105))
    plt.minorticks_on()
    plt.tick_params(axis='x', which='minor', bottom=False) # no x ticks
    plt.title('Success Ratio with increasing M')
    plt.ylabel('Success Ratio in %')
    plt.xlabel('M')
    plt.grid()
    # fig.patch.set_facecolor('xkcd:white')

    plt.savefig('/home/walther/Documents/bachelor/Graphs/SuccessRatioM.png', dpi=1000)
    plt.show()

    return

plot_success_bar_for_node(arraySR, 4)