#!/usr/bin/env ruby
# -*- ruby encoding: utf-8 -*-

require 'tinkerforge/ip_connection'
require 'tinkerforge/bricklet_io16_v2'

include Tinkerforge

HOST = 'localhost'
PORT = 4223
UID = 'XYZ' # Change XYZ to the UID of your IO-16 Bricklet 2.0

ipcon = IPConnection.new # Create IP connection
io = BrickletIO16V2.new UID, ipcon # Create device object

ipcon.connect HOST, PORT # Connect to brickd
# Don't use device before ipcon is connected

# Register input value callback
io.register_callback(BrickletIO16V2::CALLBACK_INPUT_VALUE) do |channel, changed, value|
  puts "Channel: #{channel}"
  puts "Changed: #{changed}"
  puts "Value: #{value}"
  puts ''
end

# Set period for input value (channel 4) callback to 0.5s (500ms)
io.set_input_value_callback_configuration 4, 500, false

puts 'Press key to exit'
$stdin.gets
ipcon.disconnect
