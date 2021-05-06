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
        // Get current value
        io.getValue(
            function (value) {
                console.log('Channel 0 [A0]: ' + value[0]);
                console.log('Channel 1 [A1]: ' + value[1]);
                console.log('Channel 2 [A2]: ' + value[2]);
                console.log('Channel 3 [A3]: ' + value[3]);
                console.log('Channel 4 [A4]: ' + value[4]);
                console.log('Channel 5 [A5]: ' + value[5]);
                console.log('Channel 6 [A6]: ' + value[6]);
                console.log('Channel 7 [A7]: ' + value[7]);
                console.log('Channel 8 [B0]: ' + value[8]);
                console.log('Channel 9 [B1]: ' + value[9]);
                console.log('Channel 10 [B2]: ' + value[10]);
                console.log('Channel 11 [B3]: ' + value[11]);
                console.log('Channel 12 [B4]: ' + value[12]);
                console.log('Channel 13 [B5]: ' + value[13]);
                console.log('Channel 14 [B6]: ' + value[14]);
                console.log('Channel 15 [B7]: ' + value[15]);
            },
            function (error) {
                console.log('Error: ' + error);
            }
        );
    }
);

console.log('Press key to exit');
process.stdin.on('data',
    function (data) {
        ipcon.disconnect();
        process.exit(0);
    }
);
