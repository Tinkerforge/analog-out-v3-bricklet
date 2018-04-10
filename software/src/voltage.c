/* analog-out-v3-bricklet
 * Copyright (C) 2018 Olaf Lüke <olaf@tinkerforge.com>
 *
 * voltage.c: Calculate voltage from ADC
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */

#include "voltage.h"

#include <string.h>

#include "configs/config.h"
#include "configs/config_voltage.h"

#include "xmc_vadc.h"
#include "xmc_gpio.h"

#include "communication.h"

#include "bricklib2/hal/system_timer/system_timer.h"
#include "bricklib2/utility/util_definitions.h"

#define VOLTAGE_BUFFER_SIZE 1024
#define VOLTAGE_BUFFER_MASK (VOLTAGE_BUFFER_SIZE-1)

#define voltage_adc_irq_handler IRQ_Hdlr_15
#define ANALOG_IN_V3_OVERSAMPLING_32 0
#define ANALOG_IN_V3_OVERSAMPLING_64 1
#define ANALOG_IN_V3_OVERSAMPLING_128 2
#define ANALOG_IN_V3_OVERSAMPLING_256 3
#define ANALOG_IN_V3_OVERSAMPLING_512 4
#define ANALOG_IN_V3_OVERSAMPLING_1024 5
#define ANALOG_IN_V3_OVERSAMPLING_2048 6
#define ANALOG_IN_V3_OVERSAMPLING_4096 7
#define ANALOG_IN_V3_OVERSAMPLING_8192 8
#define ANALOG_IN_V3_OVERSAMPLING_16384 9

// Keep the variables that are used in the interrupts as simple global variables,
// to be sure that the compiler does not need to do any derefernciation or similar.
static uint16_t voltage_buffer[VOLTAGE_BUFFER_SIZE];
static volatile uint32_t voltage_buffer_index = 0;
static uint32_t voltage_sum = 0;

void __attribute__((optimize("-O3"))) __attribute__ ((section (".ram_code"))) voltage_adc_irq_handler(void) {
	// Remove oldest value from moving average
	voltage_sum -= voltage_buffer[voltage_buffer_index];

	// Save new value to buffer and add it to moving average
	const uint16_t new_value = VADC->GLOBRES & 0xFFFF;
	voltage_buffer[voltage_buffer_index] = new_value;
	voltage_sum += new_value;

	// Increase index
	voltage_buffer_index = (voltage_buffer_index + 1) & VOLTAGE_BUFFER_MASK;
}

void voltage_init_adc(void) {
	// This structure contains the Global related Configuration.
	const XMC_VADC_GLOBAL_CONFIG_t adc_global_config = {
		.boundary0 = (uint32_t) 0, // Lower boundary value for Normal comparison mode
		.boundary1 = (uint32_t) 0, // Upper boundary value for Normal comparison mode

		.class0 = {
			.sample_time_std_conv     = 31,                      // The Sample time is (2*tadci)
			.conversion_mode_standard = XMC_VADC_CONVMODE_12BIT, // 12bit conversion Selected

		},
		.class1 = {
			.sample_time_std_conv     = 31,                      // The Sample time is (2*tadci)
			.conversion_mode_standard = XMC_VADC_CONVMODE_12BIT, // 12bit conversion Selected

		},

		.data_reduction_control         = 0b11, // Accumulate 4 result values
		.wait_for_read_mode             = 0, // GLOBRES Register will not be overwritten until the previous value is read
		.event_gen_enable               = 1, // Result Event from GLOBRES is enabled
		.disable_sleep_mode_control     = 0  // Sleep mode is enabled
	};


	// Global iclass0 configuration
	const XMC_VADC_GLOBAL_CLASS_t adc_global_iclass_config = {
		.conversion_mode_standard = XMC_VADC_CONVMODE_12BIT,
		.sample_time_std_conv	  = 31,
	};

	// Global Result Register configuration structure
	XMC_VADC_RESULT_CONFIG_t adc_global_result_config = {
		.data_reduction_control = 0b11, // Accumulate 4 result values
		.post_processing_mode   = XMC_VADC_DMM_REDUCTION_MODE,
		.wait_for_read_mode  	= 1, // Enabled
		.part_of_fifo       	= 0, // No FIFO
		.event_gen_enable   	= 1  // Enable Result event
	};

	// LLD Background Scan Init Structure
	const XMC_VADC_BACKGROUND_CONFIG_t adc_background_config = {
		.conv_start_mode   = XMC_VADC_STARTMODE_CIR,       // Conversion start mode selected as cancel inject repeat
		.req_src_priority  = XMC_VADC_GROUP_RS_PRIORITY_1, // Priority of the Background request source in the VADC module
		.trigger_signal    = XMC_VADC_REQ_TR_A,            // If Trigger needed then this denotes the Trigger signal
		.trigger_edge      = XMC_VADC_TRIGGER_EDGE_NONE,   // If Trigger needed then this denotes Trigger edge selected
		.gate_signal       = XMC_VADC_REQ_GT_A,			   // If Gating needed then this denotes the Gating signal
		.timer_mode        = 0,							   // Timer Mode Disabled
		.external_trigger  = 0,                            // Trigger is Disabled
		.req_src_interrupt = 0,                            // Background Request source interrupt Disabled
		.enable_auto_scan  = 1,
		.load_mode         = XMC_VADC_SCAN_LOAD_OVERWRITE
	};


	XMC_VADC_GLOBAL_Init(VADC, &adc_global_config);
	XMC_VADC_GLOBAL_StartupCalibration(VADC);

	// Initialize the Global Conversion class 0
	XMC_VADC_GLOBAL_InputClassInit(VADC, adc_global_iclass_config, XMC_VADC_GROUP_CONV_STD, 0);
	// Initialize the Global Conversion class 1
	XMC_VADC_GLOBAL_InputClassInit(VADC, adc_global_iclass_config, XMC_VADC_GROUP_CONV_STD, 1);

	// Initialize the Background Scan hardware
	XMC_VADC_GLOBAL_BackgroundInit(VADC, &adc_background_config);

	// Initialize the global result register
	XMC_VADC_GLOBAL_ResultInit(VADC, &adc_global_result_config);

	XMC_VADC_GLOBAL_BackgroundAddChannelToSequence(VADC, 0, VOLTAGE_ADC_CHANNEL);
	XMC_VADC_GLOBAL_SetResultEventInterruptNode(VADC, XMC_VADC_SR_SHARED_SR0);

	XMC_VADC_GLOBAL_BackgroundTriggerConversion(VADC);

    NVIC_SetPriority(VOLTAGE_ADC_IRQ, VOLTAGE_ADC_IRQ_PRIORITY);
    NVIC_EnableIRQ(VOLTAGE_ADC_IRQ);
}

void voltage_init(void) {
	const XMC_GPIO_CONFIG_t voltage_pin_config = {
		.mode             = XMC_GPIO_MODE_INPUT_TRISTATE,
		.input_hysteresis = XMC_GPIO_INPUT_HYSTERESIS_LARGE,
	};
	XMC_GPIO_Init(VOLTAGE_ADC_PIN, &voltage_pin_config);

	voltage_init_adc();

	// Wait for first adc data to arrive and fill buffer with value.
	// Otherwise it would take ~50ms until we get meaningful data.
	while(voltage_buffer_index == 0);
	NVIC_DisableIRQ(VOLTAGE_ADC_IRQ);
	for(uint16_t i = 1; i < VOLTAGE_BUFFER_SIZE; i++) {
		voltage_buffer[i] = voltage_buffer[0];
	}
	voltage_sum = voltage_buffer[0]*VOLTAGE_BUFFER_SIZE;

	NVIC_EnableIRQ(VOLTAGE_ADC_IRQ);
}

uint16_t voltage_get_voltage(void) {
	//const uint32_t value = (voltage_sum*55ULL)/(21*VOLTAGE_BUFFER_SIZE);
	// Conversion is done by voltage_sum*3300*68/(4095*12*4*VOLTAGE_BUFFER_SIZE);
	// Which is 935/838656. A multiplier of 935 results in an overflow, 
	// we use 187/167731 instead. The difference is irrelevant (1.3*10^-9).
	const uint32_t value = (((uint64_t)voltage_sum)*187)/167731;

	// We never return negative or out-of-bounds values
	return BETWEEN(0, value, UINT16_MAX);
}
