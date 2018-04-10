/* analog-out-v3-bricklet
 * Copyright (C) 2018 Olaf Lüke <olaf@tinkerforge.com>
 *
 * mcp4725.h: Driver for MCP4725 D/A converter
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

#ifndef MCP4725_H
#define MCP4725_H

#include <stdint.h>
#include <stdbool.h>

#include "bricklib2/hal/i2c_fifo/i2c_fifo.h"

typedef struct {
    I2CFifo i2c_fifo;

    uint16_t voltage_output;
    uint16_t last_voltage_output;

    bool in_progress;
    uint32_t start;
} MCP4725;

extern MCP4725 mcp4725;

void mcp4725_init(void);
void mcp4725_tick(void);
uint16_t mcp4725_get_input_voltage(void);

#endif