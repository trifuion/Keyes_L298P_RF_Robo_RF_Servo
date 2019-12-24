import serial
ser = serial.Serial('/dev/ttyACM0', 9600)

while 1:
    arduino_serial=str(ser.readline())
    arduino_serial=arduino_serial.lstrip("b")
    arduino_serial=arduino_serial.strip("'")
    arduino_serial=arduino_serial.rstrip("\\r\\n")
    a=int(arduino_serial)
    print(arduino_serial)