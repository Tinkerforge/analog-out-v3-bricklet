function matlab_example_simple()
    import com.tinkerforge.IPConnection;
    import com.tinkerforge.BrickletAnalogOutV3;

    HOST = 'localhost';
    PORT = 4223;
    UID = 'XYZ'; % Change XYZ to the UID of your Analog Out Bricklet 3.0

    ipcon = IPConnection(); % Create IP connection
    ao = handle(BrickletAnalogOutV3(UID, ipcon), 'CallbackProperties'); % Create device object

    ipcon.connect(HOST, PORT); % Connect to brickd
    % Don't use device before ipcon is connected

    % Set output voltage to 3.3V
    ao.setOutputVoltage(3300);

    input('Press key to exit\n', 's');
    ipcon.disconnect();
end
