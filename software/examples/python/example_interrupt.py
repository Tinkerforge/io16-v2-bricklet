#!/usr/bin/env python
# -*- coding: utf-8 -*-

HOST = "localhost"
PORT = 4223
UID = "XYZ" # Change XYZ to the UID of your IO-16 Bricklet 2.0

from tinkerforge.ip_connection import IPConnection
from tinkerforge.bricklet_io16_v2 import BrickletIO16V2

# Callback function for input_value callback
def cb_input_value(channel, changed, value):
    print("Channel: " + str(channel))
    print("Changed: " + str(changed))
    print("Value: " + str(value))
    print("")

if __name__ == "__main__":
    ipcon = IPConnection() # Create IP connection
    io = BrickletIO16V2(UID, ipcon) # Create device object

    ipcon.connect(HOST, PORT) # Connect to brickd
    # Don't use device before ipcon is connected

    # Register input_value callback to function cb_input_value
    io.register_callback(io.CALLBACK_INPUT_VALUE, cb_input_value)

    # Configure callback for channel 4 with fixed 500ms period
    io.set_input_value_callback_configuration(4, 500, False)

    raw_input("Press key to exit\n") # Use input() in Python 3
    ipcon.disconnect()
