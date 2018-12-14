package main

import (
	"fmt"
	"tinkerforge/io16_v2_bricklet"
	"tinkerforge/ipconnection"
)

const ADDR string = "localhost:4223"
const UID string = "XYZ" // Change XYZ to the UID of your IO-16 Bricklet 2.0.

func main() {
	ipcon := ipconnection.New()
	defer ipcon.Close()
	io, _ := io16_v2_bricklet.New(UID, &ipcon) // Create device object.

	ipcon.Connect(ADDR) // Connect to brickd.
	defer ipcon.Disconnect()
	// Don't use device before ipcon is connected.

	io.RegisterInputValueCallback(func(channel uint8, changed bool, value bool) {
		fmt.Printf("Channel: %d\n", channel)
		fmt.Printf("Changed: %s\n", changed)
		fmt.Printf("Value: %s\n", value)
		fmt.Println()
	})

	// Set period for input value (channel 4) callback to 0.5s (500ms).
	io.SetInputValueCallbackConfiguration(4, 500, false)

	fmt.Print("Press enter to exit.")
	fmt.Scanln()

}
