use std::{io, error::Error};
use std::thread;
use tinkerforge::{ip_connection::IpConnection, 
                  io16_v2_bricklet::*};


const HOST: &str = "localhost";
const PORT: u16 = 4223;
const UID: &str = "XYZ"; // Change XYZ to the UID of your IO-16 Bricklet 2.0.

fn main() -> Result<(), Box<dyn Error>> {
    let ipcon = IpConnection::new(); // Create IP connection.
    let io = Io16V2Bricklet::new(UID, &ipcon); // Create device object.

    ipcon.connect((HOST, PORT)).recv()??; // Connect to brickd.
    // Don't use device before ipcon is connected.

     let input_value_receiver = io.get_input_value_callback_receiver();

        // Spawn thread to handle received callback messages. 
        // This thread ends when the `io` object
        // is dropped, so there is no need for manual cleanup.
        thread::spawn(move || {
            for input_value in input_value_receiver {           
                		println!("Channel: {}", input_value.channel);
		println!("Changed: {}", input_value.changed);
		println!("Value: {}", input_value.value);
		println!();
            }
        });

		// Set period for input value (channel 4) callback to 0.5s (500ms).
		io.set_input_value_callback_configuration(4, 500, false);

    println!("Press enter to exit.");
    let mut _input = String::new();
    io::stdin().read_line(&mut _input)?;
    ipcon.disconnect();
    Ok(())
}
