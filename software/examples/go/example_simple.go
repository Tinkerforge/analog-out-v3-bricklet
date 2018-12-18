package main

import (
	"fmt"
	"github.com/tinkerforge/go-api-bindings/analog_out_v3_bricklet"
	"github.com/tinkerforge/go-api-bindings/ipconnection"
)

const ADDR string = "localhost:4223"
const UID string = "XYZ" // Change XYZ to the UID of your Analog Out Bricklet 3.0.

func main() {
	ipcon := ipconnection.New()
	defer ipcon.Close()
	ao, _ := analog_out_v3_bricklet.New(UID, &ipcon) // Create device object.

	ipcon.Connect(ADDR) // Connect to brickd.
	defer ipcon.Disconnect()
	// Don't use device before ipcon is connected.

	// Set output voltage to 3.3V
	ao.SetOutputVoltage(3300)

	fmt.Print("Press enter to exit.")
	fmt.Scanln()

}
