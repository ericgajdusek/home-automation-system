#ifndef ESP32_RUNTIME_QUEUES_H
#define ESP32_RUNTIME_QUEUES_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "ap_message.h"

#define TOPIC_MAX_LEN            96
#define PAYLOAD_MAX_LEN          256
#define UI_CONTROL_ID_MAX_LEN    32
#define MAX_RETRIES_DEFAULT      3

typedef enum {
    TRANSPORT_NONE = 0,
    TRANSPORT_MQTT,
    TRANSPORT_ESPNOW
} transport_type_t;

typedef enum {
    PROTOCOL_UNKNOWN = 0,
    PROTOCOL_AP,
    PROTOCOL_PROVISIONING
} protocol_type_t;

typedef enum {
    CODEC_UNKNOWN = 0,
    CODEC_JSON,
    CODEC_BINARY
} codec_type_t;

typedef enum {
    EVENT_NONE = 0,

    /* Local/UI events */
    EVENT_UI_BUTTON_PRESSED,
    EVENT_UI_SLIDER_CHANGED,

    /* Decoded incoming protocol events */
    EVENT_AP_COMMAND_RECEIVED,
    EVENT_AP_STATUS_RECEIVED,
    EVENT_AP_ACK_RECEIVED,

    EVENT_PROV_DISCOVERY_RECEIVED,
    EVENT_PROV_RESPONSE_RECEIVED,
    EVENT_PROV_CONFIG_RECEIVED,

    /* Internal/runtime events */
    EVENT_TX_SEND_FAILED,
    EVENT_TX_TIMEOUT_EXPIRED,
    EVENT_RETRY_REQUESTED,

    EVENT_TRANSPORT_CONNECTED,
    EVENT_TRANSPORT_DISCONNECTED
} event_type_t;

typedef struct {
    char topic[TOPIC_MAX_LEN];
} mqtt_address_t;

typedef struct {
    uint8_t mac[6];
    uint8_t channel;   // 0 = current channel if you want that convention
} espnow_address_t;

typedef struct {
    transport_type_t transport;
    union {
        mqtt_address_t mqtt;
        espnow_address_t espnow;
    } addr;
} transport_address_t;

typedef struct {
    transport_type_t transport;
    protocol_type_t protocol_hint;
    codec_type_t codec_hint;

    transport_address_t source;

    uint16_t payload_len;
    uint8_t payload[PAYLOAD_MAX_LEN];

    uint64_t timestamp_ms;
} ingress_item_t;

typedef struct {
    char control_id[UI_CONTROL_ID_MAX_LEN];   // optional, good for debugging
    device_id_t target_device;
    uint8_t brightness;                       // 0-100
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

typedef struct {
    event_type_t type;
    uint64_t timestamp_ms;

    union {
        ui_slider_changed_event_t ui_slider_changed;
        ui_button_pressed_event_t ui_button_pressed;

        ap_received_event_t ap_received;
        provisioning_received_event_t prov_received;

        tx_send_failed_event_t tx_send_failed;
        tx_timeout_event_t tx_timeout;
        retry_requested_event_t retry_requested;

        transport_state_event_t transport_state;
    } data;
} event_item_t;


typedef enum {
    EGRESS_PRIORITY_HIGH = 0,
    EGRESS_PRIORITY_NORMAL,
    EGRESS_PRIORITY_LOW
} egress_priority_t;

typedef struct {
    bool requires_ack;
    uint8_t max_retries;
    uint32_t retry_timeout_ms;
} reliability_config_t;

typedef struct {
    egress_priority_t priority;
    transport_type_t transport;
    protocol_type_t protocol;
    codec_type_t codec;
    transport_address_t destination;
    reliability_config_t reliability;
    union {
        ap_message_t ap_msg;
        provisioning_message_t prov_msg;
    } msg;
    uint64_t created_at_ms;
} egress_item_t;

#endif // ESP32_RUNTIME_QUEUES_H