#%%
import csv
from numpy import fix, genfromtxt
import numpy as np
import sys

np.set_printoptions(threshold=sys.maxsize)

#%%
measurment_data = genfromtxt('/home/walther/Documents/bachelor/Data/latency/broadcast/broadcast7NodesRR3 NewPlot.csv', dtype=int)

print(measurment_data.shape)
print(measurment_data[600:610])

#%%

bin_decoded_data = np.zeros((1398600*3), dtype=int)

for i in range(0,measurment_data.size):
    offset = i*2

    if measurment_data[i] == 1:
        bin_decoded_data[offset+i+0] = 1
    if measurment_data[i] == 2:
        bin_decoded_data[offset+i+1] = 1
    if measurment_data[i] == 3:
        bin_decoded_data[offset+i+2] = 1
    if measurment_data[i] == 4:
        bin_decoded_data[offset+i+0] = 1
        bin_decoded_data[offset+i+1] = 1
    if measurment_data[i] == 5:
        bin_decoded_data[offset+i+0] = 1
        bin_decoded_data[offset+i+2] = 1
    if measurment_data[i] == 6:
        bin_decoded_data[offset+i+1] = 1
        bin_decoded_data[offset+i+2] = 1
    if measurment_data[i] == 7:
        bin_decoded_data[offset+i+0] = 1
        bin_decoded_data[offset+i+1] = 1
        bin_decoded_data[offset+i+2] = 1
    
print(bin_decoded_data.shape)
print(bin_decoded_data[:200])
print(bin_decoded_data[-29:])

#%%
bin_decoded_data = np.zeros((1398600*3), dtype=int)

for i in range (0, measurment_data.size):
    offset = i*2
    
    bin_decoded_data[offset+i+0] = i
    bin_decoded_data[offset+i+1] = 0
    bin_decoded_data[offset+i+2] = 0

#%%

print(bin_decoded_data[:200])
print(bin_decoded_data[-29:])

#%%
formatted_data = np.zeros((int(bin_decoded_data.shape[0]/7), 7), dtype=int)

j = 0
for experiment in range(0,999):

    for fixture in range(0,7):

        for i in range(0,600):

            formatted_data[i+600*experiment][fixture] = bin_decoded_data[i+600*fixture+600*7*experiment]
            
print(formatted_data[:,:])
#%%
with open('removeme.csv', mode='w') as employee_file:
    employee_writer = csv.writer(employee_file, delimiter=',', quotechar='"', quoting=csv.QUOTE_MINIMAL)

    for i in range(0,formatted_data.shape[0]):
        employee_writer.writerow(formatted_data[i,:])