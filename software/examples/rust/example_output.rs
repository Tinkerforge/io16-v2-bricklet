use std::{error::Error, io, thread, time::Duration};
use tinkerforge::{io16_v2_bricklet::*, ip_connection::IpConnection};

const HOST: &str = "localhost";
const PORT: u16 = 4223;
const UID: &str = "XYZ"; // Change XYZ to the UID of your IO-16 Bricklet 2.0.

fn main() -> Result<(), Box<dyn Error>> {
    let ipcon = IpConnection::new(); // Create IP connection.
    let io = Io16V2Bricklet::new(UID, &ipcon); // Create device object.

    ipcon.connect((HOST, PORT)).recv()??; // Connect to brickd.
                                          // Don't use device before ipcon is connected.

    // Configure channel 7 [A7] as output low
    io.set_configuration(7, 'o', false).recv()?;

    // Set channel 7 [A7] alternating high/low 10 times with 100 ms delay
    for _i in 0..10 {
        thread::sleep(Duration::from_millis(100));
        io.set_selected_value(7, true).recv()?;
        thread::sleep(Duration::from_millis(100));
        io.set_selected_value(7, false).recv()?;
    }

    println!("Press enter to exit.");
    let mut _input = String::new();
    io::stdin().read_line(&mut _input)?;
    ipcon.disconnect();
    Ok(())
}
