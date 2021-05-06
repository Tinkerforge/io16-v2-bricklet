<?php

require_once('Tinkerforge/IPConnection.php');
require_once('Tinkerforge/BrickletIO16V2.php');

use Tinkerforge\IPConnection;
use Tinkerforge\BrickletIO16V2;

const HOST = 'localhost';
const PORT = 4223;
const UID = 'XYZ'; // Change XYZ to the UID of your IO-16 Bricklet 2.0

$ipcon = new IPConnection(); // Create IP connection
$io = new BrickletIO16V2(UID, $ipcon); // Create device object

$ipcon->connect(HOST, PORT); // Connect to brickd
// Don't use device before ipcon is connected

// Get current value
$value = $io->getValue();

echo "Channel 0 [A0]: " . $value[0] . "\n";
echo "Channel 1 [A1]: " . $value[1] . "\n";
echo "Channel 2 [A2]: " . $value[2] . "\n";
echo "Channel 3 [A3]: " . $value[3] . "\n";
echo "Channel 4 [A4]: " . $value[4] . "\n";
echo "Channel 5 [A5]: " . $value[5] . "\n";
echo "Channel 6 [A6]: " . $value[6] . "\n";
echo "Channel 7 [A7]: " . $value[7] . "\n";
echo "Channel 8 [B0]: " . $value[8] . "\n";
echo "Channel 9 [B1]: " . $value[9] . "\n";
echo "Channel 10 [B2]: " . $value[10] . "\n";
echo "Channel 11 [B3]: " . $value[11] . "\n";
echo "Channel 12 [B4]: " . $value[12] . "\n";
echo "Channel 13 [B5]: " . $value[13] . "\n";
echo "Channel 14 [B6]: " . $value[14] . "\n";
echo "Channel 15 [B7]: " . $value[15] . "\n";

echo "Press key to exit\n";
fgetc(fopen('php://stdin', 'r'));
$ipcon->disconnect();

?>
