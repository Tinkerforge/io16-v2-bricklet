/* io16-v2-bricklet
 * Copyright (C) 2018 Olaf Lüke <olaf@tinkerforge.com>
 *
 * pcal6416a.c: Driver for PCAL6416A io expander
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

#include "pcal6416a.h"

#include "configs/config_pcal6416a.h"

#include "bricklib2/logging/logging.h"
#include "bricklib2/hal/system_timer/system_timer.h"

PCAL6416A pcal6416a;

void pcal6416a_init(void) {
	memset(&pcal6416a, 0, sizeof(PCAL6416A));

	pcal6416a.i2c_fifo.baudrate         = PCAL6416A_I2C_BAUDRATE;
	pcal6416a.i2c_fifo.address          = PCAL6416A_I2C_ADDRESS;
	pcal6416a.i2c_fifo.i2c              = PCAL6416A_I2C;

	pcal6416a.i2c_fifo.scl_port         = PCAL6416A_SCL_PORT;
	pcal6416a.i2c_fifo.scl_pin          = PCAL6416A_SCL_PIN;
	pcal6416a.i2c_fifo.scl_mode         = PCAL6416A_SCL_PIN_MODE;
	pcal6416a.i2c_fifo.scl_input        = PCAL6416A_SCL_INPUT;
	pcal6416a.i2c_fifo.scl_source       = PCAL6416A_SCL_SOURCE;
	pcal6416a.i2c_fifo.scl_fifo_size    = PCAL6416A_SCL_FIFO_SIZE;
	pcal6416a.i2c_fifo.scl_fifo_pointer = PCAL6416A_SCL_FIFO_POINTER;

	pcal6416a.i2c_fifo.sda_port         = PCAL6416A_SDA_PORT;
	pcal6416a.i2c_fifo.sda_pin          = PCAL6416A_SDA_PIN;
	pcal6416a.i2c_fifo.sda_mode         = PCAL6416A_SDA_PIN_MODE;
	pcal6416a.i2c_fifo.sda_input        = PCAL6416A_SDA_INPUT;
	pcal6416a.i2c_fifo.sda_source       = PCAL6416A_SDA_SOURCE;
	pcal6416a.i2c_fifo.sda_fifo_size    = PCAL6416A_SDA_FIFO_SIZE;
	pcal6416a.i2c_fifo.sda_fifo_pointer = PCAL6416A_SDA_FIFO_POINTER;

	i2c_fifo_init(&pcal6416a.i2c_fifo);

	const XMC_GPIO_CONFIG_t input_pin_config = {
		.mode             = XMC_GPIO_MODE_INPUT_INVERTED_TRISTATE,
		.input_hysteresis = XMC_GPIO_INPUT_HYSTERESIS_STANDARD,
	};

	const XMC_GPIO_CONFIG_t low_pin_config = {
		.mode             = XMC_GPIO_MODE_OUTPUT_PUSH_PULL,
        .output_level     = XMC_GPIO_OUTPUT_LEVEL_HIGH,
	};

	XMC_GPIO_Init(PCAL6416A_INT_PIN, &input_pin_config);
	XMC_GPIO_Init(PCAL6416A_NRESET_PIN, &low_pin_config);
    system_timer_sleep_ms(2);
    XMC_GPIO_SetOutputHigh(PCAL6416A_NRESET_PIN);
    system_timer_sleep_ms(2);

//    pcal6416a.inout = 0xFFFF;
    pcal6416a.inout = 0x0000; pcal6416a.last_inout = 0xFFFF;
    pcal6416a.pullup_enable = 0xFFFF;
    pcal6416a.output_value = 0x0000; pcal6416a.last_output_value = 0xFFFF;
}

void pcal6416a_tick(void) {
	I2CFifoState state = i2c_fifo_next_state(&pcal6416a.i2c_fifo);

	if(state & I2C_FIFO_STATE_ERROR) {
        loge("PCAL6416A I2C error: %d (%d)\n\r", state, pcal6416a.i2c_fifo.i2c_status);
        pcal6416a_init();
        return;
	}

    switch(state) {
		case I2C_FIFO_STATE_READ_REGISTER_READY: {
			uint8_t data[16];
			uint8_t length = i2c_fifo_read_fifo(&pcal6416a.i2c_fifo, data, 16);

			switch(pcal6416a.state) {
				case PCAL6416A_STATE_READ_INPUT_VALUE: {
					if(length != 2) {
						loge("PCAL6416A unexpected I2C read length: %d\n\r", length);
						pcal6416a_init();
						return;
					}

                    pcal6416a.input_value = data[1] | (data[0] << 8);
					pcal6416a.state = PCAL6416A_STATE_IDLE; 
					break;
				}

				default: pcal6416a.state = PCAL6416A_STATE_IDLE; break;
			}

            break;
        }

        case I2C_FIFO_STATE_WRITE_REGISTER_READY: {
            switch(pcal6416a.state) {
				case PCAL6416A_STATE_WRITE_INOUT:         pcal6416a.state = PCAL6416A_STATE_IDLE; pcal6416a.last_inout = pcal6416a.set_inout; break;
				case PCAL6416A_STATE_WRITE_PULLUP_ENABLE: pcal6416a.state = PCAL6416A_STATE_IDLE; pcal6416a.last_pullup_enable = pcal6416a.set_pullup_enable; break;
				case PCAL6416A_STATE_WRITE_OUTPUT_VALUE:  pcal6416a.state = PCAL6416A_STATE_IDLE; pcal6416a.last_output_value = pcal6416a.set_output_value; break;
				default:                                  pcal6416a.state = PCAL6416A_STATE_IDLE; break;
			}

            break;
        }

        case I2C_FIFO_STATE_IDLE: {
			break; // Handled below
		}

		default: {
			// If we end up in a ready state that we don't handle, something went wrong
			if(state & I2C_FIFO_STATE_READY) {
				loge("PCAL6416A unrecognized I2C ready state: %d\n\r", state);
				pcal6416a_init();
			}

			return;
		}
	}

    if((state == I2C_FIFO_STATE_IDLE || (state & I2C_FIFO_STATE_READY) != 0)) {
        if(pcal6416a.inout != pcal6416a.last_inout) {
            uint8_t data[2] = {(pcal6416a.inout) >> 8 & 0xFF, pcal6416a.inout & 0xFF};
            i2c_fifo_write_register(&pcal6416a.i2c_fifo, PCAL6416A_REG_CONFIGURATION, 2, data, true);

            pcal6416a.set_inout = pcal6416a.inout;
            pcal6416a.state = PCAL6416A_STATE_WRITE_INOUT;
        } else if(pcal6416a.pullup_enable != pcal6416a.last_pullup_enable) {
            uint8_t data[2] = {(pcal6416a.pullup_enable) >> 8 & 0xFF, pcal6416a.pullup_enable & 0xFF};
            i2c_fifo_write_register(&pcal6416a.i2c_fifo, PCAL6416A_REG_PULLUP_ENABLE, 2, data, true);

            pcal6416a.set_pullup_enable = pcal6416a.pullup_enable;
            pcal6416a.state = PCAL6416A_STATE_WRITE_PULLUP_ENABLE;
        } else if(pcal6416a.output_value != pcal6416a.last_output_value) {
            uint8_t data[2] = {(pcal6416a.output_value) >> 8 & 0xFF, pcal6416a.output_value & 0xFF};
            i2c_fifo_write_register(&pcal6416a.i2c_fifo, PCAL6416A_REG_OUTPUT_PORT, 2, data, true);

            pcal6416a.set_output_value = pcal6416a.output_value;
            pcal6416a.state = PCAL6416A_STATE_WRITE_OUTPUT_VALUE;
        }else {
            i2c_fifo_read_register(&pcal6416a.i2c_fifo, PCAL6416A_REG_INPUT_PORT, 2);

            pcal6416a.state = PCAL6416A_STATE_READ_INPUT_VALUE;
        }
    }
}