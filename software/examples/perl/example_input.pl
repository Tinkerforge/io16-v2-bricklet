#!/usr/bin/perl

use strict;
use Tinkerforge::IPConnection;
use Tinkerforge::BrickletIO16V2;

use constant HOST => 'localhost';
use constant PORT => 4223;
use constant UID => 'XYZ'; # Change XYZ to the UID of your IO-16 Bricklet 2.0

my $ipcon = Tinkerforge::IPConnection->new(); # Create IP connection
my $io = Tinkerforge::BrickletIO16V2->new(&UID, $ipcon); # Create device object

$ipcon->connect(&HOST, &PORT); # Connect to brickd
# Don't use device before ipcon is connected

# Get current value
my $value = $io->get_value();

print "Channel 0 [A0]: " . @{$value}[0] . "\n";
print "Channel 1 [A1]: " . @{$value}[1] . "\n";
print "Channel 2 [A2]: " . @{$value}[2] . "\n";
print "Channel 3 [A3]: " . @{$value}[3] . "\n";
print "Channel 4 [A4]: " . @{$value}[4] . "\n";
print "Channel 5 [A5]: " . @{$value}[5] . "\n";
print "Channel 6 [A6]: " . @{$value}[6] . "\n";
print "Channel 7 [A7]: " . @{$value}[7] . "\n";
print "Channel 8 [B0]: " . @{$value}[8] . "\n";
print "Channel 9 [B1]: " . @{$value}[9] . "\n";
print "Channel 10 [B2]: " . @{$value}[10] . "\n";
print "Channel 11 [B3]: " . @{$value}[11] . "\n";
print "Channel 12 [B4]: " . @{$value}[12] . "\n";
print "Channel 13 [B5]: " . @{$value}[13] . "\n";
print "Channel 14 [B6]: " . @{$value}[14] . "\n";
print "Channel 15 [B7]: " . @{$value}[15] . "\n";

print "Press key to exit\n";
<STDIN>;
$ipcon->disconnect();
