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
        // Configure channel 7 as output low
        io.setConfiguration(7, 'o', false);

        // Set channel 7 alternating high/low 10 times with 100 ms delay
        for(var i = 0; i < 10; ++i) {
            setTimeout(function () {
                io.setSelectedValue(7, true);
            }, 200 * i + 100);
            setTimeout(function () {
                io.setSelectedValue(7, false);
            }, 200 * i + 200);
        }
    }
);

console.log('Press key to exit');
process.stdin.on('data',
    function (data) {
        ipcon.disconnect();
        process.exit(0);
    }
);
