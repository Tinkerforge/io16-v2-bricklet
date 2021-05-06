#!/bin/sh
# Connects to localhost:4223 by default, use --host and --port to change this

uid=XYZ # Change XYZ to the UID of your IO-16 Bricklet 2.0

# Get current value
tinkerforge call io16-v2-bricklet $uid get-value
