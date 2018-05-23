#!/usr/bin/env ruby
# -*- ruby encoding: utf-8 -*-

require 'tinkerforge/ip_connection'
require 'tinkerforge/bricklet_analog_out_v3'

include Tinkerforge

HOST = 'localhost'
PORT = 4223
UID = 'XYZ' # Change XYZ to the UID of your Analog Out Bricklet 3.0

ipcon = IPConnection.new # Create IP connection
ao = BrickletAnalogOutV3.new UID, ipcon # Create device object

ipcon.connect HOST, PORT # Connect to brickd
# Don't use device before ipcon is connected

# Set output voltage to 3.3V
ao.set_output_voltage 3300

puts 'Press key to exit'
$stdin.gets
ipcon.disconnect
