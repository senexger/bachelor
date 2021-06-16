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
array1 = np.genfromtxt("/home/walther/Documents/bachelor/Data/broad_successratio/broadcast/far_bc1_size10_r40_rr3_wait4.csv", delimiter=",", dtype="int")
array1 = np.delete(array1, -1)
array1 = np.delete(array1, -1)
print(array1[1:])
array2= np.genfromtxt("/home/walther/Documents/bachelor/Data/broad_successratio/broadcast/far_bc1_size10_r40_rr2_wait4.csv", delimiter=",", dtype="int")
array2 = np.delete(array2, -1)
array2 = np.delete(array2, -1)
print(array2[1:])
array3= np.genfromtxt("/home/walther/Documents/bachelor/Data/broad_successratio/broadcast/far_bc1_size10_r40_rr1_wait4.csv", delimiter=",", dtype="int")
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
    
# fig, ax1 = plt.subplots()

plotting2(array1, array2, array3)

#%%
# sphinx_gallery_thumbnail_number = 11

# Some example data to display
def plotting2(array1, array2, array3):
    fig, (ax1a, ax2a, ax3a) = plt.subplots(1, 3)
    fig.suptitle('Sharing x per column, y per row')

    ax1a.set_xlabel('Nr. of repitition')
    ax1a.set_ylabel('Successfull repititions', color='C0')
    ax1a.tick_params(axis='y', labelcolor='C0')
    plt.xticks(np.arange(0, 41, 5))
    plt.yticks(np.arange(0, 4, 1.0))
    plt.autoscale(False)
    ax1a.plot(x, decodeBinary(array1,3), linestyle='none', marker='.', markersize=10, color='C0')

    ax1b = ax1a.twinx()  # instantiate a second axes that shares the same x-axis
    ax1b.set_ylabel('Transmission time in ms', color='C1')  # we already handled the x-label with ax1
    ax1b.tick_params(axis='y', labelcolor='C1')
    plt.yticks(np.arange(100, 2000, 500.0))
    plt.autoscale(False)
    ax1b.hlines(y=array1[0]/1000, xmin=0, xmax= 40, linewidth=2, color='C1')

    ax2a.set_xlabel('Nr. of repitition')
    ax2a.set_ylabel('Successfull repititions', color='C0')
    ax2a.tick_params(axis='y', labelcolor='C0')
    plt.xticks(np.arange(0, 41, 5))
    plt.yticks(np.arange(0, 4, 1.0))
    plt.autoscale(False)
    ax2a.plot(x, decodeBinary(array1,3), linestyle='none', marker='.', markersize=10, color='C0')

    ax2b = ax2a.twinx()  # instantiate a second axes that shares the same x-axis
    ax2b.set_ylabel('Transmission time in ms', color='C1')  # we already handled the x-label with ax1
    ax2b.tick_params(axis='y', labelcolor='C1')
    plt.yticks(np.arange(100, 2000, 500.0))
    plt.autoscale(False)
    ax2b.hlines(y=array1[0]/1000, xmin=0, xmax= 40, linewidth=2, color='C1')

    ax3a.set_xlabel('Nr. of repitition')
    ax3a.set_ylabel('Successfull repititions', color='C0')
    ax3a.tick_params(axis='y', labelcolor='C0')
    plt.xticks(np.arange(0, 41, 5))
    plt.yticks(np.arange(0, 4, 1.0))
    plt.autoscale(False)
    ax3a.plot(x, decodeBinary(array3,3), linestyle='none', marker='.', markersize=10, color='C0')

    ax3b = ax3a.twinx()  # instantiate a second axes that shares the same x-axis
    ax3b.set_ylabel('Transmission time in ms', color='C1')  # we already handled the x-label with ax1
    ax3b.tick_params(axis='y', labelcolor='C1')
    plt.yticks(np.arange(100, 2000, 500.0))
    plt.autoscale(False)
    ax3b.hlines(y=array3[0]/1000, xmin=0, xmax= 40, linewidth=2, color='C1')

    for ax in fig.get_axes():
        ax.label_outer()

# %%
fig = plt.figure()

x = [0,1,2]
y = [0,1,2]
# total_rows, total_columns, subplot_index(1st, 2nd, etc..)
plt.subplot(2, 3, 1)
plt.plot(x, y)

plt.subplot(2, 3, 2)
plt.plot(x, y)

plt.subplot(2, 3, 3)
plt.plot(x, y)

plt.subplot(2, 3, 6)
plt.plot(x, y)

# %%
import sys
import timeit

import matplotlib
from matplotlib.backends.backend_pdf import PdfPages
import matplotlib.pyplot as plt
import numpy as np


matplotlib.rcParams.update({"font.size": 6})

# Dimensions for any m-rows × n-cols array of subplots / pg.
m, n = 4, 5

# Don't forget to indent after the with statement
with PdfPages("auto_subplotting.pdf") as pdf:

    # Let's time the execution required to create and save
    # each full page of subplots to the pdf
    start_time = timeit.default_timer()

    # Before beginning the iteration through all the data,
    # initialize the layout for the plots and create a
    # representation of the subplots that can be easily
    # iterated over for knowing when to create the next page
    # (and also for custom settings like partial axes labels)
    f, axarr = plt.subplots(m, n, sharex="col", sharey="row")
    arr_ij = [(x, y) for x, y in np.ndindex(axarr.shape)]
    subplots = [axarr[index] for index in arr_ij]

    # To conserve needed plotting real estate,
    # only label the bottom row and leftmost subplots
    # as determined automatically using m and n
    splot_index = 0
    for s, splot in enumerate(subplots):
        splot.set_ylim(0, 0.15)
        splot.set_xlim(0, 50)
        last_row = m * n - s < n + 1
        first_in_row = s % n == 0
        if last_row:
            splot.set_xlabel("X-axis label")
        if first_in_row:
            splot.set_ylabel("Y-axis label")

    # Iterate through each sample in the data
    for sample in range(33):

        # As a stand-in for real data, let's just make numpy take 100 random draws
        # from a poisson distribution centered around say ~25 and then display
        # the outcome as a histogram
        scaled_y = np.random.randint(20, 30)
        random_data = np.random.poisson(scaled_y, 100)
        subplots[splot_index].hist(
            random_data,
            bins=12,
            normed=True,
            fc=(0, 0, 0, 0),
            lw=0.75,
            ec="b",
        )

        # Keep subplotting through the samples in the data and increment
        # a counter each time. The page will be full once the count is equal
        # to the product of the user-set dimensions (i.e. n * m)
        splot_index += 1

        # We can basically repeat the same exact code block used for the
        # first layout initialization, but with the addition of 4 new lines:
        # 2 for saving the just-finished page to the pdf, 1 for the
        # page's execution time, & 1 more to reset the subplot index
        if splot_index == m * n:
            pdf.savefig()
            plt.close(f)
            print(timeit.default_timer() - start_time)
            start_time = timeit.default_timer()
            f, axarr = plt.subplots(m, n, sharex="col", sharey="row")
            arr_ij = [(x, y) for x, y in np.ndindex(axarr.shape)]
            subplots = [axarr[index] for index in arr_ij]
            splot_index = 0
            for s, splot in enumerate(subplots):
                splot.set_ylim(0, 0.15)
                splot.set_xlim(0, 50)
                last_row = (m * n) - s < n + 1
                first_in_row = s % n == 0
                if last_row:
                    splot.set_xlabel("X-axis label")
                if first_in_row:
                   splot.set_ylabel("Y-axis label")

    # Done!
    # But don't forget the last page    pdf.savefig()
    plt.close(f)