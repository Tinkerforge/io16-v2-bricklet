/* io16-v2-bricklet
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

BootloaderHandleMessageResponse handle_message(const void *message, void *response) {
	switch(tfp_get_fid_from_message(message)) {
		case FID_SET_VALUE: return set_value(message);
		case FID_GET_VALUE: return get_value(message, response);
		case FID_SET_SELECTED_VALUE: return set_selected_value(message);
		case FID_GET_SELECTED_VALUE: return get_selected_value(message, response);
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

	return HANDLE_MESSAGE_RESPONSE_EMPTY;
}

BootloaderHandleMessageResponse get_value(const GetValue *data, GetValue_Response *response) {
	response->header.length = sizeof(GetValue_Response);

	return HANDLE_MESSAGE_RESPONSE_NEW_MESSAGE;
}

BootloaderHandleMessageResponse set_selected_value(const SetSelectedValue *data) {

	return HANDLE_MESSAGE_RESPONSE_EMPTY;
}

BootloaderHandleMessageResponse get_selected_value(const GetSelectedValue *data, GetSelectedValue_Response *response) {
	response->header.length = sizeof(GetSelectedValue_Response);

	return HANDLE_MESSAGE_RESPONSE_NEW_MESSAGE;
}

BootloaderHandleMessageResponse set_configuration(const SetConfiguration *data) {

	return HANDLE_MESSAGE_RESPONSE_EMPTY;
}

BootloaderHandleMessageResponse get_configuration(const GetConfiguration *data, GetConfiguration_Response *response) {
	response->header.length = sizeof(GetConfiguration_Response);

	return HANDLE_MESSAGE_RESPONSE_NEW_MESSAGE;
}

BootloaderHandleMessageResponse set_input_value_callback_configuration(const SetInputValueCallbackConfiguration *data) {

	return HANDLE_MESSAGE_RESPONSE_EMPTY;
}

BootloaderHandleMessageResponse get_input_value_callback_configuration(const GetInputValueCallbackConfiguration *data, GetInputValueCallbackConfiguration_Response *response) {
	response->header.length = sizeof(GetInputValueCallbackConfiguration_Response);

	return HANDLE_MESSAGE_RESPONSE_NEW_MESSAGE;
}

BootloaderHandleMessageResponse set_all_input_value_callback_configuration(const SetAllInputValueCallbackConfiguration *data) {

	return HANDLE_MESSAGE_RESPONSE_EMPTY;
}

BootloaderHandleMessageResponse get_all_input_value_callback_configuration(const GetAllInputValueCallbackConfiguration *data, GetAllInputValueCallbackConfiguration_Response *response) {
	response->header.length = sizeof(GetAllInputValueCallbackConfiguration_Response);

	return HANDLE_MESSAGE_RESPONSE_NEW_MESSAGE;
}

BootloaderHandleMessageResponse set_monoflop(const SetMonoflop *data) {

	return HANDLE_MESSAGE_RESPONSE_EMPTY;
}

BootloaderHandleMessageResponse get_monoflop(const GetMonoflop *data, GetMonoflop_Response *response) {
	response->header.length = sizeof(GetMonoflop_Response);

	return HANDLE_MESSAGE_RESPONSE_NEW_MESSAGE;
}

BootloaderHandleMessageResponse get_edge_count(const GetEdgeCount *data, GetEdgeCount_Response *response) {
	response->header.length = sizeof(GetEdgeCount_Response);

	return HANDLE_MESSAGE_RESPONSE_NEW_MESSAGE;
}

BootloaderHandleMessageResponse set_edge_count_configuration(const SetEdgeCountConfiguration *data) {

	return HANDLE_MESSAGE_RESPONSE_EMPTY;
}

BootloaderHandleMessageResponse get_edge_count_configuration(const GetEdgeCountConfiguration *data, GetEdgeCountConfiguration_Response *response) {
	response->header.length = sizeof(GetEdgeCountConfiguration_Response);

	return HANDLE_MESSAGE_RESPONSE_NEW_MESSAGE;
}




bool handle_input_value_callback(void) {
	static bool is_buffered = false;
	static InputValue_Callback cb;

	if(!is_buffered) {
		tfp_make_default_header(&cb.header, bootloader_get_uid(), sizeof(InputValue_Callback), FID_CALLBACK_INPUT_VALUE);
		// TODO: Implement InputValue callback handling

		return false;
	}

	if(bootloader_spitfp_is_send_possible(&bootloader_status.st)) {
		bootloader_spitfp_send_ack_and_message(&bootloader_status, (uint8_t*)&cb, sizeof(InputValue_Callback));
		is_buffered = false;
		return true;
	} else {
		is_buffered = true;
	}

	return false;
}

bool handle_all_input_value_callback(void) {
	static bool is_buffered = false;
	static AllInputValue_Callback cb;

	if(!is_buffered) {
		tfp_make_default_header(&cb.header, bootloader_get_uid(), sizeof(AllInputValue_Callback), FID_CALLBACK_ALL_INPUT_VALUE);
		// TODO: Implement AllInputValue callback handling

		return false;
	}

	if(bootloader_spitfp_is_send_possible(&bootloader_status.st)) {
		bootloader_spitfp_send_ack_and_message(&bootloader_status, (uint8_t*)&cb, sizeof(AllInputValue_Callback));
		is_buffered = false;
		return true;
	} else {
		is_buffered = true;
	}

	return false;
}

bool handle_monoflop_done_callback(void) {
	static bool is_buffered = false;
	static MonoflopDone_Callback cb;

	if(!is_buffered) {
		tfp_make_default_header(&cb.header, bootloader_get_uid(), sizeof(MonoflopDone_Callback), FID_CALLBACK_MONOFLOP_DONE);
		// TODO: Implement MonoflopDone callback handling

		return false;
	}

	if(bootloader_spitfp_is_send_possible(&bootloader_status.st)) {
		bootloader_spitfp_send_ack_and_message(&bootloader_status, (uint8_t*)&cb, sizeof(MonoflopDone_Callback));
		is_buffered = false;
		return true;
	} else {
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
