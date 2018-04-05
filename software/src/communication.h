/* io16-v2-bricklet
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
#define IO16_V2_DIRECTION_IN 'i'
#define IO16_V2_DIRECTION_OUT 'o'

#define IO16_V2_EDGE_TYPE_RISING 0
#define IO16_V2_EDGE_TYPE_FALLING 1
#define IO16_V2_EDGE_TYPE_BOTH 2

#define IO16_V2_BOOTLOADER_MODE_BOOTLOADER 0
#define IO16_V2_BOOTLOADER_MODE_FIRMWARE 1
#define IO16_V2_BOOTLOADER_MODE_BOOTLOADER_WAIT_FOR_REBOOT 2
#define IO16_V2_BOOTLOADER_MODE_FIRMWARE_WAIT_FOR_REBOOT 3
#define IO16_V2_BOOTLOADER_MODE_FIRMWARE_WAIT_FOR_ERASE_AND_REBOOT 4

#define IO16_V2_BOOTLOADER_STATUS_OK 0
#define IO16_V2_BOOTLOADER_STATUS_INVALID_MODE 1
#define IO16_V2_BOOTLOADER_STATUS_NO_CHANGE 2
#define IO16_V2_BOOTLOADER_STATUS_ENTRY_FUNCTION_NOT_PRESENT 3
#define IO16_V2_BOOTLOADER_STATUS_DEVICE_IDENTIFIER_INCORRECT 4
#define IO16_V2_BOOTLOADER_STATUS_CRC_MISMATCH 5

#define IO16_V2_STATUS_LED_CONFIG_OFF 0
#define IO16_V2_STATUS_LED_CONFIG_ON 1
#define IO16_V2_STATUS_LED_CONFIG_SHOW_HEARTBEAT 2
#define IO16_V2_STATUS_LED_CONFIG_SHOW_STATUS 3

// Function and callback IDs and structs
#define FID_SET_VALUE 1
#define FID_GET_VALUE 2
#define FID_SET_SELECTED_VALUE 3
#define FID_GET_SELECTED_VALUE 4
#define FID_SET_CONFIGURATION 5
#define FID_GET_CONFIGURATION 6
#define FID_SET_INPUT_VALUE_CALLBACK_CONFIGURATION 7
#define FID_GET_INPUT_VALUE_CALLBACK_CONFIGURATION 8
#define FID_SET_ALL_INPUT_VALUE_CALLBACK_CONFIGURATION 9
#define FID_GET_ALL_INPUT_VALUE_CALLBACK_CONFIGURATION 10
#define FID_SET_MONOFLOP 11
#define FID_GET_MONOFLOP 12
#define FID_GET_EDGE_COUNT 13
#define FID_SET_EDGE_COUNT_CONFIGURATION 14
#define FID_GET_EDGE_COUNT_CONFIGURATION 15

#define FID_CALLBACK_INPUT_VALUE 16
#define FID_CALLBACK_ALL_INPUT_VALUE 17
#define FID_CALLBACK_MONOFLOP_DONE 18

typedef struct {
	TFPMessageHeader header;
	bool value[16];
} __attribute__((__packed__)) SetValue;

typedef struct {
	TFPMessageHeader header;
} __attribute__((__packed__)) GetValue;

typedef struct {
	TFPMessageHeader header;
	bool value[16];
} __attribute__((__packed__)) GetValue_Response;

typedef struct {
	TFPMessageHeader header;
	uint8_t channel;
	bool value;
} __attribute__((__packed__)) SetSelectedValue;

typedef struct {
	TFPMessageHeader header;
	uint8_t channel;
} __attribute__((__packed__)) GetSelectedValue;

typedef struct {
	TFPMessageHeader header;
	bool value;
} __attribute__((__packed__)) GetSelectedValue_Response;

typedef struct {
	TFPMessageHeader header;
	uint8_t channel;
	char direction;
	bool value;
} __attribute__((__packed__)) SetConfiguration;

typedef struct {
	TFPMessageHeader header;
	uint8_t channel;
} __attribute__((__packed__)) GetConfiguration;

typedef struct {
	TFPMessageHeader header;
	char direction;
	bool value;
} __attribute__((__packed__)) GetConfiguration_Response;

typedef struct {
	TFPMessageHeader header;
	uint8_t channel;
	uint32_t period;
	bool value_has_to_change;
} __attribute__((__packed__)) SetInputValueCallbackConfiguration;

typedef struct {
	TFPMessageHeader header;
	uint8_t channel;
} __attribute__((__packed__)) GetInputValueCallbackConfiguration;

typedef struct {
	TFPMessageHeader header;
	uint32_t period;
	bool value_has_to_change;
} __attribute__((__packed__)) GetInputValueCallbackConfiguration_Response;

typedef struct {
	TFPMessageHeader header;
	uint32_t period;
	bool value_has_to_change;
} __attribute__((__packed__)) SetAllInputValueCallbackConfiguration;

typedef struct {
	TFPMessageHeader header;
} __attribute__((__packed__)) GetAllInputValueCallbackConfiguration;

typedef struct {
	TFPMessageHeader header;
	uint32_t period;
	bool value_has_to_change;
} __attribute__((__packed__)) GetAllInputValueCallbackConfiguration_Response;

typedef struct {
	TFPMessageHeader header;
	uint8_t channel;
	bool value;
	uint32_t time;
} __attribute__((__packed__)) SetMonoflop;

typedef struct {
	TFPMessageHeader header;
	uint8_t channel;
} __attribute__((__packed__)) GetMonoflop;

typedef struct {
	TFPMessageHeader header;
	bool value;
	uint32_t time;
	uint32_t time_remaining;
} __attribute__((__packed__)) GetMonoflop_Response;

typedef struct {
	TFPMessageHeader header;
	uint8_t channel;
	bool reset_counter;
} __attribute__((__packed__)) GetEdgeCount;

typedef struct {
	TFPMessageHeader header;
	uint32_t count;
} __attribute__((__packed__)) GetEdgeCount_Response;

typedef struct {
	TFPMessageHeader header;
	uint8_t channel;
	uint8_t edge_type;
	uint8_t debounce;
} __attribute__((__packed__)) SetEdgeCountConfiguration;

typedef struct {
	TFPMessageHeader header;
	uint8_t channel;
} __attribute__((__packed__)) GetEdgeCountConfiguration;

typedef struct {
	TFPMessageHeader header;
	uint8_t edge_type;
	uint8_t debounce;
} __attribute__((__packed__)) GetEdgeCountConfiguration_Response;

typedef struct {
	TFPMessageHeader header;
	uint8_t channel;
	bool changed;
	bool value;
} __attribute__((__packed__)) InputValue_Callback;

typedef struct {
	TFPMessageHeader header;
	bool changed[16];
	bool value[16];
} __attribute__((__packed__)) AllInputValue_Callback;

typedef struct {
	TFPMessageHeader header;
	uint8_t channel;
	bool value;
} __attribute__((__packed__)) MonoflopDone_Callback;


// Function prototypes
BootloaderHandleMessageResponse set_value(const SetValue *data);
BootloaderHandleMessageResponse get_value(const GetValue *data, GetValue_Response *response);
BootloaderHandleMessageResponse set_selected_value(const SetSelectedValue *data);
BootloaderHandleMessageResponse get_selected_value(const GetSelectedValue *data, GetSelectedValue_Response *response);
BootloaderHandleMessageResponse set_configuration(const SetConfiguration *data);
BootloaderHandleMessageResponse get_configuration(const GetConfiguration *data, GetConfiguration_Response *response);
BootloaderHandleMessageResponse set_input_value_callback_configuration(const SetInputValueCallbackConfiguration *data);
BootloaderHandleMessageResponse get_input_value_callback_configuration(const GetInputValueCallbackConfiguration *data, GetInputValueCallbackConfiguration_Response *response);
BootloaderHandleMessageResponse set_all_input_value_callback_configuration(const SetAllInputValueCallbackConfiguration *data);
BootloaderHandleMessageResponse get_all_input_value_callback_configuration(const GetAllInputValueCallbackConfiguration *data, GetAllInputValueCallbackConfiguration_Response *response);
BootloaderHandleMessageResponse set_monoflop(const SetMonoflop *data);
BootloaderHandleMessageResponse get_monoflop(const GetMonoflop *data, GetMonoflop_Response *response);
BootloaderHandleMessageResponse get_edge_count(const GetEdgeCount *data, GetEdgeCount_Response *response);
BootloaderHandleMessageResponse set_edge_count_configuration(const SetEdgeCountConfiguration *data);
BootloaderHandleMessageResponse get_edge_count_configuration(const GetEdgeCountConfiguration *data, GetEdgeCountConfiguration_Response *response);

// Callbacks
bool handle_input_value_callback(void);
bool handle_all_input_value_callback(void);
bool handle_monoflop_done_callback(void);

#define COMMUNICATION_CALLBACK_TICK_WAIT_MS 1
#define COMMUNICATION_CALLBACK_HANDLER_NUM 3
#define COMMUNICATION_CALLBACK_LIST_INIT \
	handle_input_value_callback, \
	handle_all_input_value_callback, \
	handle_monoflop_done_callback, \


#endif
