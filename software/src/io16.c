/* io16-v2-bricklet
 * Copyright (C) 2018 Ishraq Ibne Ashraf <ishraq@tinkerforge.com>
 *
 * io16.c: Implementation of IO16 V2 Bricklet
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

#include "io16.h"

#include <stdint.h>
#include <stdbool.h>

#include "bricklib2/logging/logging.h"
#include "bricklib2/hal/system_timer/system_timer.h"

#include "communication.h"

IO16 io16;

void io16_init(void) {
	logd("[+] IO16-V2: io16_init()\n\r");

	// Initialise all the channels as input pull-up
	pcal6416a.inout = 0xFFFF;
	pcal6416a.pullup_enable = 0xFFFF;

	pcal6416a_tick();

	// Initialise all the channels
	for(uint8_t i = 0; i < NUMBER_OF_CHANNELS; i++) {
		// Update current channel value
		(pcal6416a.input_value & (1 << i)) ? (io16.channels[i].value = true) : (io16.channels[i].value = false);

		// Generic channel config
		io16.channels[i].direction = IO16_V2_DIRECTION_IN;

		// Channel monoflop config
		io16.channels[i].monoflop.time = 0;
		io16.channels[i].monoflop.time_start = 0;
		io16.channels[i].monoflop.time_remaining = 0;

		// Channel edge count config
		io16.channels[i].edge_count.debounce = 100;
		io16.channels[i].edge_count.cnt_edge_rising = 0;
		io16.channels[i].edge_count.cnt_edge_falling = 0;
		io16.channels[i].edge_count.edge_type = IO16_V2_EDGE_TYPE_RISING;

		// Channel input value callback config
		io16.channels[i].input_value_cb.period = 0;
		io16.channels[i].input_value_cb.last_value = false;
		io16.channels[i].input_value_cb.period_start = 0;
		io16.channels[i].input_value_cb.value_has_to_change = false;

		// Channel edge count config
		io16.channels[i].edge_count.last_value = io16.channels[i].value;
		io16.channels[i].edge_count.debounce_start = system_timer_get_ms();

		// Initialised as pull-up input
		io16.channels[i].init_value = true;
	}

	// All input value callback
	io16.all_input_value_cb.period = 0;
	io16.all_input_value_cb.period_start = 0;
	io16.all_input_value_cb.value_has_to_change = false;

	// Input value callback ringbuffer init
	ringbuffer_init(&io16.input_value_cb_rb, INPUT_VALUE_CB_BUFFER_SIZE, &io16.input_value_cb_buffer[0]);
	ringbuffer_init(&io16.all_input_value_cb.cb_rb, ALL_INPUT_VALUE_CB_BUFFER_SIZE, &io16.all_input_value_cb.cb_buffer[0]);

	// Monopflop callback ringbuffer init
	ringbuffer_init(&io16.monoflop_cb_rb, MONOFLOP_CB_BUFFER_SIZE, &io16.monoflop_cb_buffer[0]);
}

void io16_tick(void) {
	uint8_t all_channel_values[2] = {0, 0};
	uint8_t all_channel_changed[2] = {0, 0};
	bool all_ch_in_value_changed = false;
	bool all_ch_in_period_expired = false;

	// Check if all channel input value callback is enabled and if period expired
	if((io16.all_input_value_cb.period > 0) && (system_timer_is_time_elapsed_ms(io16.all_input_value_cb.period_start, io16.all_input_value_cb.period))) {
		all_ch_in_period_expired = true;
	}

	// Iterate all channels
	for(uint8_t i = 0; i < NUMBER_OF_CHANNELS; i++) {
		if(io16.channels[i].direction == IO16_V2_DIRECTION_IN) {
			// Channel is input

			// Update current input channel value
			(pcal6416a.input_value & (1 << i)) ? (io16.channels[i].value = true) : (io16.channels[i].value = false);

			// Manage channel specific input value callback
			if((io16.channels[i].input_value_cb.period > 0)) {
				if(system_timer_is_time_elapsed_ms(io16.channels[i].input_value_cb.period_start, io16.channels[i].input_value_cb.period)) {
					// Period expired

					if(io16.channels[i].input_value_cb.value_has_to_change) {
						// Enqueue callback if value changed otherwise not
						if(io16.channels[i].value != io16.channels[i].input_value_cb.last_value) {
							if(ringbuffer_get_used(&io16.input_value_cb_rb) < INPUT_VALUE_CB_BUFFER_SIZE) {
								ringbuffer_add(&io16.input_value_cb_rb, i); // Channel
								ringbuffer_add(&io16.input_value_cb_rb, (uint8_t)true); // Changed
								ringbuffer_add(&io16.input_value_cb_rb, (uint8_t)io16.channels[i].value); // Value
							}
						}
					}
					else {
						// Enqueue callback regardless of change
						if(ringbuffer_get_used(&io16.input_value_cb_rb) < INPUT_VALUE_CB_BUFFER_SIZE) {
							// Channel
							ringbuffer_add(&io16.input_value_cb_rb, i);

							// Changed
							if(io16.channels[i].value != io16.channels[i].input_value_cb.last_value) {
								ringbuffer_add(&io16.input_value_cb_rb, (uint8_t)true);
							}
							else {
								ringbuffer_add(&io16.input_value_cb_rb, (uint8_t)false);
							}

							// Value
							ringbuffer_add(&io16.input_value_cb_rb, (uint8_t)io16.channels[i].value);
						}
					}

					// Update last value
					io16.channels[i].input_value_cb.last_value = io16.channels[i].value;
					io16.channels[i].input_value_cb.period_start = system_timer_get_ms();
				}
			}

			// Manage all input value callback
			if(all_ch_in_period_expired) {
				if(io16.channels[i].value != io16.all_input_value_cb.last_values[i]) {
					all_ch_in_value_changed = true;

					if(i < 8) {
						all_channel_changed[0] |= (1 << i);
					}
					else {
						all_channel_changed[1] |= (1 << i);
					}
				}
				else {
					if(i < 8) {
						all_channel_changed[0] &= ~(1 << i);
					}
					else {
						all_channel_changed[1] &= ~(1 << i);
					}
				}

				if(io16.channels[i].value) {
					if(i < 8) {
						all_channel_values[0] |= (1 << i);
					}
					else {
						all_channel_values[1] |= (1 << i);
					}
				}
				else {
					if(i < 8) {
						all_channel_values[0] &= ~(1 << i);
					}
					else {
						all_channel_values[1] &= ~(1 << i);
					}
				}

				// Update last value
				io16.all_input_value_cb.last_values[i] = io16.channels[i].value;
			}

			// Manage edge count
			if(system_timer_is_time_elapsed_ms(io16.channels[i].edge_count.debounce_start, io16.channels[i].edge_count.debounce)) {
				// Debounce period expired, update edge count

				if(!io16.channels[i].edge_count.last_value && io16.channels[i].value) {
					// Rising edge
					io16.channels[i].edge_count.cnt_edge_rising++;
				}
				else if(io16.channels[i].edge_count.last_value && !io16.channels[i].value) {
					// Falling edge
					io16.channels[i].edge_count.cnt_edge_falling++;
				}

				// Update last value
				io16.channels[i].edge_count.last_value = io16.channels[i].value;
				io16.channels[i].edge_count.debounce_start = system_timer_get_ms();
			}
		}
		else {
			// Channel is output

			// Manage monoflop
			if(io16.channels[i].monoflop.time > 0) {
				if(system_timer_is_time_elapsed_ms(io16.channels[i].monoflop.time_start, io16.channels[i].monoflop.time)) {
					// Monoflop time expired

					io16.channels[i].monoflop.time = 0;
					io16.channels[i].monoflop.time_start = 0;
					io16.channels[i].monoflop.time_remaining = 0;

					if(io16.channels[i].value) {
						io16.channels[i].value = false;
					}
					else {
						io16.channels[i].value = true;
					}

					if(io16.channels[i].value) {
						pcal6416a.output_value |= (1 << i);
					}
					else {
						pcal6416a.output_value &= ~(1 << i);
					}

					// Enqueue monoflop callback for the channel
					if(ringbuffer_get_used(&io16.monoflop_cb_rb) < MONOFLOP_CB_BUFFER_SIZE) {
						// Channel
						ringbuffer_add(&io16.monoflop_cb_rb, i);
						// Value
						ringbuffer_add(&io16.monoflop_cb_rb, (uint8_t)io16.channels[i].value);
					}
				}
				else {
					io16.channels[i].monoflop.time_remaining = \
						io16.channels[i].monoflop.time - (system_timer_get_ms() - io16.channels[i].monoflop.time_start);
				}
			}
		}
	}

	// Manage all input value callback
	if(all_ch_in_period_expired) {
		// Period expired

		if(io16.all_input_value_cb.value_has_to_change) {
			// Enqueue CB if value changed otherwise not
			if(all_ch_in_value_changed) {
				if(ringbuffer_get_used(&io16.all_input_value_cb.cb_rb) < ALL_INPUT_VALUE_CB_BUFFER_SIZE) {
					// Changed
					ringbuffer_add(&io16.all_input_value_cb.cb_rb, all_channel_changed[0]);
					ringbuffer_add(&io16.all_input_value_cb.cb_rb, all_channel_changed[1]);
					// Value
					ringbuffer_add(&io16.all_input_value_cb.cb_rb, all_channel_values[0]);
					ringbuffer_add(&io16.all_input_value_cb.cb_rb, all_channel_values[1]);
				}
			}
		}
		else {
			// Enqueue CB regardless of change
			if(ringbuffer_get_used(&io16.all_input_value_cb.cb_rb) < ALL_INPUT_VALUE_CB_BUFFER_SIZE) {
				// Changed
				ringbuffer_add(&io16.all_input_value_cb.cb_rb, all_channel_changed[0]);
				ringbuffer_add(&io16.all_input_value_cb.cb_rb, all_channel_changed[1]);
				// Value
				ringbuffer_add(&io16.all_input_value_cb.cb_rb, all_channel_values[0]);
				ringbuffer_add(&io16.all_input_value_cb.cb_rb, all_channel_values[1]);
			}
		}

		io16.all_input_value_cb.period_start = system_timer_get_ms();
	}
}
