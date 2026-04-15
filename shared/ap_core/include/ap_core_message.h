#ifndef AP_CORE_MESSAGE_H
#define AP_CORE_MESSAGE_H

#include <stdint.h>
#include "ap_core_version.h"
#include "ap_core_ids.h"
#include "ap_core_types.h"
#include "ap_core_payloads.h"
#include "ap_core_err.h"
#include "ap_core_results.h"

typedef struct message_t {
    device_id_t target;
    device_id_t sender;
    uint8_t protocol_version_major;
    uint8_t protocol_version_minor;
    message_id_t message_id;
    transaction_id_t transaction_id;
    message_type_t message_type;
    message_id_t ack_for_message_id;
    action_t action;
    payload_t payload;
    result_t result;
    uint32_t timestamp_ms;
    uint8_t retry_count;
} message_t;

#ifdef __cplusplus
extern "C" {
#endif

int ap_build_message(
                    message_t *msg, 
                    device_id_t target, 
                    transaction_id_t transaction_id, 
                    message_type_t message_type, 
                    message_id_t ack_for_message_id, 
                    action_t action, 
                    payload_t payload, 
                    result_t result, 
                    int retry_count);
int ap_build_ack_message(message_t *ack_msg, const message_t *original_msg);

#ifdef __cplusplus
}
#endif

#endif // AP_CORE_MESSAGE_H