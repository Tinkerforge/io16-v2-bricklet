#include "bindings/hal_common.h"
#include "bindings/bricklet_io16_v2.h"

#define UID "XYZ" // Change XYZ to the UID of your IO-16 Bricklet 2.0

void check(int rc, const char* msg);

void example_setup(TF_HalContext *hal);
void example_loop(TF_HalContext *hal);


static TF_IO16V2 io;

void example_setup(TF_HalContext *hal) {
	// Create device object
	check(tf_io16_v2_create(&io, UID, hal), "create device object");

	// Configure channel 7 as output low
	check(tf_io16_v2_set_configuration(&io, 7, 'o', false), "call set_configuration");

	// Set channel 7 alternating high/low 10 times with 100 ms delay
	int i;
	for(i = 0; i < 10; ++i) {
		tf_hal_sleep_us(hal, 100 * 1000);
		check(tf_io16_v2_set_selected_value(&io, 7, true), "call set_selected_value");
		tf_hal_sleep_us(hal, 100 * 1000);
		check(tf_io16_v2_set_selected_value(&io, 7, false), "call set_selected_value");
	}
}

void example_loop(TF_HalContext *hal) {
	// Poll for callbacks
	tf_hal_callback_tick(hal, 0);
}
