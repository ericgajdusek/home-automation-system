#ifndef APP_PROTOCOL_MESSAGES_H
#define APP_PROTOCOL_MESSAGES_H

#include <stdint.h>
#include "app_protocol_version.h"
#include "app_protocol_ids.h"
#include "app_protocol_types.h"
#include "app_protocol_payloads.h"

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
    uint32_t timestamp_ms;
    uint8_t retry_count;
} message_t;

#ifdef __cplusplus
extern "C" {
#endif



#ifdef __cplusplus
}
#endif

#endif // APP_PROTOCOL_MESSAGES_H