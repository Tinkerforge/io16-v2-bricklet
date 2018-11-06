use std::{error::Error, io, thread, time::Duration};
use tinkerforge::{io16_v2_bricklet::*, ipconnection::IpConnection};

const HOST: &str = "127.0.0.1";
const PORT: u16 = 4223;
const UID: &str = "XYZ"; // Change XYZ to the UID of your IO-16 Bricklet 2.0

fn main() -> Result<(), Box<dyn Error>> {
    let ipcon = IpConnection::new(); // Create IP connection
    let io16_v2_bricklet = IO16V2Bricklet::new(UID, &ipcon); // Create device object

    ipcon.connect(HOST, PORT).recv()??; // Connect to brickd
                                        // Don't use device before ipcon is connected

    // Configure channel 7 as output low
    io16_v2_bricklet.set_configuration(7, 'o', false);

    // Set channel 7 alternating high/low 10 times with 100 ms delay
    for i in 0..10 {
        thread::sleep(Duration::from_millis(100));
        io16_v2_bricklet.set_selected_value(7, true);
        thread::sleep(Duration::from_millis(100));
        io16_v2_bricklet.set_selected_value(7, false);
    }

    println!("Press enter to exit.");
    let mut _input = String::new();
    io::stdin().read_line(&mut _input)?;
    ipcon.disconnect();
    Ok(())
}
