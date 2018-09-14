import com.tinkerforge.IPConnection;
import com.tinkerforge.BrickletIO16V2;

public class ExampleInterrupt {
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

		// Add input value listener
		io.addInputValueListener(new BrickletIO16V2.InputValueListener() {
			public void inputValue(int channel, boolean changed, boolean value) {
				System.out.println("Channel: " + channel);
				System.out.println("Changed: " + changed);
				System.out.println("Value: " + value);
				System.out.println("");
			}
		});

		// Set period for input value (channel 4) callback to 0.5s (500ms)
		io.setInputValueCallbackConfiguration(4, 500, false);

		System.out.println("Press key to exit"); System.in.read();
		ipcon.disconnect();
	}
}
