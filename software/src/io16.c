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

	memset(&io16, 0, sizeof(IO16));

	// Initialise all the channels
	// The commented out code is already initialized by the memset above (we need to save the flash space).
	for(uint8_t i = 0; i < NUMBER_OF_CHANNELS; i++) {
		IO_CH_t *channel = &io16.channels[i];

		// Update current channel value
		channel->value = pcal6416a.input_value & (1 << i);

		// Generic channel config
		channel->direction = IO16_V2_DIRECTION_IN;

		// Channel monoflop config
//		channel->monoflop.time = 0;
//		channel->monoflop.time_start = 0;
//		channel->monoflop.time_remaining = 0;

		// Channel edge count config
		channel->edge_count.debounce = 100;
//		channel->edge_count.cnt_edge_rising = 0;
//		channel->edge_count.cnt_edge_falling = 0;
//		channel->edge_count.edge_type = IO16_V2_EDGE_TYPE_RISING;

		// Channel input value callback config
//		channel->input_value_cb.period = 0;
//		channel->input_value_cb.last_value = false;
//		channel->input_value_cb.period_start = 0;
//		channel->input_value_cb.value_has_to_change = false;

		// Channel edge count config
		channel->edge_count.last_value = channel->value;
		channel->edge_count.debounce_start = system_timer_get_ms();

		// Initialised as pull-up input
		channel->init_value = true;
	}

	// All input value callback
//	io16.all_input_value_cb.period = 0;
//	io16.all_input_value_cb.period_start = 0;
//	io16.all_input_value_cb.value_has_to_change = false;

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
		IO_CH_t *channel = &io16.channels[i];
		if(channel->direction == IO16_V2_DIRECTION_IN) {
			// Channel is input

			// Update current input channel value
			channel->value = pcal6416a.input_value & (1 << i);

			// Manage channel specific input value callback
			if((channel->input_value_cb.period > 0)) {
				if(system_timer_is_time_elapsed_ms(channel->input_value_cb.period_start, channel->input_value_cb.period)) {
					// Period expired

					if(channel->input_value_cb.value_has_to_change) {
						// Enqueue callback if value changed otherwise not
						if(channel->value != channel->input_value_cb.last_value) {
							if(ringbuffer_get_used(&io16.input_value_cb_rb) < INPUT_VALUE_CB_BUFFER_SIZE) {
								ringbuffer_add(&io16.input_value_cb_rb, i); // Channel
								ringbuffer_add(&io16.input_value_cb_rb, (uint8_t)true); // Changed
								ringbuffer_add(&io16.input_value_cb_rb, (uint8_t)channel->value); // Value
							}
						}
					}
					else {
						// Enqueue callback regardless of change
						if(ringbuffer_get_used(&io16.input_value_cb_rb) < INPUT_VALUE_CB_BUFFER_SIZE) {
							// Channel
							ringbuffer_add(&io16.input_value_cb_rb, i);

							// Changed
							if(channel->value != channel->input_value_cb.last_value) {
								ringbuffer_add(&io16.input_value_cb_rb, (uint8_t)true);
							}
							else {
								ringbuffer_add(&io16.input_value_cb_rb, (uint8_t)false);
							}

							// Value
							ringbuffer_add(&io16.input_value_cb_rb, (uint8_t)channel->value);
						}
					}

					// Update last value
					channel->input_value_cb.last_value = channel->value;
					channel->input_value_cb.period_start = system_timer_get_ms();
				}
			}

			// Manage all input value callback
			if(all_ch_in_period_expired) {
				if(channel->value != io16.all_input_value_cb.last_values[i]) {
					all_ch_in_value_changed = true;

					if(i < 8) {
						all_channel_changed[0] |= (1 << i);
					}
					else {
						all_channel_changed[1] |= (1 << (i - 8));
					}
				}

				if(channel->value) {
					if(i < 8) {
						all_channel_values[0] |= (1 << i);
					}
					else {
						all_channel_values[1] |= (1 << (i - 8));
					}
				}
				else {
					if(i < 8) {
						all_channel_values[0] &= ~(1 << i);
					}
					else {
						all_channel_values[1] &= ~(1 << (i - 8));
					}
				}

				// Update last value
				io16.all_input_value_cb.last_values[i] = channel->value;
			}

			// Manage edge count
			if(system_timer_is_time_elapsed_ms(channel->edge_count.debounce_start, channel->edge_count.debounce)) {
				// Debounce period expired, update edge count

				if(!channel->edge_count.last_value && channel->value) {
					// Rising edge
					channel->edge_count.cnt_edge_rising++;
				}
				else if(channel->edge_count.last_value && !channel->value) {
					// Falling edge
					channel->edge_count.cnt_edge_falling++;
				}

				// Update last value
				channel->edge_count.last_value = channel->value;
				channel->edge_count.debounce_start = system_timer_get_ms();
			}
		}
		else {
			// Channel is output

			// Manage monoflop
			if(channel->monoflop.time > 0) {
				if(system_timer_is_time_elapsed_ms(channel->monoflop.time_start, channel->monoflop.time)) {
					// Monoflop time expired

					channel->monoflop.time = 0;
					channel->monoflop.time_start = 0;
					channel->monoflop.time_remaining = 0;

					if(channel->value) {
						channel->value = false;
					}
					else {
						channel->value = true;
					}

					if(channel->value) {
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
						ringbuffer_add(&io16.monoflop_cb_rb, (uint8_t)channel->value);
					}
				}
				else {
					channel->monoflop.time_remaining = \
						channel->monoflop.time - (system_timer_get_ms() - channel->monoflop.time_start);
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
