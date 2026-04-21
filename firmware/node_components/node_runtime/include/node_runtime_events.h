#ifndef NODE_RUNTIME_EVENTS_H
#define NODE_RUNTIME_EVENTS_H

#include <stdint.h>
#include "ap_message.h"
#include "provisioning_message.h"
#include "node_runtime_types.h"

#define UI_CONTROL_ID_MAX_LEN 32

typedef struct {
    char control_id[UI_CONTROL_ID_MAX_LEN];   // optional, good for debugging
    device_id_t target_device;
    uint8_t value;                       // 0-100
} ui_slider_changed_event_t;

typedef struct {
    char control_id[UI_CONTROL_ID_MAX_LEN];
    device_id_t target_device;
    uint8_t button_id;
} ui_button_pressed_event_t;

typedef struct {
    ap_message_t msg;
    transport_type_t received_via;
    transport_address_t source;
} ap_received_event_t;

typedef struct {
    provisioning_message_t msg;
    transport_type_t received_via;
    transport_address_t source;
} provisioning_received_event_t;

typedef struct {
    message_id_t message_id;
    transaction_id_t transaction_id;
    transport_type_t transport;
    int error_code;
} tx_send_failed_event_t;

typedef struct {
    message_id_t message_id;
    transaction_id_t transaction_id;
} tx_timeout_event_t;

typedef struct {
    message_id_t message_id;
    transaction_id_t transaction_id;
} retry_requested_event_t;

typedef struct {
    transport_type_t transport;
} transport_state_event_t;

#endif // NODE_RUNTIME_EVENTS_H