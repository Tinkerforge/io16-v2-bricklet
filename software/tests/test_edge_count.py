#!/usr/bin/env python
# -*- coding: utf-8 -*-

HOST = "localhost"
PORT = 4223
UID = "XYZ" # Change XYZ to the UID of your IO-16 2.0 Bricklet

import time
from tinkerforge.ip_connection import IPConnection
from tinkerforge.bricklet_io16_v2 import BrickletIO16V2

if __name__ == "__main__":
    ipcon = IPConnection() # Create IP connection
    io = BrickletIO16V2(UID, ipcon) # Create device object

    ipcon.connect(HOST, PORT) # Connect to brickd
    # Don't use device before ipcon is connected

    # Configure all the channels as input no pull-up
    for i in range(0, 16):
        io.set_configuration(i, io.DIRECTION_IN, False)

    # Set edge count configuration
    for i in range(0, 16):
        io.set_edge_count_configuration(i, io.EDGE_TYPE_BOTH, 1)

    # Get edge count configuration
    for i in range(0, 16):
        print "[+] CH", i, "CONFIG =", io.get_edge_count_configuration(i)

    print ""

    time.sleep(8)

    # Report edge counts
    for i in range(0, 16):
        print "[+] CH", i, "EDGE COUNT =", io.get_edge_count(i, True)

    print ""

    raw_input("Press key to exit\n") # Use input() in Python 3
    ipcon.disconnect()
