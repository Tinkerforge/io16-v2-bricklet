#!/usr/bin/env python
# -*- coding: utf-8 -*-

HOST = "localhost"
PORT = 4223
UID = "XYZ" # Change XYZ to the UID of your IO-16 Bricklet 2.0

from tinkerforge.ip_connection import IPConnection
from tinkerforge.bricklet_io16_v2 import BrickletIO16V2

if __name__ == "__main__":
    ipcon = IPConnection() # Create IP connection
    io = BrickletIO16V2(UID, ipcon) # Create device object

    ipcon.connect(HOST, PORT) # Connect to brickd
    # Don't use device before ipcon is connected

    # Get current value
    value = io.get_value()

    print("Channel 0 [A0]: " + str(value[0]))
    print("Channel 1 [A1]: " + str(value[1]))
    print("Channel 2 [A2]: " + str(value[2]))
    print("Channel 3 [A3]: " + str(value[3]))
    print("Channel 4 [A4]: " + str(value[4]))
    print("Channel 5 [A5]: " + str(value[5]))
    print("Channel 6 [A6]: " + str(value[6]))
    print("Channel 7 [A7]: " + str(value[7]))
    print("Channel 8 [B0]: " + str(value[8]))
    print("Channel 9 [B1]: " + str(value[9]))
    print("Channel 10 [B2]: " + str(value[10]))
    print("Channel 11 [B3]: " + str(value[11]))
    print("Channel 12 [B4]: " + str(value[12]))
    print("Channel 13 [B5]: " + str(value[13]))
    print("Channel 14 [B6]: " + str(value[14]))
    print("Channel 15 [B7]: " + str(value[15]))

    input("Press key to exit\n") # Use raw_input() in Python 2
    ipcon.disconnect()
