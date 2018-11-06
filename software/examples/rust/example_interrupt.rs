use std::{error::Error, io, thread};
use tinkerforge::{io16_v2_bricklet::*, ipconnection::IpConnection};

const HOST: &str = "127.0.0.1";
const PORT: u16 = 4223;
const UID: &str = "XYZ"; // Change XYZ to the UID of your IO-16 Bricklet 2.0

fn main() -> Result<(), Box<dyn Error>> {
    let ipcon = IpConnection::new(); // Create IP connection
    let io16_v2_bricklet = IO16V2Bricklet::new(UID, &ipcon); // Create device object

    ipcon.connect(HOST, PORT).recv()??; // Connect to brickd
                                        // Don't use device before ipcon is connected

    //Create listener for input value events.
    let input_value_listener = io16_v2_bricklet.get_input_value_receiver();
    // Spawn thread to handle received events. This thread ends when the io16_v2_bricklet
    // is dropped, so there is no need for manual cleanup.
    thread::spawn(move || {
        for event in input_value_listener {
            println!("Channel: {}", event.channel);
            println!("Changed: {}", event.changed);
            println!("Value: {}", event.value);
            println!();
        }
    });

    // Set period for input value (channel 4) callback to 0.5s (500ms)
    io16_v2_bricklet.set_input_value_callback_configuration(4, 500, false);

    println!("Press enter to exit.");
    let mut _input = String::new();
    io::stdin().read_line(&mut _input)?;
    ipcon.disconnect();
    Ok(())
}
