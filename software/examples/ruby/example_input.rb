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

# Get current value
value = io.get_value

puts "Channel 0 [A0]: #{value[0]}"
puts "Channel 1 [A1]: #{value[1]}"
puts "Channel 2 [A2]: #{value[2]}"
puts "Channel 3 [A3]: #{value[3]}"
puts "Channel 4 [A4]: #{value[4]}"
puts "Channel 5 [A5]: #{value[5]}"
puts "Channel 6 [A6]: #{value[6]}"
puts "Channel 7 [A7]: #{value[7]}"
puts "Channel 8 [B0]: #{value[8]}"
puts "Channel 9 [B1]: #{value[9]}"
puts "Channel 10 [B2]: #{value[10]}"
puts "Channel 11 [B3]: #{value[11]}"
puts "Channel 12 [B4]: #{value[12]}"
puts "Channel 13 [B5]: #{value[13]}"
puts "Channel 14 [B6]: #{value[14]}"
puts "Channel 15 [B7]: #{value[15]}"

puts 'Press key to exit'
$stdin.gets
ipcon.disconnect
