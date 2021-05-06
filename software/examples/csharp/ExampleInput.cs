using System;
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

		// Get current value
		bool[] value = io.GetValue();

		Console.WriteLine("Channel 0 [A0]: " + value[0]);
		Console.WriteLine("Channel 1 [A1]: " + value[1]);
		Console.WriteLine("Channel 2 [A2]: " + value[2]);
		Console.WriteLine("Channel 3 [A3]: " + value[3]);
		Console.WriteLine("Channel 4 [A4]: " + value[4]);
		Console.WriteLine("Channel 5 [A5]: " + value[5]);
		Console.WriteLine("Channel 6 [A6]: " + value[6]);
		Console.WriteLine("Channel 7 [A7]: " + value[7]);
		Console.WriteLine("Channel 8 [B0]: " + value[8]);
		Console.WriteLine("Channel 9 [B1]: " + value[9]);
		Console.WriteLine("Channel 10 [B2]: " + value[10]);
		Console.WriteLine("Channel 11 [B3]: " + value[11]);
		Console.WriteLine("Channel 12 [B4]: " + value[12]);
		Console.WriteLine("Channel 13 [B5]: " + value[13]);
		Console.WriteLine("Channel 14 [B6]: " + value[14]);
		Console.WriteLine("Channel 15 [B7]: " + value[15]);

		Console.WriteLine("Press enter to exit");
		Console.ReadLine();
		ipcon.Disconnect();
	}
}
