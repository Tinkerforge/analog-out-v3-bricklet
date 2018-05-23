<?php

require_once('Tinkerforge/IPConnection.php');
require_once('Tinkerforge/BrickletAnalogOutV3.php');

use Tinkerforge\IPConnection;
use Tinkerforge\BrickletAnalogOutV3;

const HOST = 'localhost';
const PORT = 4223;
const UID = 'XYZ'; // Change XYZ to the UID of your Analog Out Bricklet 3.0

$ipcon = new IPConnection(); // Create IP connection
$ao = new BrickletAnalogOutV3(UID, $ipcon); // Create device object

$ipcon->connect(HOST, PORT); // Connect to brickd
// Don't use device before ipcon is connected

// Set output voltage to 3.3V
$ao->setOutputVoltage(3300);

echo "Press key to exit\n";
fgetc(fopen('php://stdin', 'r'));
$ipcon->disconnect();

?>
