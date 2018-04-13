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

    print '======== INITAL CHANNEL CONFIG ========'
    for i in range(0, 16):
        print '[+] CH', i, 'CONFIG =', io.get_configuration(i)
    print '======== INITAL CHANNEL CONFIG ========\n'

    # Configure all the channels as input pull-up
    for i in range(0, 16):
        io.set_configuration(i, io.DIRECTION_IN, True)

    time.sleep(2)

    print '======== CHANNEL CONFIG (IN, PULL-UP) ========'
    for i in range(0, 16):
        print '[+] CH', i, 'CONFIG =', io.get_configuration(i)
    print '======== CHANNEL CONFIG (IN, PULL-UP) ========\n'

    print '======== CHANNEL VALUES ========'
    print '[+] CHANNEL VALUES =', io.get_value()
    print '======== CHANNEL VALUES ========\n'

    # Configure all the channels as input no pull-up
    for i in range(0,16):
        io.set_configuration(i, io.DIRECTION_IN, False)

    time.sleep(2)

    print '======== CHANNEL CONFIG (IN, DEFAULT) ========'
    for i in range(0, 16):
        print '[+] CH', i, 'CONFIG =', io.get_configuration(i)
    print '======== CHANNEL CONFIG (IN, DEFAULT) ========\n'

    print '======== CHANNEL VALUES ========'
    print '[+] CHANNEL VALUES =', io.get_value()
    print '======== CHANNEL VALUES ========\n'

    raw_input("Press key to exit\n") # Use input() in Python 3
    ipcon.disconnect()
