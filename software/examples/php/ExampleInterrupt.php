<?php

require_once('Tinkerforge/IPConnection.php');
require_once('Tinkerforge/BrickletIO16V2.php');

use Tinkerforge\IPConnection;
use Tinkerforge\BrickletIO16V2;

const HOST = 'localhost';
const PORT = 4223;
const UID = 'XYZ'; // Change XYZ to the UID of your IO-16 Bricklet 2.0

// Callback function for input_value callback
function cb_inputValue($channel, $changed, $value)
{
    echo "Channel: $channel\n";
    echo "Changed: $changed\n";
    echo "Value: $value\n";
    echo "\n";
}

$ipcon = new IPConnection(); // Create IP connection
$io = new BrickletIO16V2(UID, $ipcon); // Create device object

$ipcon->connect(HOST, PORT); // Connect to brickd
// Don't use device before ipcon is connected

// Register input_value callback to function cb_inputValue
$io->registerCallback(BrickletIO16V2::CALLBACK_INPUT_VALUE, 'cb_inputValue');

// Configure callback for channel 4 with fixed 500ms period
$io->setInputValueCallbackConfiguration(4, 500, FALSE);

echo "Press ctrl+c to exit\n";
$ipcon->dispatchCallbacks(-1); // Dispatch callbacks forever

?>
