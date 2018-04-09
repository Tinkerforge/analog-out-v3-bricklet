/* analog-out-v3-bricklet
 * Copyright (C) 2018 Olaf Lüke <olaf@tinkerforge.com>
 *
 * communication.h: TFP protocol message handling
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

#ifndef COMMUNICATION_H
#define COMMUNICATION_H

#include <stdint.h>
#include <stdbool.h>

#include "bricklib2/protocols/tfp/tfp.h"
#include "bricklib2/bootloader/bootloader.h"

// Default functions
BootloaderHandleMessageResponse handle_message(const void *data, void *response);
void communication_tick(void);
void communication_init(void);

// Constants
#define ANALOG_OUT_V3_BOOTLOADER_MODE_BOOTLOADER 0
#define ANALOG_OUT_V3_BOOTLOADER_MODE_FIRMWARE 1
#define ANALOG_OUT_V3_BOOTLOADER_MODE_BOOTLOADER_WAIT_FOR_REBOOT 2
#define ANALOG_OUT_V3_BOOTLOADER_MODE_FIRMWARE_WAIT_FOR_REBOOT 3
#define ANALOG_OUT_V3_BOOTLOADER_MODE_FIRMWARE_WAIT_FOR_ERASE_AND_REBOOT 4

#define ANALOG_OUT_V3_BOOTLOADER_STATUS_OK 0
#define ANALOG_OUT_V3_BOOTLOADER_STATUS_INVALID_MODE 1
#define ANALOG_OUT_V3_BOOTLOADER_STATUS_NO_CHANGE 2
#define ANALOG_OUT_V3_BOOTLOADER_STATUS_ENTRY_FUNCTION_NOT_PRESENT 3
#define ANALOG_OUT_V3_BOOTLOADER_STATUS_DEVICE_IDENTIFIER_INCORRECT 4
#define ANALOG_OUT_V3_BOOTLOADER_STATUS_CRC_MISMATCH 5

#define ANALOG_OUT_V3_STATUS_LED_CONFIG_OFF 0
#define ANALOG_OUT_V3_STATUS_LED_CONFIG_ON 1
#define ANALOG_OUT_V3_STATUS_LED_CONFIG_SHOW_HEARTBEAT 2
#define ANALOG_OUT_V3_STATUS_LED_CONFIG_SHOW_STATUS 3

// Function and callback IDs and structs
#define FID_SET_OUTPUT_VOLTAGE 1
#define FID_GET_OUTPUT_VOLTAGE 2
#define FID_GET_INPUT_VOLTAGE 3


typedef struct {
	TFPMessageHeader header;
	uint16_t voltage;
} __attribute__((__packed__)) SetOutputVoltage;

typedef struct {
	TFPMessageHeader header;
} __attribute__((__packed__)) GetOutputVoltage;

typedef struct {
	TFPMessageHeader header;
	uint16_t voltage;
} __attribute__((__packed__)) GetOutputVoltage_Response;

typedef struct {
	TFPMessageHeader header;
} __attribute__((__packed__)) GetInputVoltage;

typedef struct {
	TFPMessageHeader header;
	uint16_t voltage;
} __attribute__((__packed__)) GetInputVoltage_Response;


// Function prototypes
BootloaderHandleMessageResponse set_output_voltage(const SetOutputVoltage *data);
BootloaderHandleMessageResponse get_output_voltage(const GetOutputVoltage *data, GetOutputVoltage_Response *response);
BootloaderHandleMessageResponse get_input_voltage(const GetInputVoltage *data, GetInputVoltage_Response *response);

// Callbacks


#define COMMUNICATION_CALLBACK_TICK_WAIT_MS 1
#define COMMUNICATION_CALLBACK_HANDLER_NUM 0
#define COMMUNICATION_CALLBACK_LIST_INIT \


#endif
