import time
import json
import serial
from pprint import pprint
import random
import csv
import datetime

if __name__ == "__main__":
    print ("Ready...")
    ser = serial.Serial(
        port='/dev/ttyUSB0',\
        baudrate=115200,\
        parity=serial.PARITY_NONE,\
        stopbits=serial.STOPBITS_ONE,\
        bytesize=serial.EIGHTBITS,\
        timeout=2.5
    )
    # data = {}
    # data["operation"] = "channel"

    exp_name = "successratio_broadcast_F100_Node6"
    test1 = {
        "VERBOSE"               : 1,
        "DEBUG"                 : 1,
        "TIMESTAMP"             : 0,
        "AIRTIME"               : 0,
        "FULL_REPETITIONS"      : 1,
        "DMX_BROADCASTING"      : 1,

        "CHANNEL_TOTAL"         : 100,
        "BROADCAST_FRAME_SIZE"  : 100,
        "UNICAST_FRAME_SIZE"    : 100,
        "SLAVE_COUNT"           : 6,
        "SEND_REPITITION"       : 100,  
        "WAIT_AFTER_SEND"       : 100,
        "WAIT_AFTER_REP_SEND"   : 100
    }
    # exp_name = "successratio_broadcast_F201"
    # test1 = {
    #     "VERBOSE"               : 1,
    #     "DEBUG"                 : 1,
    #     "TIMESTAMP"             : 0,
    #     "AIRTIME"               : 0,
    #     "FULL_REPETITIONS"      : 1,
    #     "DMX_BROADCASTING"      : 1,

    #     "CHANNEL_TOTAL"         : 200,
    #     "BROADCAST_FRAME_SIZE"  : 200,
    #     "UNICAST_FRAME_SIZE"    : 10,
    #     "UNICAST_SLAVE_COUNT"   : 4,
    #     # "SLAVE_COUNT"           : 4,
    #     "SEND_REPITITION"       : 100,  
    #     "WAIT_AFTER_SEND"       : 100,
    #     "WAIT_AFTER_REP_SEND"   : 1000
    # }

    PY_DEBUG = True

    current_time = datetime.datetime.now().strftime("_%m-%d-%H:%M:%S")
    data=json.dumps(test1)
    print (data)

    for i in range (0,200):
        print("Durchgang Nummer: " + str(i))

        if ser.isOpen():
            ser.write(data.encode('ascii'))
            ser.flush()

            currentSlave = 0

            while (True):
                
                try:
                    incoming = ser.readline().decode("utf-8")
                    print (incoming, end=" ")
                    if ('DONE' in incoming):
                        currentSlave += 1
                        if (currentSlave == test1['SLAVE_COUNT']):
                            with open(f'/home/walther/Documents/bachelor/Data/broad_successratio/{exp_name}{current_time}.csv', 'a', newline='') as file:
                                writer = csv.writer(file, delimiter=',')
                                writer.writerow([int(9999)])
                            break
                    if ('Entering Python Bridge' in incoming):
                        break

                    with open(f'/home/walther/Documents/bachelor/Data/broad_successratio/{exp_name}{current_time}.csv', 'a', newline='') as file:
                        writer = csv.writer(file, delimiter=',')
                        writer.writerow([int(incoming)])
                        
                except Exception as e:
                    print (e)
                    pass


        else:
            print ("opening error")
    
    ser.close()

    