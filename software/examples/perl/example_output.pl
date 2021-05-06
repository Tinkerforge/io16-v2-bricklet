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

# Configure channel 7 [A7] as output low
$io->set_configuration(7, 'o', 0);

# Set channel 7 [A7] alternating high/low 10 times with 100 ms delay
for (my $i = 0; $i < 10; $i++)
{
    select(undef, undef, undef, 0.1);
    $io->set_selected_value(7, 1);
    select(undef, undef, undef, 0.1);
    $io->set_selected_value(7, 0);
}

print "Press key to exit\n";
<STDIN>;
$ipcon->disconnect();
