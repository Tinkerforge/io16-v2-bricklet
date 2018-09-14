function octave_example_interrupt()
    more off;

    HOST = "localhost";
    PORT = 4223;
    UID = "XYZ"; % Change XYZ to the UID of your IO-16 Bricklet 2.0

    ipcon = javaObject("com.tinkerforge.IPConnection"); % Create IP connection
    io = javaObject("com.tinkerforge.BrickletIO16V2", UID, ipcon); % Create device object

    ipcon.connect(HOST, PORT); % Connect to brickd
    % Don't use device before ipcon is connected

    % Register input value callback to function cb_input_value
    io.addInputValueCallback(@cb_input_value);

    % Set period for input value (channel 4) callback to 0.5s (500ms)
    io.setInputValueCallbackConfiguration(4, 500, false);

    input("Press key to exit\n", "s");
    ipcon.disconnect();
end

% Callback function for input value callback
function cb_input_value(e)
    fprintf("Channel: %d\n", e.channel);
    fprintf("Changed: %d\n", e.changed);
    fprintf("Value: %d\n", e.value);
    fprintf("\n");
end
