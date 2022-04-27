// This example is not self-contained.
// It requires usage of the example driver specific to your platform.
// See the HAL documentation.

#include "src/bindings/hal_common.h"
#include "src/bindings/bricklet_io16_v2.h"

void check(int rc, const char *msg);
void example_setup(TF_HAL *hal);
void example_loop(TF_HAL *hal);

static TF_IO16V2 io;

void example_setup(TF_HAL *hal) {
	// Create device object
	check(tf_io16_v2_create(&io, NULL, hal), "create device object");

	// Configure channel 7 [A7] as output low
	check(tf_io16_v2_set_configuration(&io, 7, 'o', false), "call set_configuration");

	// Set channel 7 [A7] alternating high/low 10 times with 100 ms delay
	int i;
	for (i = 0; i < 10; ++i) {
		tf_hal_sleep_us(hal, 100 * 1000);
		check(tf_io16_v2_set_selected_value(&io, 7, true), "call set_selected_value");
		tf_hal_sleep_us(hal, 100 * 1000);
		check(tf_io16_v2_set_selected_value(&io, 7, false), "call set_selected_value");
	}
}

void example_loop(TF_HAL *hal) {
	// Poll for callbacks
	tf_hal_callback_tick(hal, 0);
}
