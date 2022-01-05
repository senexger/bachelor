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
TEST_REPETITION  = 1000
SEND_REPETITION  = 200
RAPID_REPETITION = 2
SLAVE_COUNT      = 7

arraySR = np.genfromtxt("/home/walther/Documents/bachelor/Data/latency/broadcast/broadcast7" \
                        "NodesRR3.csv", delimiter=",", dtype="int")
arraySR = np.reshape(arraySR, (TEST_REPETITION, SLAVE_COUNT, SEND_REPETITION))

#%%
# 3 dim array to vector per node
def array_to_vector(array, selected_node):
    """Creates a vector of encoded transmissions from an vector containing all nodes (called array)
    
    Keyword arguments:
    array         -- the measured values
    selected_node -- the node encoded in the array
    repetitions   -- the number of rapid-repetitions (redundant information/retransmissions)
    """
    array_to_vector = np.zeros(SEND_REPETITION*TEST_REPETITION, dtype=int)
    for i in range(0, TEST_REPETITION):
        for j in range(0, SEND_REPETITION):
            array_to_vector[j+i*SEND_REPETITION] = array[i,selected_node,j]
    return array_to_vector

def decode_array_to_vector(vector):
    """Decode a encoded vector to a bool for each transmission"""
    vector_decoded = np.zeros(len(vector)*3, dtype=int)
    # TODO Das ist so hässlich, das könnte glatt ein modernes Kunstwerk sein!!!!111!1!!11
    for i in range(0, len(vector)*3, 3):
        if vector[i//3] == 0:
            vector_decoded[i]   = 0
            vector_decoded[i+1] = 0
            vector_decoded[i+2] = 0
        elif vector[i//3] == 1:
            vector_decoded[i]   = 1
            vector_decoded[i+1] = 0
            vector_decoded[i+2] = 0
        elif vector[i//3] == 2:
            vector_decoded[i]   = 0
            vector_decoded[i+1] = 1
            vector_decoded[i+2] = 0
        elif vector[i//3] == 3:
            vector_decoded[i]   = 1
            vector_decoded[i+1] = 1
            vector_decoded[i+2] = 0
        elif vector[i//3] == 4:
            vector_decoded[i]   = 0
            vector_decoded[i+1] = 0
            vector_decoded[i+2] = 1
        elif vector[i//3] == 5:
            vector_decoded[i]   = 1
            vector_decoded[i+1] = 0
            vector_decoded[i+2] = 1
        elif vector[i//3] == 6:
            vector_decoded[i]   = 0
            vector_decoded[i+1] = 1
            vector_decoded[i+2] = 1
        elif vector[i//3] == 7:
            vector_decoded[i]   = 1
            vector_decoded[i+1] = 1
            vector_decoded[i+2] = 1
        else:
            print("ERROR")
    return vector_decoded

def vector_modulation(vector, M, repetitions):
    """Calculates from the incomming vector for each sequence of repetitions, 
    if at least one of the repetitions arrived correctly.
    
    Keyword arguments:
    vector        -- decoded vector with zeros for one specific node
    M             -- Delay for the delayed repetition
    repetitions   -- the number of rapid-repetition
    """
    print(vector[:30])
    modulation = np.zeros((repetitions,len(vector)//repetitions), dtype=int)

    #? TODO is the RAPID-REPETITION NEEDED?
    for i in range(0,len(vector)-RAPID_REPETITION):
        offset = (i // (M*repetitions)) * M
        x      = (i //  M) % repetitions
        y      = (i %   M)
    
        modulation[x][y+offset] = vector[i]

    for i in range(0, repetitions):
        print('rr' + str(i) + '= ', modulation[i,:10])

    return modulation

def vector_success_ratio(vector):
    """Create a success vector given all success vectors of all repetitions"""
    success = np.zeros(np.shape(vector)[1], dtype=int)

    for i in range(0, len(success)):
        if (vector[0:RAPID_REPETITION,i].any()):
            success[i] = 1

    return success
        
def array_to_success_vector(array, node, modulation, repetitions):
    """Kochbuch-Funktion"""
    array1D = array_to_vector(array, node)
    # skip first 200
    print('rx =', array1D[200:220])

    # skip first 200 for each node, because measurement was broken
    arrayDecoded = decode_array_to_vector(array1D[200:])
    # print('decode_array_to_vecor\n',arrayDecoded[:20])

    vectorModulation = vector_modulation(arrayDecoded, modulation, repetitions)
    # print('vector_modulation\n',vectorModulation[:,:20])

    return vector_success_ratio(vectorModulation)

######TESTING##################
selectedNode = 3
modulation   = 6
# TODO just temp
RAPID_REPETITION = 3

# array, node, M
successVector = array_to_success_vector(arraySR[:,:,:], selectedNode, modulation, RAPID_REPETITION)#SEND_REPETITION*3)
print(successVector[:20])
print(successVector.size)

nonZero_100_parts = np.zeros(51)

for i in range(0,50):
    nonZero_100_parts[i] = 1/(200/np.count_nonzero(successVector[i*200:(i+1)*200]))
    # total = (np.size(successVector))
    # nonzero = (np.count_nonzero(successVector[0,200]))

print(nonZero_100_parts)
print(np.mean(nonZero_100_parts))
print(np.sqrt(np.std(nonZero_100_parts)))
# print (1/(200/nonzero))





#%% NOTFALLPLOT!!! ALARM!!!111

def notfall_plot():
    """Next try"""

    std_errors_notfall = [0.9441176470588235,0.956764705882353,0.9566666666666667,0.9555882352941176,0.9581372549019608]
    # means_notfall      = [0.3683008780654771,0.369783911444378,0.3699188003853914,0.3693752096142936,0.3697603382284395]

    plt.subplots(figsize=(10, 7))
    x = ["No Delay","1","2","3","4"]
    # plt.errorbar(x, means_notfall*100, yerr=std_errors_notfall, fmt='-o', color='b', markersize=8, linestyle='none', capsize=7)
    # plt.errorbar(x, means2*100, yerr=std_errors2/6, fmt='-o', color='r', markersize=8, linestyle='none', capsize=7)
    # plt.boxplot(std_errors_notfall)
    plt.plot(x, std_errors_notfall)
    

    # plt.axis((0,SLAVE_COUNT, 95, 105))
    plt.minorticks_on()
    plt.tick_params(axis='x', which='minor', bottom=False) # no x ticks
    plt.title('Success Ratio for Delayed Repetion (DR)\nNode #4')
    plt.ylabel('Success Ratio in %')
    plt.xlabel('DR')
    plt.xlim((-1, 7))
    plt.ylim(95,100)
    plt.grid()
    # fig.patch.set_facecolor('xkcd:white')

    plt.savefig('/home/walther/Documents/bachelor/Graphs/SuccessRatioM.png', dpi=1000)
    plt.show()

    return

notfall_plot()


#%%

def plot_success_bar_for_node2(array, node):
    """Next try"""
    std_errors = np.zeros(SLAVE_COUNT)
    means = np.zeros(SLAVE_COUNT)
    std_errors2 = np.zeros(SLAVE_COUNT)
    means2 = np.zeros(SLAVE_COUNT)

    # 1-6
    RAPID_REPETITION = 2
    for M in range(1,7):
        std_errors[M-1] = np.std(array_to_success_vector(array, node, M, RAPID_REPETITION), ddof=1) / np.sqrt(np.size(np.std(array_to_success_vector(array, node, M, RAPID_REPETITION))) / 100)
        means[M-1]      = np.mean(array_to_success_vector(array, node, M, RAPID_REPETITION))
    
    # RAPID_REPETITION = 3
    # for M in range(1,7):
    #     std_errors2[M-1] = np.std(array_to_success_vector(array, node, M, RAPID_REPETITION), ddof=1) / np.sqrt(np.size(np.std(array_to_success_vector(array, node, M, RAPID_REPETITION))) / 100)
    #     means2[M-1]      = np.mean(array_to_success_vector(array, node, M, RAPID_REPETITION))
    
    # Maximum Delay
    # std_errors[6] = np.std(array_to_success_vector(array, node, SEND_REPETITION*RAPID_REPETITION, RAPID_REPETITION), ddof=1) / np.sqrt(np.size(np.std(array_to_success_vector(array, node, SEND_REPETITION*RAPID_REPETITION, RAPID_REPETITION))) / 100)
    # means[6]      = np.mean(array_to_success_vector(array, node, SEND_REPETITION*RAPID_REPETITION, RAPID_REPETITION))

    print(std_errors)
    print(means*100)

    plt.subplots(figsize=(10, 7))
    x = ["No Delay","1","2","3","4","5","6"]
    plt.errorbar(x, means*100, yerr=std_errors/6, fmt='-o', color='b', markersize=8, linestyle='none', capsize=7)
    # plt.errorbar(x, means2*100, yerr=std_errors2/6, fmt='-o', color='r', markersize=8, linestyle='none', capsize=7)
    # plt.boxplot(means*100)

    plt.axis((0,SLAVE_COUNT, 95, 105))
    plt.minorticks_on()
    plt.tick_params(axis='x', which='minor', bottom=False) # no x ticks
    plt.title('Success Ratio for Delayed Repetion (DR)\nNode #4')
    plt.ylabel('Success Ratio in %')
    plt.xlabel('DR')
    plt.xlim((-1, 7))
    plt.ylim(95,100)
    plt.grid()
    # fig.patch.set_facecolor('xkcd:white')

    plt.savefig('/home/walther/Documents/bachelor/Graphs/SuccessRatioM.png', dpi=1000)
    plt.show()

    return

plot_success_bar_for_node2(arraySR, 3)

#%%
# SUCCESS BAR FOR NODE
def plot_success_bar_for_node(array, node):
    std_errors = np.zeros(SLAVE_COUNT)
    means = np.zeros(SLAVE_COUNT)

    # 1-6
    for M in range(1,7):
        std_errors[M-1] = np.std(array_to_success_vector(array, node, M), ddof=1) / np.sqrt(np.size(np.std(array_to_success_vector(array, node, M))) / 100)
        means[M-1]      = np.mean(array_to_success_vector(array, node, M))
    # Maximum Delay
    std_errors[6] = np.std(array_to_success_vector(array, node, SEND_REPETITION*3), ddof=1) / np.sqrt(np.size(np.std(array_to_success_vector(array, node, SEND_REPETITION*3))) / 100)
    means[6]      = np.mean(array_to_success_vector(array, node, SEND_REPETITION*3))
    
    print(std_errors)
    print(means*100)

    plt.subplots(figsize=(10, 7))
    x = ["No Delay","1","2","3","4","5", "200"]
    plt.errorbar(x, means*100, yerr=std_errors/6, fmt='-o', color='b', markersize=8, linestyle='none', capsize=7)
    # plt.boxplot(means*100)

    plt.axis((0,SLAVE_COUNT, 95, 105))
    plt.minorticks_on()
    plt.tick_params(axis='x', which='minor', bottom=False) # no x ticks
    plt.title('Success Ratio for Delayed Repetion (DR)\nNode #4')
    plt.ylabel('Success Ratio in %')
    plt.xlabel('DR')
    plt.xlim((-1, 7))
    plt.ylim(95,100)
    plt.grid()
    # fig.patch.set_facecolor('xkcd:white')

    plt.savefig('/home/walther/Documents/bachelor/Graphs/SuccessRatioM.png', dpi=1000)
    plt.show()

    return

plot_success_bar_for_node(arraySR, 3)

#%%
# SUCCESS BAR FOR GOUPING

from itertools import combinations

def grouping_success(array, modulation):
    LEN = SEND_REPETITION*TEST_REPETITION-200
    success_vectors = np.zeros((SLAVE_COUNT, LEN), dtype=int)

    for node in range (0,SLAVE_COUNT):
        success_vectors[node] = array_to_success_vector(array, node, modulation)
    # print('succes_vector[#x] =\n', success_vectors[3,200:400])
    # print('shape             ='  , success_vectors.shape)
    
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
            
        mean_group[groupsize - 1]      = np.mean(success_ratios_for_all_combinations)
        std_error_group[groupsize - 1] = np.std(success_ratios_for_all_combinations)
        # print(mean_group[groupsize - 1])
        # print(std_error_group[groupsize - 1])

    print('mean_group:     \n', mean_group)
    print('std_error_group:\n', std_error_group)
    return mean_group, std_error_group

def grouping_plot(array):
    # ERRORPLOT OR PLOT
    mean_group, std_error_group = grouping_success(arraySR, 1)
    plt.errorbar(range(1,8), mean_group*100, yerr=std_error_group*100, fmt='-o', color='b', markersize=8, linestyle='none', capsize=7)
    mean_group, std_error_group = grouping_success(arraySR, 2)
    plt.errorbar(range(1,8), mean_group*100, yerr=std_error_group*100, fmt='-o', color='r', markersize=8, linestyle='none', capsize=7)
    mean_group, std_error_group = grouping_success(arraySR, 10)
    plt.errorbar(range(1,8), mean_group*100, yerr=std_error_group*100, fmt='-o', color='g', markersize=8, linestyle='none', capsize=7)

    # mean_group, std_error_group = grouping_success(arraySR, 1)
    # plt.plot(range(1,8), mean_group*100, color='b', markersize=8, marker='.')
    # mean_group, std_error_group = grouping_success(arraySR, 2)
    # plt.plot(range(1,8), mean_group*100, color='r', markersize=8, marker='.')
    # mean_group, std_error_group = grouping_success(arraySR, 5)
    # plt.plot(range(1,8), mean_group*100, color='g', markersize=8, marker='.')
    # mean_group, std_error_group = grouping_success(arraySR, 10)
    # plt.plot(range(1,8), mean_group*100, color='y', markersize=8, marker='.')

    fig.tight_layout()  # otherwise the right y-label is slightly clipped
    fig.set_size_inches(7, 5)
    ax1.legend(['M=0', 'M=1', 'M=10'])
    plt.minorticks_on()
    plt.ylim((95,100))
    plt.xticks(np.arange(1, 8, step=1))
    plt.yticks(np.arange(95, 101, 1.0))
    plt.title('Success ratio Delayed Repetition for grouping\n Repetitions = 2')
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
    # print('rx            =', array1D[200:218])

    # skip first 200, because measurement was broken
    arrayDecoded = decode_array_to_vector(array1D[200:])
    # print('decoded_array =',arrayDecoded[:18])

    vectorModulation = vector_modulation(arrayDecoded, modulation)
    # print('vector_modulation (%i)\n' % modulation,vectorModulation[:,:10])

    LEN = (SEND_REPETITION*TEST_REPETITION)-200
    delayVector = np.zeros(LEN, dtype=int)
    diff_vector = np.zeros(LEN, dtype=int)

    for i in range(0,len(vectorModulation[1,:8])):
        isSuccess = 1
        offset = (i // modulation) * modulation * 3
        modWidth = (i % modulation) + 1
        # print('offset=', offset)

        if (vectorModulation[0][i]):
            repetition = 0
        elif (vectorModulation[1][i]):
            repetition = 1
        elif (vectorModulation[2][i]):
            repetition = 2
        else:
            isSuccess = 0

        delayVector[i] = (modWidth + modulation*repetition + offset) * isSuccess

        if (delayVector[i] == 0):
            diff_vector[i] = 0
        else:
            diff_vector[i] = delayVector[i] - delayVector[i-1]

    # sanitize vector
    for i in range(0,len(diff_vector)):
        if (diff_vector[i] > 300):
            diff_vector[i] = 0

    # print('rr1   =',vectorModulation[0,:8])
    # print('rr2   =',vectorModulation[1,:8])
    # print('rr3   =',vectorModulation[2,:8])
    # print('diff  =', delayVector[:8])
    # print('delay =', diff_vector[:8])
    return(diff_vector)

selectedNode = 3

diff_vector1 = diff_vector(arraySR, selectedNode, 1)
diff_vector2 = diff_vector(arraySR, selectedNode, 2)
diff_vector3 = diff_vector(arraySR, selectedNode, 3)
diff_vector4 = diff_vector(arraySR, selectedNode, 4)
diff_vector5 = diff_vector(arraySR, selectedNode, 5)
# diff_vector_max = diff_vector(arraySR, node, SEND_REPETITION)

diff_array = [diff_vector1, diff_vector2, diff_vector3, diff_vector4, diff_vector5]
print('dly_vctr(M=1)', diff_vector1[:18])

#%%
def diff_plot(diff_array):
    fig.set_size_inches(10, 6)

    plt.boxplot(diff_array)

    plt.title('Delay in Transmissions\n RR = 3, M = 1')

    plt.minorticks_on()
    plt.grid()
    # plt.xticks([0,1,2,3,4], ['1','2','3','4''#Seq'])
    plt.ylabel('Delay between nodes')
    plt.xlabel('Modulation M')
    plt.savefig('/home/walther/Documents/bachelor/Graphs/delay.png', dpi=2000)
    plt.show()

fig, ax1 = plt.subplots()
diff_plot(diff_array)


#%%
def simple_delayed_repetition_buffering(delayedRepetition, repetitions):
    result = [ (dr+1) * repetitions for dr in delayedRepetition]
    print(result)
    return result

def delay_plot_bc_overModulation():
    fig.set_size_inches(10, 6)

    x = [0,1,2,3,4,5]
    plt.plot(x, simple_delayed_repetition_buffering(x, 0), color='b', markersize=8, marker='.')
    plt.plot(x, simple_delayed_repetition_buffering(x, 1), color='r', markersize=8, marker='.')
    plt.plot(x, simple_delayed_repetition_buffering(x, 2), color='g', markersize=8, marker='.')

    plt.title('Buffering Delay in for Delayed Repetition')

    ax1.legend(['Repetitions=0', 'Repetitions=1', 'Repetitions=2'])
    # plt.xticks(np.arange(1, 8, step=1))
    # plt.yticks(np.arange(0, 25, 5))
    plt.minorticks_on()

    plt.grid()
    plt.ylabel('Buffering Delay')
    plt.xlabel('Delayed Repetitions DR')
    plt.savefig('/home/walther/Documents/bachelor/Graphs/delayOverModulationBCDeterministic.png', dpi=1000)
    plt.show()

fig, ax1 = plt.subplots()
delay_plot_bc_overModulation()