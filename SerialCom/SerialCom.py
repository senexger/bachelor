import time
import json
import serial
from pprint import pprint
import random

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
    test1 = {
        "VERBOSE"               : 1,
        "DEBUG"                 : 1,
        "TIMESTAMP"             : 1,
        "AIRTIME"               : 0,
        "FULL_REPETITIONS"      : 1,
        "DMX_BROADCASTING"      : 0,

        "CHANNEL_TOTAL"         : 100,
        "BROADCAST_FRAME_SIZE"  : 100,
        "UNICAST_FRAME_SIZE"    : 10,
        "UNICAST_SLAVE_COUNT"   : 1,
        "SEND_REPITITION"       : 100,  
        "WAIT_AFTER_SEND"       : 1000,
        "WAIT_AFTER_REP_SEND"   : 0
    }
 
    data=json.dumps(test1)
    print (data)

    if ser.isOpen():
        ser.write(data.encode('ascii'))
        ser.flush()
        while (True):
            try:
                incoming = ser.readline().decode("utf-8")
                print (incoming, end="")
            except Exception as e:
                print (e)
                pass
        ser.close()
    else:
        print ("opening error")