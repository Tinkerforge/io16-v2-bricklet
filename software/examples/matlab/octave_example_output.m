function octave_example_output()
    more off;

    HOST = "localhost";
    PORT = 4223;
    UID = "XYZ"; % Change XYZ to the UID of your IO-16 Bricklet 2.0

    ipcon = javaObject("com.tinkerforge.IPConnection"); % Create IP connection
    io = javaObject("com.tinkerforge.BrickletIO16V2", UID, ipcon); % Create device object

    ipcon.connect(HOST, PORT); % Connect to brickd
    % Don't use device before ipcon is connected

    % Configure channel 7 [A7] as output low
    io.setConfiguration(7, "o", false);

    % Set channel 7 [A7] alternating high/low 10 times with 100 ms delay
    for i = 0:9
        pause(0.1);
        io.setSelectedValue(7, true);
        pause(0.1);
        io.setSelectedValue(7, false);
    end

    input("Press key to exit\n", "s");
    ipcon.disconnect();
end
