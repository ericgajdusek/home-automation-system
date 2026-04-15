#include "ap_core_message.h"

int ap_build_message(
                    message_t *msg, 
                    device_id_t target, 
                    transaction_id_t transaction_id, 
                    message_type_t message_type, 
                    message_id_t ack_for_message_id, 
                    action_t action, 
                    payload_t payload, 
                    result_t result, 
                    int retry_count) {

    message_id_t message_id;
    if (ap_build_message_id(&message_id, ap_get_device_id()) != PROTOCOL_OK) {
        return PROTOCOL_ERR_INVALID_ARG;
    }

    msg->target = target;
    msg->sender = ap_get_device_id();
    msg->protocol_version_major = ap_get_protocol_version_major();
    msg->protocol_version_minor = ap_get_protocol_version_minor();
    msg->message_id = message_id;
    msg->transaction_id = transaction_id;
    msg->message_type = message_type;
    msg->ack_for_message_id = ack_for_message_id;
    msg->action = action;
    msg->payload = payload;
    msg->result = result;
    msg->timestamp_ms = 0;
    msg->retry_count = retry_count;
    return PROTOCOL_OK;
}

int ap_build_ack_message(message_t *ack_msg, const message_t *original_msg) {
    ap_build_message(ack_msg, original_msg->sender, original_msg->transaction_id, MSG_TYPE_ACK, original_msg->message_id, ACTION_NONE, (payload_t){0}, (result_t){0}, 0);
    return PROTOCOL_OK;
}