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
    data = {}
    # data["operation"] = "channel"
    data = {
        "VERBOSE"               : 17,
        "DEBUG"                 : 4,
        "TIMESTAMP"             : 0,
        "AIRTIME"               : 0,
        "FULL_REPETITIONS"      : 2,
        "DMX_BROADCASTING"      : 0,

        "CHANNEL_TOTAL"         : 10,
        "BROADCAST_FRAME_SIZE"  : 40,
        "UNICAST_FRAME_SIZE"    : 20,
        "SEND_REPITITION"       : 1,
        "WAIT_AFTER_SEND"       : 0,
        "WAIT_AFTER_REP_SEND"   : 1000
    }

    data=json.dumps(data)
    print (data)

    if ser.isOpen():
        ser.write(data.encode('ascii'))
        ser.flush()
        while (True):
            try:
                incoming = ser.readline().decode("utf-8")
                print (incoming)
            except Exception as e:
                print (e)
                pass
        ser.close()
    else:
        print ("opening error")

# TODO print every input to the terminal
    # while (str(espData) != "b'done\\r\\n'"):
    #     espData = ser.readline()
    #     print(espData)
