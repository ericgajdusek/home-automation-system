

typedef struct {
    union {
        ap_device_id_t id;
        provisioning_device_id_t prov_id;
    }
} device_id_t;

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
    transport_type_t transport;
    union {
        mqtt_address_t mqtt;
        espnow_address_t espnow;
    } addr;
} transport_address_t;

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
    PENDING_STATE_NONE = 0,
    PENDING_STATE_WAITING_FOR_ACK,
    PENDING_STATE_RETRY_DUE,
    PENDING_STATE_EXPIRED
} pending_state_t;

typedef struct {
    bool in_use;

    pending_state_t state;

    message_id_t message_id;
    transaction_id_t transaction_id;

    uint8_t retry_count;
    uint8_t max_retries;

    uint32_t retry_timeout_ms;
    uint64_t next_retry_deadline_ms;
    uint64_t last_send_timestamp_ms;

    egress_item_t original_request;
} pending_entry_t;