#ifndef ESP32_RUNTIME_PENDING_TABLE_H
#define ESP32_RUNTIME_PENDING_TABLE_H

#include <stdint.h>
#include <stdbool.h>
#include "ap_message.h"

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

#endif // ESP32_RUNTIME_PENDING_TABLE_H