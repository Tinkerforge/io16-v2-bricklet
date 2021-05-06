function matlab_example_input()
    import com.tinkerforge.IPConnection;
    import com.tinkerforge.BrickletIO16V2;

    HOST = 'localhost';
    PORT = 4223;
    UID = 'XYZ'; % Change XYZ to the UID of your IO-16 Bricklet 2.0

    ipcon = IPConnection(); % Create IP connection
    io = handle(BrickletIO16V2(UID, ipcon), 'CallbackProperties'); % Create device object

    ipcon.connect(HOST, PORT); % Connect to brickd
    % Don't use device before ipcon is connected

    % Get current value
    value = io.getValue();

    fprintf('Channel 0 [A0]: %i\n', value(1));
    fprintf('Channel 1 [A1]: %i\n', value(2));
    fprintf('Channel 2 [A2]: %i\n', value(3));
    fprintf('Channel 3 [A3]: %i\n', value(4));
    fprintf('Channel 4 [A4]: %i\n', value(5));
    fprintf('Channel 5 [A5]: %i\n', value(6));
    fprintf('Channel 6 [A6]: %i\n', value(7));
    fprintf('Channel 7 [A7]: %i\n', value(8));
    fprintf('Channel 8 [B0]: %i\n', value(9));
    fprintf('Channel 9 [B1]: %i\n', value(10));
    fprintf('Channel 10 [B2]: %i\n', value(11));
    fprintf('Channel 11 [B3]: %i\n', value(12));
    fprintf('Channel 12 [B4]: %i\n', value(13));
    fprintf('Channel 13 [B5]: %i\n', value(14));
    fprintf('Channel 14 [B6]: %i\n', value(15));
    fprintf('Channel 15 [B7]: %i\n', value(16));

    input('Press key to exit\n', 's');
    ipcon.disconnect();
end
