package main

import (
	"fmt"
	"github.com/Tinkerforge/go-api-bindings/io16_v2_bricklet"
	"github.com/Tinkerforge/go-api-bindings/ipconnection"
	"time"
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

	// Configure channel 7 as output low
	io.SetConfiguration(7, 'o', false)

	// Set channel 7 alternating high/low 10 times with 100 ms delay
	for i := 0; i < 10; i++ {
		time.Sleep(100 * time.Millisecond)
		io.SetSelectedValue(7, true)
		time.Sleep(100 * time.Millisecond)
		io.SetSelectedValue(7, false)
	}

	fmt.Print("Press enter to exit.")
	fmt.Scanln()
}
