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
        "VERBOSE" : 1,
        "DEBUG" : 1,
        "TIMESTAMP" : 1,
        "AIRTIME" : 0,
        "FULL_REPETITIONS" : 10
    }

    data=json.dumps(data)
    print (data)
    if ser.isOpen():
        ser.write(data.encode('ascii'))
        ser.flush()
        try:
            incoming = ser.readline().decode("utf-8")
            print (incoming)
        except Exception as e:
            print (e)
            pass
        ser.close()
    else:
        print ("opening error")


# import serial

# # init empty string for serial
# espData = ''

# def main():
#     global espData

#     # Setup serial
#     ser = serial.Serial(
#         port='/dev/ttyUSB0',\
#         baudrate=115200,\
#         parity=serial.PARITY_NONE,\
#         stopbits=serial.STOPBITS_ONE,\
#         bytesize=serial.EIGHTBITS,\
#             timeout=1)
#     print("connected to: " + ser.portstr)

#     ser.write(b'a')                         # start the program 'a'

#     # waiting until program is terminated
#     while (str(espData) != "b'done\\r\\n'"):
#         espData = ser.readline()
#         print(espData)

#     # stop timer
#     print("Testrun ended!")

# if __name__ == "__main__":
#     main()
