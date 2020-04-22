import serial
import timeit
import time

# init empty string for serial
espData = ''

def getSerial ():
    global espData
    espData = str(ser.readline())
    print(str(espData))

def main():
    global espData

    # Setup serial
    ser = serial.Serial(
        port='/dev/ttyUSB0',\
        baudrate=115200,\
        parity=serial.PARITY_NONE,\
        stopbits=serial.STOPBITS_ONE,\
        bytesize=serial.EIGHTBITS,\
            timeout=1)
    print("connected to: " + ser.portstr)

    ser.write(b'a')                         # start the program 'a'
    timestampA = timeit.default_timer()     # star timestamp

    # waiting until program is terminated
    while (str(espData) != "b'done\\r\\n'"):
        espData = ser.readline()
        print(espData)

    # stop timer
    timestampB = timeit.default_timer()
    print('Duration: ', timestampB - timestampA)  

if __name__ == "__main__":
    main()