#ifndef NODE_RX_H
#define NODE_RX_H

#include <stdint.h>
#include <stdbool.h>
#include "node_runtime_types.h"

#define PAYLOAD_MAX_LEN 256

typedef struct {
    transport_type_t transport;
    protocol_type_t protocol_hint;
    codec_type_t codec_hint;

    transport_address_t source;

    uint16_t payload_len;
    uint8_t payload[PAYLOAD_MAX_LEN];

    uint64_t timestamp_ms;
} rx_item_t;

int node_rx_enqueue(const rx_item_t *item);
int node_rx_process_next();

#endif // NODE_RX_H