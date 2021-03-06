package main

import (
	"fmt"
	"github.com/Tinkerforge/go-api-bindings/io16_v2_bricklet"
	"github.com/Tinkerforge/go-api-bindings/ipconnection"
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

	// Get current value.
	value, _ := io.GetValue()

	fmt.Printf("Channel 0 [A0]: \n", value[0])
	fmt.Printf("Channel 1 [A1]: \n", value[1])
	fmt.Printf("Channel 2 [A2]: \n", value[2])
	fmt.Printf("Channel 3 [A3]: \n", value[3])
	fmt.Printf("Channel 4 [A4]: \n", value[4])
	fmt.Printf("Channel 5 [A5]: \n", value[5])
	fmt.Printf("Channel 6 [A6]: \n", value[6])
	fmt.Printf("Channel 7 [A7]: \n", value[7])
	fmt.Printf("Channel 8 [B0]: \n", value[8])
	fmt.Printf("Channel 9 [B1]: \n", value[9])
	fmt.Printf("Channel 10 [B2]: \n", value[10])
	fmt.Printf("Channel 11 [B3]: \n", value[11])
	fmt.Printf("Channel 12 [B4]: \n", value[12])
	fmt.Printf("Channel 13 [B5]: \n", value[13])
	fmt.Printf("Channel 14 [B6]: \n", value[14])
	fmt.Printf("Channel 15 [B7]: \n", value[15])

	fmt.Print("Press enter to exit.")
	fmt.Scanln()
}
