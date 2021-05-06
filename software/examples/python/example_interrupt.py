#!/usr/bin/env python
# -*- coding: utf-8 -*-

HOST = "localhost"
PORT = 4223
UID = "XYZ" # Change XYZ to the UID of your IO-16 Bricklet 2.0

from tinkerforge.ip_connection import IPConnection
from tinkerforge.bricklet_io16_v2 import BrickletIO16V2

# Callback function for input value callback
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

    # Register input value callback to function cb_input_value
    io.register_callback(io.CALLBACK_INPUT_VALUE, cb_input_value)

    # Set period for input value (channel 4 [A4]) callback to 0.5s (500ms)
    io.set_input_value_callback_configuration(4, 500, False)

    input("Press key to exit\n") # Use raw_input() in Python 2
    ipcon.disconnect()
