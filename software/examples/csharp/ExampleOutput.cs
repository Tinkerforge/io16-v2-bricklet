using System;
using System.Threading;
using Tinkerforge;

class Example
{
	private static string HOST = "localhost";
	private static int PORT = 4223;
	private static string UID = "XYZ"; // Change XYZ to the UID of your IO-16 Bricklet 2.0

	static void Main()
	{
		IPConnection ipcon = new IPConnection(); // Create IP connection
		BrickletIO16V2 io = new BrickletIO16V2(UID, ipcon); // Create device object

		ipcon.Connect(HOST, PORT); // Connect to brickd
		// Don't use device before ipcon is connected

		// Configure channel 7 as output low
		io.SetConfiguration(7, 'o', false);

		// Set channel 7 alternating high/low 10 times with 100 ms delay
		for(int i = 0; i < 10; i++)
		{
			Thread.Sleep(100);
			io.SetSelectedValue(7, true);
			Thread.Sleep(100);
			io.SetSelectedValue(7, false);
		}

		Console.WriteLine("Press enter to exit");
		Console.ReadLine();
		ipcon.Disconnect();
	}
}
