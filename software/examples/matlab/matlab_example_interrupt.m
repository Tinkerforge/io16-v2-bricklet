function matlab_example_interrupt()
    import com.tinkerforge.IPConnection;
    import com.tinkerforge.BrickletIO16V2;

    HOST = 'localhost';
    PORT = 4223;
    UID = 'XYZ'; % Change XYZ to the UID of your IO-16 Bricklet 2.0

    ipcon = IPConnection(); % Create IP connection
    io = handle(BrickletIO16V2(UID, ipcon), 'CallbackProperties'); % Create device object

    ipcon.connect(HOST, PORT); % Connect to brickd
    % Don't use device before ipcon is connected

    % Register input_value callback to function cb_input_value
    set(io, 'InputValueCallback', @(h, e) cb_input_value(e));

    % Configure callback for channel 4 with fixed 500ms period
    io.setInputValueCallbackConfiguration(4, 500, false);

    input('Press key to exit\n', 's');
    ipcon.disconnect();
end

% Callback function for input_value callback
function cb_input_value(e)
    fprintf('Channel: %i\n', e.channel);
    fprintf('Changed: %i\n', e.changed);
    fprintf('Value: %i\n', e.value);
    fprintf('\n');
end
