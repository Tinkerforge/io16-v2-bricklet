#!/usr/bin/env python
# -*- coding: utf-8 -*-

HOST = "localhost"
PORT = 4223
UID = "XYZ" # Change XYZ to the UID of your IO-16 Bricklet 2.0

import time

from tinkerforge.ip_connection import IPConnection
from tinkerforge.bricklet_io16_v2 import BrickletIO16V2

if __name__ == "__main__":
    ipcon = IPConnection() # Create IP connection
    io = BrickletIO16V2(UID, ipcon) # Create device object

    ipcon.connect(HOST, PORT) # Connect to brickd
    # Don't use device before ipcon is connected

    # Configure channel 7 as output low
    io.set_configuration(7, "o", False)

    # Set channel 7 alternating high/low 10 times with 100 ms delay
    for i in range(10):
        time.sleep(0.1)
        io.set_selected_value(7, True)
        time.sleep(0.1)
        io.set_selected_value(7, False)

    raw_input("Press key to exit\n") # Use input() in Python 3
    ipcon.disconnect()
