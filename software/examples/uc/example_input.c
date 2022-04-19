// This example is not self-contained.
// It requres usage of the example driver specific to your platform.
// See the HAL documentation.

#include "bindings/hal_common.h"
#include "bindings/bricklet_io16_v2.h"

#define UID "XYZ" // Change XYZ to the UID of your IO-16 Bricklet 2.0

void check(int rc, const char* msg);

void example_setup(TF_HAL *hal);
void example_loop(TF_HAL *hal);


static TF_IO16V2 io;

void example_setup(TF_HAL *hal) {
	// Create device object
	check(tf_io16_v2_create(&io, UID, hal), "create device object");

	// Get current value
	bool value[16];
	check(tf_io16_v2_get_value(&io, value), "get value");

	tf_hal_printf("Channel 0 [A0]: %s\n", value[0] ? "true" : "false");
	tf_hal_printf("Channel 1 [A1]: %s\n", value[1] ? "true" : "false");
	tf_hal_printf("Channel 2 [A2]: %s\n", value[2] ? "true" : "false");
	tf_hal_printf("Channel 3 [A3]: %s\n", value[3] ? "true" : "false");
	tf_hal_printf("Channel 4 [A4]: %s\n", value[4] ? "true" : "false");
	tf_hal_printf("Channel 5 [A5]: %s\n", value[5] ? "true" : "false");
	tf_hal_printf("Channel 6 [A6]: %s\n", value[6] ? "true" : "false");
	tf_hal_printf("Channel 7 [A7]: %s\n", value[7] ? "true" : "false");
	tf_hal_printf("Channel 8 [B0]: %s\n", value[8] ? "true" : "false");
	tf_hal_printf("Channel 9 [B1]: %s\n", value[9] ? "true" : "false");
	tf_hal_printf("Channel 10 [B2]: %s\n", value[10] ? "true" : "false");
	tf_hal_printf("Channel 11 [B3]: %s\n", value[11] ? "true" : "false");
	tf_hal_printf("Channel 12 [B4]: %s\n", value[12] ? "true" : "false");
	tf_hal_printf("Channel 13 [B5]: %s\n", value[13] ? "true" : "false");
	tf_hal_printf("Channel 14 [B6]: %s\n", value[14] ? "true" : "false");
	tf_hal_printf("Channel 15 [B7]: %s\n", value[15] ? "true" : "false");
}

void example_loop(TF_HAL *hal) {
	// Poll for callbacks
	tf_hal_callback_tick(hal, 0);
}
