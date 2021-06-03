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

    test1 = {
        "VERBOSE"               : 0,
        "DEBUG"                 : 0,
        "TIMESTAMP"             : 1,
        "AIRTIME"               : 0,
        "FULL_REPETITIONS"      : 1,
        "DMX_BROADCASTING"      : 0,

        "CHANNEL_TOTAL"         : 50,
        "BROADCAST_FRAME_SIZE"  : 50,
        "UNICAST_FRAME_SIZE"    : 50,
        "SLAVE_COUNT"           : 1,
        "RAPID_REPITITION"      : 1,
        "SEND_REPITITION"       : 40,
        "WAIT_AFTER_SEND"       : 1,
        "WAIT_AFTER_REP_SEND"   : 1000
    }


    PY_DEBUG = True

    for size in [10,50,200]:
        test1['BROADCAST_FRAME_SIZE'] = size
        test1['CHANNEL_TOTAL'] = size
        # for rapid in range (1,4):
        #     test1['RAPID_REPITITION'] = rapid
        for wait in range (0,5):
            test1['WAIT_AFTER_SEND'] = wait

            current_time = datetime.datetime.now().strftime("_%m-%d-%H:%M:%S")
            data=json.dumps(test1)
            print (data)

            bc = test1['DMX_BROADCASTING'    ]
            sz = test1['BROADCAST_FRAME_SIZE']
            # sz = test1['UNICAST_FRAME_SIZE'  ]
            sc = test1['SLAVE_COUNT'         ]
            rr = test1['RAPID_REPITITION'    ]
            rp = test1['SEND_REPITITION'     ]
            w8 = test1['WAIT_AFTER_SEND'     ]

            exp_name = f'far_uc{bc}_size{sz}_r{rp}_rr{rr}_wait{w8}'
            print(f'--->{exp_name}')
            # RUN EXPERIMENT
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
                                # break

                        with open(f'/home/walther/Documents/bachelor/Data/broad_successratio/{exp_name}{current_time}.csv', 'a', newline='') as file:
                            writer = csv.writer(file, delimiter=',')
                            writer.writerow([int(incoming)])

                    except Exception as e:
                        print (e)
                        pass

                    if ('===FINITO===' in incoming):
                        break

            else:
                print ("opening error")

            time.sleep(1)

    ser.close()
