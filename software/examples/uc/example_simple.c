

#include "bindings/hal_common.h"
#include "bindings/bricklet_analog_out_v3.h"

#define UID "XYZ" // Change XYZ to the UID of your Analog Out Bricklet 3.0

void check(int rc, const char* msg);




TF_AnalogOutV3 ao;

void example_setup(TF_HalContext *hal) {
	// Create device object
	check(tf_analog_out_v3_create(&ao, UID, hal), "create device object");


	// Set output voltage to 3.3V
	check(tf_analog_out_v3_set_output_voltage(&ao, 3300), "call set_output_voltage");

}

void example_loop(TF_HalContext *hal) {
	// Poll for callbacks
	tf_hal_callback_tick(hal, 0);
}
