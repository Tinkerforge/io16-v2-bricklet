#!/bin/sh
# Connects to localhost:4223 by default, use --host and --port to change this

uid=XYZ # Change XYZ to the UID of your IO-16 Bricklet 2.0

# Configure channel 7 [A7] as output low
tinkerforge call io16-v2-bricklet $uid set-configuration 7 direction-out false

# Set channel 7 [A7] alternating high/low 10 times with 100 ms delay
for i in 0 1 2 3 4 5 6 7 8 9; do
	sleep 0.1
	tinkerforge call io16-v2-bricklet $uid set-selected-value 7 true
	sleep 0.1
	tinkerforge call io16-v2-bricklet $uid set-selected-value 7 false
done
