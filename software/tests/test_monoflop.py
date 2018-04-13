#!/usr/bin/env python
# -*- coding: utf-8 -*-

HOST = "localhost"
PORT = 4223
UID = "XYZ" # Change XYZ to the UID of your IO-16 2.0 Bricklet

import time
from tinkerforge.ip_connection import IPConnection
from tinkerforge.bricklet_io16_v2 import BrickletIO16V2

def cb_monoflop_done(channel, value):
    print "[+] MONOFLOP DONE CB: CHANNEL =", channel, "VALUE =", value

if __name__ == "__main__":
    ipcon = IPConnection() # Create IP connection
    io = BrickletIO16V2(UID, ipcon) # Create device object

    ipcon.connect(HOST, PORT) # Connect to brickd
    # Don't use device before ipcon is connected

    # Configure all the channels as output low
    for i in range(0, 16):
        io.set_configuration(i, io.DIRECTION_OUT, False)

    # Set monoflop configuration
    for i in range(0, 16):
        io.set_monoflop(i, True, 8000)

    time.sleep(2)

    # Get monoflop configuration
    for i in range(0, 16):
        print "[+] CHANNEL =", i ,"CONFIG =", io.get_monoflop(i)

    print ""

    # Register callback
    io.register_callback(io.CALLBACK_MONOFLOP_DONE, cb_monoflop_done)

    raw_input("Press key to exit\n\n") # Use input() in Python 3
    ipcon.disconnect()
