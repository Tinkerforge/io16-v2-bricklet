using System;
using Tinkerforge;

class Example
{
	private static string HOST = "localhost";
	private static int PORT = 4223;
	private static string UID = "XYZ"; // Change XYZ to the UID of your IO-16 Bricklet 2.0

	// Callback function for input value callback
	static void InputValueCB(BrickletIO16V2 sender, byte channel, bool changed,
	                         bool value)
	{
		Console.WriteLine("Channel: " + channel);
		Console.WriteLine("Changed: " + changed);
		Console.WriteLine("Value: " + value);
		Console.WriteLine("");
	}

	static void Main()
	{
		IPConnection ipcon = new IPConnection(); // Create IP connection
		BrickletIO16V2 io = new BrickletIO16V2(UID, ipcon); // Create device object

		ipcon.Connect(HOST, PORT); // Connect to brickd
		// Don't use device before ipcon is connected

		// Register input value callback to function InputValueCB
		io.InputValueCallback += InputValueCB;

		// Set period for input value (channel 4 [A4]) callback to 0.5s (500ms)
		io.SetInputValueCallbackConfiguration(4, 500, false);

		Console.WriteLine("Press enter to exit");
		Console.ReadLine();
		ipcon.Disconnect();
	}
}
