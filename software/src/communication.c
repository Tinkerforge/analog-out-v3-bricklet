/* analog-out-v3-bricklet
 * Copyright (C) 2018 Olaf Lüke <olaf@tinkerforge.com>
 *
 * communication.c: TFP protocol message handling
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

#include "communication.h"

#include "bricklib2/utility/communication_callback.h"
#include "bricklib2/protocols/tfp/tfp.h"

#include "mcp4725.h"
#include "voltage.h"

BootloaderHandleMessageResponse handle_message(const void *message, void *response) {
	switch(tfp_get_fid_from_message(message)) {
		case FID_SET_OUTPUT_VOLTAGE: return set_output_voltage(message);
		case FID_GET_OUTPUT_VOLTAGE: return get_output_voltage(message, response);
		case FID_GET_INPUT_VOLTAGE: return get_input_voltage(message, response);
		default: return HANDLE_MESSAGE_RESPONSE_NOT_SUPPORTED;
	}
}


BootloaderHandleMessageResponse set_output_voltage(const SetOutputVoltage *data) {
	if(data->voltage > 12000) {
		return HANDLE_MESSAGE_RESPONSE_INVALID_PARAMETER;
	}

	mcp4725.voltage_output = data->voltage;
	return HANDLE_MESSAGE_RESPONSE_EMPTY;
}

BootloaderHandleMessageResponse get_output_voltage(const GetOutputVoltage *data, GetOutputVoltage_Response *response) {
	response->header.length = sizeof(GetOutputVoltage_Response);
	response->voltage       = mcp4725.voltage_output;

	return HANDLE_MESSAGE_RESPONSE_NEW_MESSAGE;
}

BootloaderHandleMessageResponse get_input_voltage(const GetInputVoltage *data, GetInputVoltage_Response *response) {
	response->header.length = sizeof(GetInputVoltage_Response);
	response->voltage       = voltage_get_voltage();

	return HANDLE_MESSAGE_RESPONSE_NEW_MESSAGE;
}


void communication_tick(void) {
	communication_callback_tick();
}

void communication_init(void) {
	communication_callback_init();
}
