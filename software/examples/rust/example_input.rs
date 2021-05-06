use std::{error::Error, io};

use tinkerforge::{io16_v2_bricklet::*, ip_connection::IpConnection};

const HOST: &str = "localhost";
const PORT: u16 = 4223;
const UID: &str = "XYZ"; // Change XYZ to the UID of your IO-16 Bricklet 2.0.

fn main() -> Result<(), Box<dyn Error>> {
    let ipcon = IpConnection::new(); // Create IP connection.
    let io = Io16V2Bricklet::new(UID, &ipcon); // Create device object.

    ipcon.connect((HOST, PORT)).recv()??; // Connect to brickd.
                                          // Don't use device before ipcon is connected.

    // Get current value.
    let value = io.get_value().recv()?;

    println!("Channel 0 [A0]: {}", value[0]);
    println!("Channel 1 [A1]: {}", value[1]);
    println!("Channel 2 [A2]: {}", value[2]);
    println!("Channel 3 [A3]: {}", value[3]);
    println!("Channel 4 [A4]: {}", value[4]);
    println!("Channel 5 [A5]: {}", value[5]);
    println!("Channel 6 [A6]: {}", value[6]);
    println!("Channel 7 [A7]: {}", value[7]);
    println!("Channel 8 [B0]: {}", value[8]);
    println!("Channel 9 [B1]: {}", value[9]);
    println!("Channel 10 [B2]: {}", value[10]);
    println!("Channel 11 [B3]: {}", value[11]);
    println!("Channel 12 [B4]: {}", value[12]);
    println!("Channel 13 [B5]: {}", value[13]);
    println!("Channel 14 [B6]: {}", value[14]);
    println!("Channel 15 [B7]: {}", value[15]);

    println!("Press enter to exit.");
    let mut _input = String::new();
    io::stdin().read_line(&mut _input)?;
    ipcon.disconnect();
    Ok(())
}
