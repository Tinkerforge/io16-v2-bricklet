#!/bin/sh
# Connects to localhost:4223 by default, use --host and --port to change this

uid=XYZ # Change XYZ to the UID of your IO-16 Bricklet 2.0

# Handle incoming input_value callbacks
tinkerforge dispatch io16-v2-bricklet $uid input-value &

# Configure callback for channel 4 with fixed 500ms period
tinkerforge call io16-v2-bricklet $uid set-input-value-callback-configuration 4 500 false

echo "Press key to exit"; read dummy

kill -- -$$ # Stop callback dispatch in background
