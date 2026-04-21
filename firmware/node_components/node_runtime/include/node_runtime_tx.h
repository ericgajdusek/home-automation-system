#ifndef NODE_TX_H
#define NODE_TX_H
#include <stdint.h>
#include <stdbool.h>
#include "node_runtime_types.h"
#include "node_runtime_pending_table.h"

typedef enum {
    EGRESS_PRIORITY_HIGH = 0,
    EGRESS_PRIORITY_NORMAL,
    EGRESS_PRIORITY_LOW
} tx_priority_t;

typedef struct {
    bool requires_ack;
    uint8_t max_retries;
    uint32_t retry_timeout_ms;
} reliability_config_t;

typedef struct {
    tx_priority_t priority;
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
} tx_item_t;

int node_tx_enqueue(const tx_item_t *item);
int node_tx_send(const tx_item_t *item);
int node_tx_process_next();

#endif // NODE_TX_H