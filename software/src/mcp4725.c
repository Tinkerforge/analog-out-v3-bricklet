/* analog-out-v3-bricklet
 * Copyright (C) 2018 Olaf Lüke <olaf@tinkerforge.com>
 *
 * mcp4725.c: Driver for MCP4725 D/A converter
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

#include "mcp4725.h"

#include "configs/config_mcp4725.h"

#include "bricklib2/hal/system_timer/system_timer.h"
#include "bricklib2/logging/logging.h"
#include "bricklib2/utility/util_definitions.h"

MCP4725 mcp4725;

void mcp4725_init_i2c(void) {
	memset(&mcp4725, 0, sizeof(MCP4725));

	mcp4725.i2c_fifo.baudrate         = MCP4725_I2C_BAUDRATE;
	mcp4725.i2c_fifo.address          = MCP4725_I2C_ADDRESS;
	mcp4725.i2c_fifo.i2c              = MCP4725_I2C;

	mcp4725.i2c_fifo.scl_port         = MCP4725_SCL_PORT;
	mcp4725.i2c_fifo.scl_pin          = MCP4725_SCL_PIN;
	mcp4725.i2c_fifo.scl_mode         = MCP4725_SCL_PIN_MODE;
	mcp4725.i2c_fifo.scl_input        = MCP4725_SCL_INPUT;
	mcp4725.i2c_fifo.scl_source       = MCP4725_SCL_SOURCE;
	mcp4725.i2c_fifo.scl_fifo_size    = MCP4725_SCL_FIFO_SIZE;
	mcp4725.i2c_fifo.scl_fifo_pointer = MCP4725_SCL_FIFO_POINTER;

	mcp4725.i2c_fifo.sda_port         = MCP4725_SDA_PORT;
	mcp4725.i2c_fifo.sda_pin          = MCP4725_SDA_PIN;
	mcp4725.i2c_fifo.sda_mode         = MCP4725_SDA_PIN_MODE;
	mcp4725.i2c_fifo.sda_input        = MCP4725_SDA_INPUT;
	mcp4725.i2c_fifo.sda_source       = MCP4725_SDA_SOURCE;
	mcp4725.i2c_fifo.sda_fifo_size    = MCP4725_SDA_FIFO_SIZE;
	mcp4725.i2c_fifo.sda_fifo_pointer = MCP4725_SDA_FIFO_POINTER;

	i2c_fifo_init(&mcp4725.i2c_fifo);

	// Force an update of voltage data
	mcp4725.last_voltage_output = ~mcp4725.voltage_output;
}

void mcp4725_init(void) {
	mcp4725.voltage_output = 0;
	mcp4725_init_i2c();
}

void mcp4725_tick(void) {
	I2CFifoState state = i2c_fifo_next_state(&mcp4725.i2c_fifo);
	if(state & I2C_FIFO_STATE_ERROR) {
        loge("MCP4725 I2C error: %d (%d)\n\r", state, mcp4725.i2c_fifo.i2c_status);
        mcp4725_init();
        return;
	}

    switch(state) {
        case I2C_FIFO_STATE_WRITE_DIRECT_READY: {
			mcp4725.in_progress = false;
            break;
        }

        case I2C_FIFO_STATE_IDLE: {
			break; // Handled below
		}

		default: {
			// If we end up in a ready state that we don't handle, something went wrong
			if(state & I2C_FIFO_STATE_READY) {
				loge("MCP4725 unrecognized I2C ready state: %d\n\r", state);
				mcp4725_init();
			}

			return;
		}
	}

    if((state == I2C_FIFO_STATE_IDLE || (state & I2C_FIFO_STATE_READY) != 0)) {
        if(mcp4725.voltage_output != mcp4725.last_voltage_output) {
            mcp4725.last_voltage_output = mcp4725.voltage_output;
            const uint16_t set_voltage = mcp4725.voltage_output*4095/12000;
            uint8_t data[2] = {set_voltage >> 8, set_voltage & 0xFF};
			mcp4725.in_progress = true;
            i2c_fifo_write_direct(&mcp4725.i2c_fifo, 2, data, true);
        }
    }
}
