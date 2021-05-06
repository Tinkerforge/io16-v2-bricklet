var Tinkerforge = require('tinkerforge');

var HOST = 'localhost';
var PORT = 4223;
var UID = 'XYZ'; // Change XYZ to the UID of your IO-16 Bricklet 2.0

var ipcon = new Tinkerforge.IPConnection(); // Create IP connection
var io = new Tinkerforge.BrickletIO16V2(UID, ipcon); // Create device object

ipcon.connect(HOST, PORT,
    function (error) {
        console.log('Error: ' + error);
    }
); // Connect to brickd
// Don't use device before ipcon is connected

ipcon.on(Tinkerforge.IPConnection.CALLBACK_CONNECTED,
    function (connectReason) {
        // Set period for input value (channel 4 [A4]) callback to 0.5s (500ms)
        io.setInputValueCallbackConfiguration(4, 500, false);
    }
);

// Register input value callback
io.on(Tinkerforge.BrickletIO16V2.CALLBACK_INPUT_VALUE,
    // Callback function for input value callback
    function (channel, changed, value) {
        console.log('Channel: ' + channel);
        console.log('Changed: ' + changed);
        console.log('Value: ' + value);
        console.log();
    }
);

console.log('Press key to exit');
process.stdin.on('data',
    function (data) {
        ipcon.disconnect();
        process.exit(0);
    }
);
