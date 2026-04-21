#ifndef AP_MESSAGE_H
#define AP_MESSAGE_H

#include <stdint.h>
#include "ap_version.h"
#include "ap_message_ids.h"
#include "ap_message_types.h"
#include "ap_message_payloads.h"
#include "ap_err.h"
#include "ap_message_results.h"

#define AP_MESSAGE_MAX_RETRY_COUNT 3

typedef struct ap_message_t {
    ap_device_id_t target;
    ap_device_id_t sender;
    uint8_t protocol_version_major;
    uint8_t protocol_version_minor;
    ap_message_id_t message_id;
    ap_transaction_id_t transaction_id;
    ap_message_type_t message_type;
    ap_message_id_t ack_for_message_id;
    ap_action_t action;
    ap_payload_t payload;
    ap_result_t result;
    uint32_t timestamp_ms;
    uint8_t retry_count;
} ap_message_t;

#ifdef __cplusplus
extern "C" {
#endif

int ap_build_message(
                    ap_message_t *msg, 
                    ap_device_id_t target, 
                    ap_transaction_id_t transaction_id, 
                    ap_message_type_t message_type, 
                    ap_message_id_t ack_for_message_id, 
                    ap_action_t action, 
                    ap_payload_t payload, 
                    ap_result_t result, 
                    int retry_count);
int ap_build_ack_message(ap_message_t *ack_msg, const ap_message_t *original_msg);

#ifdef __cplusplus
}
#endif

#endif // AP_MESSAGE_H