# %%
# import data
import sys
import numpy as np
import pandas as pd
import matplotlib.ticker as tck
from matplotlib import pyplot as plt
from scipy import stats

# np.set_printoptions(threshold=sys.maxsize)
np.set_printoptions(formatter={'float': lambda x: "{0:0.4f}".format(x)})

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
    # print(vector[:30])
    modulation = np.zeros((3,len(vector)//3), dtype=int)

    for i in range(0,len(vector)):
        offset = (i // (M*3)) * M
        x      = (i // M) % 3
        y      = (i % M)
    
        modulation[x][y+offset] = vector[i]
    # print('rr1=',modulation[0,:10])
    # print('rr2=',modulation[1,:10])
    # print('rr3=',modulation[2,:10])
    return modulation

def vector_success_ratio(vector):
    success = np.zeros(np.shape(vector)[1], dtype=int)

    for i in range(0, len(success)):
        if (vector[0:3,i].any()):
            success[i] = 1

    return (success)
        
def array_to_success_vector(array, node, modulation):
    array1D = array_To_Vector(array, node)
    # print('rx =', array1D[200:220])

    # skip first 200 for each node, because measurement was broken
    arrayDecoded = decode_array_to_vector(array1D[200:])
    # print('decode_array_to_vecor\n',arrayDecoded[401:422])

    vectorModulation = vector_modulation(arrayDecoded, modulation)
    # print('vector_modulation\n',vectorModulation[:,:20])

    return vector_success_ratio(vectorModulation)

######TESTING##################
node = 3
modulation = 3
# array, node, M
successVector = array_to_success_vector(arraySR[:,:,:], node, 4)#SEND_REPETITION*3)
print(successVector[:20])
print(successVector.size)
total = (np.size(successVector))
nonzero = (np.count_nonzero(successVector))

print (1/(total/nonzero))

#%%
# SUCCESS BAR FOR NODE
def plot_success_bar_for_node(array, node):
    std_errors = np.zeros(SLAVE_COUNT)
    means = np.zeros(SLAVE_COUNT)

    # 1-6
    for M in range(1,7):1
        std_errors[M-1] = np.std(array_to_success_vector(array, node, M), ddof=1) / np.sqrt(np.size(np.std(array_to_success_vector(array, node, M))) / 100)
        means[M-1]      = np.mean(array_to_success_vector(array, node, M))
    # Maximum Delay
    std_errors[6] = np.std(array_to_success_vector(array, node, SEND_REPETITION*3), ddof=1) / np.sqrt(np.size(np.std(array_to_success_vector(array, node, SEND_REPETITION*3))) / 100)
    means[6]      = np.mean(array_to_success_vector(array, node, SEND_REPETITION*3))
    
    print(std_errors)
    print(means*100)

    plt.subplots(figsize=(10, 7))
    x = ["1","2","3","4","5","6", "#Sequences"]
    plt.errorbar(x, means*100, yerr=std_errors, fmt='-o', color='b', markersize=8, linestyle='none', capsize=7)
    plt.axis((0,SLAVE_COUNT, 95, 105))
    plt.minorticks_on()
    plt.tick_params(axis='x', which='minor', bottom=False) # no x ticks
    plt.title('Success Ratio with increasing M\nUsing node #4')
    plt.ylabel('Success Ratio in %')
    plt.xlabel('M')
    plt.grid()
    # fig.patch.set_facecolor('xkcd:white')

    plt.savefig('/home/walther/Documents/bachelor/Graphs/SuccessRatioM.png', dpi=1000)
    plt.show()

    return

plot_success_bar_for_node(arraySR, 3)

#%%
# SUCCESS BAR FOR GOUPING

# input: arraySR
# output: success vector for each node
# TODO: Remove?!
# def success_vector_for_all_nodes(array):
#     decoded_vecors_by_node = np.zeros((SLAVE_COUNT, SEND_REPETITION*TEST_REPETITION*3-200*3), dtype=int)
#     for node in range(0,7):
#         array1d = array_To_Vector(array, node)
#         decoded_vecors_by_node[node] = decode_array_to_vector(array1d[200:])
#         total = (np.size(decoded_vecors_by_node[node]))
#         nonzero = (np.count_nonzero(decoded_vecors_by_node[node]))
#         print(nonzero/total)
#     return decoded_vecors_by_node

from itertools import combinations

def grouping_success(array, modulation):
    LEN = SEND_REPETITION*TEST_REPETITION-200
    print(LEN)
    success_vectors = np.zeros((SLAVE_COUNT, LEN), dtype=int)

    for node in range (0,SLAVE_COUNT):
        success_vectors[node] = array_to_success_vector(array, node, modulation)
    print('succes_vector[#x] =\n', success_vectors[3,200:400])
    print('shape             ='  , success_vectors.shape)
    
    std_error_group = np.zeros(SLAVE_COUNT)
    mean_group = np.zeros(SLAVE_COUNT)
    
    for groupsize in range(1,8):
        combination = (list(combinations(np.arange(0,7,1), groupsize)))
    
        # print('combination =', combination)
    
        possible_combinations = len(combination)
        # print('possible combinations =', possible_combinations)
    
        success_ratios_for_all_combinations = np.zeros(possible_combinations)
    
        for i in range(0,possible_combinations):
            tmpArray = np.zeros(LEN, dtype=int)
    
            for j in combination[i]:
                tmpArray = tmpArray + success_vectors[j]
    
            success_ratios_for_all_combinations[i] = np.count_nonzero(tmpArray == groupsize)/ LEN
            
        # print("")
        mean_group[groupsize - 1]      = np.mean(success_ratios_for_all_combinations)
        std_error_group[groupsize - 1] = np.std(success_ratios_for_all_combinations)
        # print(mean_group[groupsize - 1])
        # print(std_error_group[groupsize - 1])

    print('mean_group:     \n', mean_group)
    print('std_error_group:\n', std_error_group)
    return mean_group, std_error_group

def grouping_plot(array):
    # mean_group, std_error_group = grouping_success(arraySR, 1)
    # plt.errorbar(range(1,8), mean_group*100, yerr=std_error_group*100, fmt='-o', color='b', markersize=8, linestyle='none', capsize=7)
    # mean_group, std_error_group = grouping_success(arraySR, 2)
    # plt.errorbar(range(1,8), mean_group*100, yerr=std_error_group*100, fmt='-o', color='r', markersize=8, linestyle='none', capsize=7)
    # mean_group, std_error_group = grouping_success(arraySR, 5)
    # plt.errorbar(range(1,8), mean_group*100, yerr=std_error_group*100, fmt='-o', color='g', markersize=8, linestyle='none', capsize=7)
    # mean_group, std_error_group = grouping_success(arraySR, 10)
    # plt.errorbar(range(1,8), mean_group*100, yerr=std_error_group*100, fmt='-o', color='y', markersize=8, linestyle='none', capsize=7)

    fig.tight_layout()  # otherwise the right y-label is slightly clipped
    fig.set_size_inches(7, 5)
    plt.xticks(np.arange(1, 8, step=1))
    plt.yticks(np.arange(90, 110, 10.0))
    plt.minorticks_on()
    plt.title('Success ratio for grouping\n Using RR=3 over M')
    plt.ylabel('Success Ratio in %')
    plt.xlabel('Groupsize')
    plt.grid()
    plt.savefig('/home/walther/Documents/bachelor/Graphs/groupingOverM.jpg', dpi=2000)
    plt.show()

fig, ax1 = plt.subplots()
grouping_plot(arraySR)

#%%
def diff_vector(array, node, modulation):
    array1D = array_To_Vector(array, node)
    print('rx            =', array1D[200:218])

    # skip first 200, because measurement was broken
    arrayDecoded = decode_array_to_vector(array1D[200:])
    print('decoded_array =',arrayDecoded[:18])

    vectorModulation = vector_modulation(arrayDecoded, modulation)
    print('modulation =',modulation)
    print('vector_modulation\n',vectorModulation[:,:10])

    LEN = (SEND_REPETITION*TEST_REPETITION)-200
    delayVector = np.zeros(LEN, dtype=int)
    diff_vector = np.zeros(LEN, dtype=int)

    for i in range(0,len(vectorModulation[1,:])):
        isSuccess = 1
        offset = (i // modulation) * 3 * modulation
        modWidth = (i % 3) + 1

        if (vectorModulation[0][i]):
            repetition = 0
        elif (vectorModulation[1][i]):
            repetition = 1
        elif (vectorModulation[2][i]):
            repetition = 2
        else:
            isSuccess = 0

        delayVector[i] = (modWidth + 3*repetition + offset) * isSuccess

        if (delayVector[i] == 0):
            diff_vector[i] = 0
        else:
            diff_vector[i] = delayVector[i] - delayVector[i-1]

    # sanitize vector
    for i in range(0,len(diff_vector)):
        if (diff_vector[i] > 300):
            diff_vector[i] = 0

    # print(vectorModulation[1,200:600])
    # print(delayVector[:200])
    # print(diff_vector[:200])
    return(diff_vector)

node = 3
# diff_vector = diff_vector(arraySR, 4, 3)

diff_vector1 = diff_vector(arraySR, node, 1)
diff_vector2 = diff_vector(arraySR, node, 3)
diff_vector3 = diff_vector(arraySR, node, 5)
diff_vector4 = diff_vector(arraySR, node, 6)
diff_vector_max = diff_vector(arraySR, node, SEND_REPETITION)

diff_array = [diff_vector1, diff_vector2, diff_vector3, diff_vector4, diff_vector_max]

#%%
def diff_plot(diff_array):
    fig.set_size_inches(7, 4)

    plt.boxplot(diff_array)

    plt.title('Success ratio for grouping\n RR = 3, M = 1\n')

    plt.minorticks_on()
    plt.grid()
    # plt.xticks([1,2,3,4,5], ['1','2','3','4''#Seq'])
    plt.ylabel('Delay between nodes')
    plt.xlabel('Modulation M')
    plt.savefig('/home/walther/Documents/bachelor/Graphs/delay.png', dpi=2000)
    plt.show()

fig, ax1 = plt.subplots()
diff_plot(diff_array)