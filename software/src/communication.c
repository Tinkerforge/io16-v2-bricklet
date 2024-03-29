/* io16-v2-bricklet
 * Copyright (C) 2018 Olaf Lüke <olaf@tinkerforge.com>
 * Copyright (C) 2018 Ishraq Ibne Ashraf <ishraq@tinkerforge.com>
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
#include "bricklib2/hal/system_timer/system_timer.h"
#include "bricklib2/protocols/tfp/tfp.h"
#include "bricklib2/logging/logging.h"

#include "io16.h"

BootloaderHandleMessageResponse handle_message(const void *message, void *response) {
	switch(tfp_get_fid_from_message(message)) {
		case FID_SET_VALUE: return set_value(message);
		case FID_GET_VALUE: return get_value(message, response);
		case FID_SET_SELECTED_VALUE: return set_selected_value(message);
		case FID_SET_CONFIGURATION: return set_configuration(message);
		case FID_GET_CONFIGURATION: return get_configuration(message, response);
		case FID_SET_INPUT_VALUE_CALLBACK_CONFIGURATION: return set_input_value_callback_configuration(message);
		case FID_GET_INPUT_VALUE_CALLBACK_CONFIGURATION: return get_input_value_callback_configuration(message, response);
		case FID_SET_ALL_INPUT_VALUE_CALLBACK_CONFIGURATION: return set_all_input_value_callback_configuration(message);
		case FID_GET_ALL_INPUT_VALUE_CALLBACK_CONFIGURATION: return get_all_input_value_callback_configuration(message, response);
		case FID_SET_MONOFLOP: return set_monoflop(message);
		case FID_GET_MONOFLOP: return get_monoflop(message, response);
		case FID_GET_EDGE_COUNT: return get_edge_count(message, response);
		case FID_SET_EDGE_COUNT_CONFIGURATION: return set_edge_count_configuration(message);
		case FID_GET_EDGE_COUNT_CONFIGURATION: return get_edge_count_configuration(message, response);
		default: return HANDLE_MESSAGE_RESPONSE_NOT_SUPPORTED;
	}
}

BootloaderHandleMessageResponse set_value(const SetValue *data) {
	logd("[+] IO16-V2: set_value()\n\r");

	for(uint8_t i = 0; i < NUMBER_OF_CHANNELS; i++) {
		if(io16.channels[i].direction != IO16_V2_DIRECTION_OUT) {
			// Channel not an output
			continue;
		}

		// Reset monoflop
		io16.channels[i].monoflop.time_start = 0;
		io16.channels[i].monoflop.time_remaining = 0;
		io16.channels[i].monoflop.running = false;

		if(data->value & (1 << i)) {
			io16.channels[i].value = true;
			pcal6416a.output_value |= (1 << i);
		}
		else {
			io16.channels[i].value = false;
			pcal6416a.output_value &= ~(1 << i);
		}
	}

	return HANDLE_MESSAGE_RESPONSE_EMPTY;
}

BootloaderHandleMessageResponse get_value(const GetValue *data,
                                          GetValue_Response *response) {
	logd("[+] IO16-V2: get_value()\n\r");

	uint16_t bit_encoded_values = 0;
	response->header.length = sizeof(GetValue_Response);

	for(uint8_t i = 0; i < NUMBER_OF_CHANNELS; i++) {
		if(io16.channels[i].value) {
			bit_encoded_values |= (1 << i);
		}
		else {
			bit_encoded_values &= ~(1 << i);
		}
	}

	response->value = bit_encoded_values;

	return HANDLE_MESSAGE_RESPONSE_NEW_MESSAGE;
}

BootloaderHandleMessageResponse set_selected_value(const SetSelectedValue *data) {
	logd("[+] IO16-V2: set_selected_value()\n\r");

	if(data->channel > NUMBER_OF_CHANNELS - 1) {
		return HANDLE_MESSAGE_RESPONSE_INVALID_PARAMETER;
	}

	if(io16.channels[data->channel].direction != IO16_V2_DIRECTION_OUT) {
		return HANDLE_MESSAGE_RESPONSE_EMPTY;
	}

	io16.channels[data->channel].value = data->value;

	// Reset monoflop
	io16.channels[data->channel].monoflop.time_start = 0;
	io16.channels[data->channel].monoflop.time_remaining = 0;
	io16.channels[data->channel].monoflop.running = false;

	if(io16.channels[data->channel].value) {
		pcal6416a.output_value |= (1 << data->channel);
	}
	else {
		pcal6416a.output_value &= ~(1 << data->channel);
	}

	return HANDLE_MESSAGE_RESPONSE_EMPTY;
}

BootloaderHandleMessageResponse set_configuration(const SetConfiguration *data) {
	logd("[+] IO16-V2: set_configuration()\n\r");

	char channel_previous_direction;

	if(data->channel > NUMBER_OF_CHANNELS - 1) {
		return HANDLE_MESSAGE_RESPONSE_INVALID_PARAMETER;
	}

	if(data->direction == IO16_V2_DIRECTION_IN) {
		channel_previous_direction = io16.channels[data->channel].direction;
		io16.channels[data->channel].init_value = data->value;
		io16.channels[data->channel].direction = IO16_V2_DIRECTION_IN;

		pcal6416a.inout |= (1 << data->channel);

		if(io16.channels[data->channel].init_value) {
			pcal6416a.pullup_enable |= (1 << data->channel);
		}
		else {
			pcal6416a.pullup_enable &= ~(1 << data->channel);
		}

		if(channel_previous_direction == IO16_V2_DIRECTION_OUT) {
			// Reset and start edge counter
			io16.channels[data->channel].edge_count.debounce = 100;
			io16.channels[data->channel].edge_count.cnt_edge_rising = 0;
			io16.channels[data->channel].edge_count.cnt_edge_falling = 0;
			io16.channels[data->channel].edge_count.debounce_start = system_timer_get_ms();
		}

		// Reset monoflop
		io16.channels[data->channel].monoflop.time_start = 0;
		io16.channels[data->channel].monoflop.time_remaining = 0;
		io16.channels[data->channel].monoflop.running = false;
	}
	else if(data->direction == IO16_V2_DIRECTION_OUT) {
		io16.channels[data->channel].init_value = data->value;
		io16.channels[data->channel].direction = IO16_V2_DIRECTION_OUT;
		io16.channels[data->channel].value = io16.channels[data->channel].init_value;

		pcal6416a.inout &= ~(1 << data->channel);

		if(io16.channels[data->channel].init_value) {
			pcal6416a.output_value |= (1 << data->channel);
		}
		else {
			pcal6416a.output_value &= ~(1 << data->channel);
		}

		// Reset and stop edge counter
		io16.channels[data->channel].edge_count.debounce = 0;
		io16.channels[data->channel].edge_count.debounce_start = 0;
		io16.channels[data->channel].edge_count.cnt_edge_rising = 0;
		io16.channels[data->channel].edge_count.cnt_edge_falling = 0;

		// Reset monoflop
		io16.channels[data->channel].monoflop.time_start = 0;
		io16.channels[data->channel].monoflop.time_remaining = 0;
		io16.channels[data->channel].monoflop.running = false;
	}
	else {
		return HANDLE_MESSAGE_RESPONSE_INVALID_PARAMETER;
	}

	return HANDLE_MESSAGE_RESPONSE_EMPTY;
}

BootloaderHandleMessageResponse get_configuration(const GetConfiguration *data, GetConfiguration_Response *response) {
	logd("[+] IO16-V2: get_configuration()\n\r");

	response->header.length = sizeof(GetConfiguration_Response);

	if(data->channel > NUMBER_OF_CHANNELS - 1) {
		return HANDLE_MESSAGE_RESPONSE_INVALID_PARAMETER;
	}

	response->value = io16.channels[data->channel].init_value;
	response->direction = io16.channels[data->channel].direction;

	return HANDLE_MESSAGE_RESPONSE_NEW_MESSAGE;
}

BootloaderHandleMessageResponse set_input_value_callback_configuration(const SetInputValueCallbackConfiguration *data) {
	logd("[+] IO16-V2: set_input_value_callback_configuration()\n\r");

	if(data->channel > NUMBER_OF_CHANNELS - 1) {
		return HANDLE_MESSAGE_RESPONSE_INVALID_PARAMETER;
	}

	if(io16.channels[data->channel].direction != IO16_V2_DIRECTION_IN) {
		return HANDLE_MESSAGE_RESPONSE_INVALID_PARAMETER;
	}

	io16.channels[data->channel].input_value_cb.period_start = 0;
	io16.channels[data->channel].input_value_cb.period = data->period;
	io16.channels[data->channel].input_value_cb.value_has_to_change = data->value_has_to_change;

	if(io16.channels[data->channel].input_value_cb.period > 0) {
		(pcal6416a.input_value & (1 << data->channel)) ? (io16.channels[data->channel].input_value_cb.last_value = true) : (io16.channels[data->channel].input_value_cb.last_value = false);
		io16.channels[data->channel].input_value_cb.period_start = system_timer_get_ms();
	}

	return HANDLE_MESSAGE_RESPONSE_EMPTY;
}

BootloaderHandleMessageResponse get_input_value_callback_configuration(const GetInputValueCallbackConfiguration *data,
                                                                       GetInputValueCallbackConfiguration_Response *response) {
	logd("[+] IO16-V2: get_input_value_callback_configuration()\n\r");

	response->header.length = sizeof(GetInputValueCallbackConfiguration_Response);

	if(data->channel > NUMBER_OF_CHANNELS - 1) {
		return HANDLE_MESSAGE_RESPONSE_INVALID_PARAMETER;
	}

	response->period = io16.channels[data->channel].input_value_cb.period;
	response->value_has_to_change = io16.channels[data->channel].input_value_cb.value_has_to_change;

	return HANDLE_MESSAGE_RESPONSE_NEW_MESSAGE;
}

BootloaderHandleMessageResponse set_all_input_value_callback_configuration(const SetAllInputValueCallbackConfiguration *data) {
	logd("[+] IO16-V2: set_all_input_value_callback_configuration()\n\r");

	for(uint8_t i = 0; i < NUMBER_OF_CHANNELS; i++) {
		if(io16.channels[i].direction != IO16_V2_DIRECTION_IN) {
			return HANDLE_MESSAGE_RESPONSE_INVALID_PARAMETER;
		}
	}

	io16.all_input_value_cb.period_start = 0;
	io16.all_input_value_cb.period = data->period;
	io16.all_input_value_cb.value_has_to_change = data->value_has_to_change;

	if(io16.all_input_value_cb.period > 0) {
		// Store current channel states
		for(uint8_t i = 0; i < NUMBER_OF_CHANNELS; i++) {
			(pcal6416a.input_value & (1 << i)) ? (io16.all_input_value_cb.last_values[i] = true) : (io16.all_input_value_cb.last_values[i] = false);
		}

		io16.all_input_value_cb.period_start = system_timer_get_ms();
	}

	return HANDLE_MESSAGE_RESPONSE_EMPTY;
}

BootloaderHandleMessageResponse get_all_input_value_callback_configuration(const GetAllInputValueCallbackConfiguration *data,
                                                                           GetAllInputValueCallbackConfiguration_Response *response) {
	logd("[+] IO16-V2: get_all_input_value_callback_configuration()\n\r");

	response->header.length = sizeof(GetInputValueCallbackConfiguration_Response);
	response->period = io16.all_input_value_cb.period;
	response->value_has_to_change = io16.all_input_value_cb.value_has_to_change;

	return HANDLE_MESSAGE_RESPONSE_NEW_MESSAGE;
}

BootloaderHandleMessageResponse set_monoflop(const SetMonoflop *data) {
	logd("[+] IO16-V2: set_monoflop()\n\r");

	if(data->channel > NUMBER_OF_CHANNELS - 1) {
		return HANDLE_MESSAGE_RESPONSE_INVALID_PARAMETER;
	}

	if(io16.channels[data->channel].direction != IO16_V2_DIRECTION_OUT) {
		return HANDLE_MESSAGE_RESPONSE_INVALID_PARAMETER;
	}

	io16.channels[data->channel].value = data->value;
	io16.channels[data->channel].monoflop.time = data->time;
	io16.channels[data->channel].monoflop.time_remaining = data->time;
	io16.channels[data->channel].monoflop.running = true;

	if(io16.channels[data->channel].value) {
		pcal6416a.output_value |= (1 << data->channel);
	}
	else {
		pcal6416a.output_value &= ~(1 << data->channel);
	}

	io16.channels[data->channel].monoflop.time_start = system_timer_get_ms();

	return HANDLE_MESSAGE_RESPONSE_EMPTY;
}

BootloaderHandleMessageResponse get_monoflop(const GetMonoflop *data,
                                             GetMonoflop_Response *response) {
	logd("[+] IO16-V2: get_monoflop()\n\r");

	response->header.length = sizeof(GetMonoflop_Response);

	if(data->channel > NUMBER_OF_CHANNELS - 1) {
		return HANDLE_MESSAGE_RESPONSE_INVALID_PARAMETER;
	}

	if(io16.channels[data->channel].direction != IO16_V2_DIRECTION_OUT) {
		return HANDLE_MESSAGE_RESPONSE_INVALID_PARAMETER;
	}

	response->time = io16.channels[data->channel].monoflop.time;
	response->value = io16.channels[data->channel].value;
	response->time_remaining = io16.channels[data->channel].monoflop.time_remaining;

	return HANDLE_MESSAGE_RESPONSE_NEW_MESSAGE;
}

BootloaderHandleMessageResponse get_edge_count(const GetEdgeCount *data, GetEdgeCount_Response *response) {
	logd("[+] IO16-V2: get_edge_count()\n\r");

	response->header.length = sizeof(GetEdgeCount_Response);

	if(data->channel > NUMBER_OF_CHANNELS - 1) {
		return HANDLE_MESSAGE_RESPONSE_INVALID_PARAMETER;
	}

	if(io16.channels[data->channel].direction != IO16_V2_DIRECTION_IN) {
		return HANDLE_MESSAGE_RESPONSE_INVALID_PARAMETER;
	}

	if(io16.channels[data->channel].edge_count.edge_type == IO16_V2_EDGE_TYPE_RISING) {
		response->count = io16.channels[data->channel].edge_count.cnt_edge_rising;
	}
	else if(io16.channels[data->channel].edge_count.edge_type == IO16_V2_EDGE_TYPE_FALLING) {
		response->count = io16.channels[data->channel].edge_count.cnt_edge_falling;
	}
	else if(io16.channels[data->channel].edge_count.edge_type == IO16_V2_EDGE_TYPE_BOTH) {
		response->count = \
			io16.channels[data->channel].edge_count.cnt_edge_rising + \
			io16.channels[data->channel].edge_count.cnt_edge_falling;
	}

	if(data->reset_counter) {
		io16.channels[data->channel].edge_count.cnt_edge_rising = 0;
		io16.channels[data->channel].edge_count.cnt_edge_falling = 0;
	}

	return HANDLE_MESSAGE_RESPONSE_NEW_MESSAGE;
}

BootloaderHandleMessageResponse set_edge_count_configuration(const SetEdgeCountConfiguration *data) {
	logd("[+] IO16-V2: set_edge_count_configuration()\n\r");

	if(data->channel > NUMBER_OF_CHANNELS - 1) {
		return HANDLE_MESSAGE_RESPONSE_INVALID_PARAMETER;
	}

	if(io16.channels[data->channel].direction != IO16_V2_DIRECTION_IN) {
		return HANDLE_MESSAGE_RESPONSE_INVALID_PARAMETER;
	}

	io16.channels[data->channel].edge_count.cnt_edge_rising = 0;
	io16.channels[data->channel].edge_count.cnt_edge_falling = 0;
	io16.channels[data->channel].edge_count.debounce = data->debounce;
	io16.channels[data->channel].edge_count.edge_type = data->edge_type;
	(pcal6416a.input_value & (1 << data->channel)) ? (io16.channels[data->channel].edge_count.last_value = true) : (io16.channels[data->channel].edge_count.last_value = false);
	io16.channels[data->channel].edge_count.debounce_start = system_timer_get_ms();

	return HANDLE_MESSAGE_RESPONSE_EMPTY;
}

BootloaderHandleMessageResponse get_edge_count_configuration(const GetEdgeCountConfiguration *data,
                                                             GetEdgeCountConfiguration_Response *response) {
	logd("[+] IO16-V2: get_edge_count_configuration()\n\r");

	response->header.length = sizeof(GetEdgeCountConfiguration_Response);

	if(data->channel > NUMBER_OF_CHANNELS - 1) {
		return HANDLE_MESSAGE_RESPONSE_INVALID_PARAMETER;
	}

	if(io16.channels[data->channel].direction != IO16_V2_DIRECTION_IN) {
		return HANDLE_MESSAGE_RESPONSE_INVALID_PARAMETER;
	}

	response->debounce = io16.channels[data->channel].edge_count.debounce;
	response->edge_type = io16.channels[data->channel].edge_count.edge_type;

	return HANDLE_MESSAGE_RESPONSE_NEW_MESSAGE;
}

bool handle_input_value_callback(void) {
	static bool is_buffered = false;
	static InputValue_Callback cb;

	if(!is_buffered) {
		if(ringbuffer_is_empty(&io16.input_value_cb_rb)) {
			// Nothing to send
			return false;
		}

		tfp_make_default_header(&cb.header,
		                        bootloader_get_uid(),
		                        sizeof(InputValue_Callback),
		                        FID_CALLBACK_INPUT_VALUE);

		ringbuffer_get(&io16.input_value_cb_rb, &cb.channel);
		ringbuffer_get(&io16.input_value_cb_rb, (uint8_t *)&cb.changed);
		ringbuffer_get(&io16.input_value_cb_rb, (uint8_t *)&cb.value);
	}

	if(bootloader_spitfp_is_send_possible(&bootloader_status.st)) {
		bootloader_spitfp_send_ack_and_message(&bootloader_status,
		                                       (uint8_t*)&cb,
		                                       sizeof(InputValue_Callback));
		is_buffered = false;

		return true;
	}
	else {
		is_buffered = true;
	}

	return false;
}

bool handle_all_input_value_callback(void) {
	static bool is_buffered = false;
	static AllInputValue_Callback cb;
	uint8_t rb_get_tmp_0 = 0;
	uint8_t rb_get_tmp_1 = 0;

	if(!is_buffered) {
		if(ringbuffer_is_empty(&io16.all_input_value_cb.cb_rb)) {
			// Nothing to send
			return false;
		}

		tfp_make_default_header(&cb.header,
		                        bootloader_get_uid(),
		                        sizeof(AllInputValue_Callback),
		                        FID_CALLBACK_ALL_INPUT_VALUE);

		ringbuffer_get(&io16.all_input_value_cb.cb_rb, &rb_get_tmp_0);
		ringbuffer_get(&io16.all_input_value_cb.cb_rb, &rb_get_tmp_1);
		cb.changed = (rb_get_tmp_1 << 8) | rb_get_tmp_0;

		ringbuffer_get(&io16.all_input_value_cb.cb_rb, &rb_get_tmp_0);
		ringbuffer_get(&io16.all_input_value_cb.cb_rb, &rb_get_tmp_1);
		cb.value = (rb_get_tmp_1 << 8) | rb_get_tmp_0;
	}

	if(bootloader_spitfp_is_send_possible(&bootloader_status.st)) {
		bootloader_spitfp_send_ack_and_message(&bootloader_status,
		                                       (uint8_t*)&cb,
		                                       sizeof(AllInputValue_Callback));

		is_buffered = false;

		return true;
	}
	else {
		is_buffered = true;
	}

	return false;
}

bool handle_monoflop_done_callback(void) {
	static bool is_buffered = false;
	static MonoflopDone_Callback cb;

	if(!is_buffered) {
		if(ringbuffer_is_empty(&io16.monoflop_cb_rb)) {
			// Nothing to send
			return false;
		}

		tfp_make_default_header(&cb.header,
		                        bootloader_get_uid(),
		                        sizeof(MonoflopDone_Callback),
		                        FID_CALLBACK_MONOFLOP_DONE);

		ringbuffer_get(&io16.monoflop_cb_rb, &cb.channel);
		ringbuffer_get(&io16.monoflop_cb_rb, (uint8_t *)&cb.value);
	}

	if(bootloader_spitfp_is_send_possible(&bootloader_status.st)) {
		bootloader_spitfp_send_ack_and_message(&bootloader_status,
		                                       (uint8_t*)&cb,
		                                       sizeof(MonoflopDone_Callback));

		is_buffered = false;

		return true;
	}
	else {
		is_buffered = true;
	}

	return false;
}

void communication_tick(void) {
	communication_callback_tick();
}

void communication_init(void) {
	communication_callback_init();
}
