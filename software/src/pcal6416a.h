/* io16-v2-bricklet
 * Copyright (C) 2018 Olaf Lüke <olaf@tinkerforge.com>
 *
 * pcal6416a.h: Driver for PCAL6416A io expander
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

#ifndef PCAL6416A_H
#define PCAL6416A_H

#include <stdint.h>
#include <stdbool.h>

#include "bricklib2/hal/i2c_fifo/i2c_fifo.h"
typedef enum {
    PCAL6416A_STATE_IDLE,
    PCAL6416A_STATE_READ_INPUT_VALUE,
    PCAL6416A_STATE_WRITE_INOUT,
    PCAL6416A_STATE_WRITE_PULLUP_ENABLE,
    PCAL6416A_STATE_WRITE_OUTPUT_VALUE,
} PCAL6416AState;

typedef struct {
    uint16_t input_value; // 1 = high, 0 = low, input and output
    uint16_t inout; // 1 = input, 0 = output
    uint16_t pullup_enable; // 1 = enable
    uint16_t output_value; // 1 = high, 0 = low

    uint16_t last_inout;
    uint16_t last_pullup_enable;
    uint16_t last_output_value;

    uint16_t set_inout;
    uint16_t set_pullup_enable;
    uint16_t set_output_value;

    I2CFifo i2c_fifo;
    PCAL6416AState state;
} PCAL6416A;

extern PCAL6416A pcal6416a;

void pcal6416a_init(void);
void pcal6416a_tick(void);

#define PCAL6416A_REG_INPUT_PORT 0x00
#define PCAL6416A_REG_OUTPUT_PORT 0x02
#define PCAL6416A_REG_POLARITY_INVERSION 0x04
#define PCAL6416A_REG_CONFIGURATION 0x06
#define PCAL6416A_REG_OUTPUT_DRIVE_STRENGHT 0x40
#define PCAL6416A_REG_INPUT_LATCH 0x44
#define PCAL6416A_REG_PULLUP_ENABLE 0x46
#define PCAL6416A_REG_PULLUP_SELECTION 0x48
#define PCAL6416A_REG_INTERRUPT_MASK 0x4A
#define PCAL6416A_REG_INTERRUPT_STATUS 0x4C
#define PCAL6416A_REG_OUTPUT_PORT_CONFIGURATION 0x4F

#endif