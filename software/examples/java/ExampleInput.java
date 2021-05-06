import com.tinkerforge.IPConnection;
import com.tinkerforge.BrickletIO16V2;

public class ExampleInput {
	private static final String HOST = "localhost";
	private static final int PORT = 4223;

	// Change XYZ to the UID of your IO-16 Bricklet 2.0
	private static final String UID = "XYZ";

	// Note: To make the example code cleaner we do not handle exceptions. Exceptions
	//       you might normally want to catch are described in the documentation
	public static void main(String args[]) throws Exception {
		IPConnection ipcon = new IPConnection(); // Create IP connection
		BrickletIO16V2 io = new BrickletIO16V2(UID, ipcon); // Create device object

		ipcon.connect(HOST, PORT); // Connect to brickd
		// Don't use device before ipcon is connected

		// Get current value
		boolean[] value = io.getValue(); // Can throw com.tinkerforge.TimeoutException

		System.out.println("Channel 0 [A0]: " + value[0]);
		System.out.println("Channel 1 [A1]: " + value[1]);
		System.out.println("Channel 2 [A2]: " + value[2]);
		System.out.println("Channel 3 [A3]: " + value[3]);
		System.out.println("Channel 4 [A4]: " + value[4]);
		System.out.println("Channel 5 [A5]: " + value[5]);
		System.out.println("Channel 6 [A6]: " + value[6]);
		System.out.println("Channel 7 [A7]: " + value[7]);
		System.out.println("Channel 8 [B0]: " + value[8]);
		System.out.println("Channel 9 [B1]: " + value[9]);
		System.out.println("Channel 10 [B2]: " + value[10]);
		System.out.println("Channel 11 [B3]: " + value[11]);
		System.out.println("Channel 12 [B4]: " + value[12]);
		System.out.println("Channel 13 [B5]: " + value[13]);
		System.out.println("Channel 14 [B6]: " + value[14]);
		System.out.println("Channel 15 [B7]: " + value[15]);

		System.out.println("Press key to exit"); System.in.read();
		ipcon.disconnect();
	}
}
