// This example is not self-contained.
// It requires usage of the example driver specific to your platform.
// See the HAL documentation.

#include "src/bindings/hal_common.h"
#include "src/bindings/bricklet_analog_out_v3.h"

void check(int rc, const char *msg);
void example_setup(TF_HAL *hal);
void example_loop(TF_HAL *hal);

static TF_AnalogOutV3 ao;

void example_setup(TF_HAL *hal) {
	// Create device object
	check(tf_analog_out_v3_create(&ao, NULL, hal), "create device object");

	// Set output voltage to 3.3V
	check(tf_analog_out_v3_set_output_voltage(&ao, 3300), "call set_output_voltage");
}

void example_loop(TF_HAL *hal) {
	// Poll for callbacks
	tf_hal_callback_tick(hal, 0);
}
