#!/usr/bin/perl

use Tinkerforge::IPConnection;
use Tinkerforge::BrickletAnalogOutV3;

use constant HOST => 'localhost';
use constant PORT => 4223;
use constant UID => 'XYZ'; # Change XYZ to the UID of your Analog Out Bricklet 3.0

my $ipcon = Tinkerforge::IPConnection->new(); # Create IP connection
my $ao = Tinkerforge::BrickletAnalogOutV3->new(&UID, $ipcon); # Create device object

$ipcon->connect(&HOST, &PORT); # Connect to brickd
# Don't use device before ipcon is connected

# Set output voltage to 3.3V
$ao->set_output_voltage(3300);

print "Press key to exit\n";
<STDIN>;
$ipcon->disconnect();
