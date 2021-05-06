#!/usr/bin/perl

use strict;
use Tinkerforge::IPConnection;
use Tinkerforge::BrickletIO16V2;

use constant HOST => 'localhost';
use constant PORT => 4223;
use constant UID => 'XYZ'; # Change XYZ to the UID of your IO-16 Bricklet 2.0

# Callback subroutine for input value callback
sub cb_input_value
{
    my ($channel, $changed, $value) = @_;

    print "Channel: $channel\n";
    print "Changed: $changed\n";
    print "Value: $value\n";
    print "\n";
}

my $ipcon = Tinkerforge::IPConnection->new(); # Create IP connection
my $io = Tinkerforge::BrickletIO16V2->new(&UID, $ipcon); # Create device object

$ipcon->connect(&HOST, &PORT); # Connect to brickd
# Don't use device before ipcon is connected

# Register input value callback to subroutine cb_input_value
$io->register_callback($io->CALLBACK_INPUT_VALUE, 'cb_input_value');

# Set period for input value (channel 4 [A4]) callback to 0.5s (500ms)
$io->set_input_value_callback_configuration(4, 500, 0);

print "Press key to exit\n";
<STDIN>;
$ipcon->disconnect();
