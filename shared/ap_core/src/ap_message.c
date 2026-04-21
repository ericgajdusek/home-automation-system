#include "ap_message.h"

int ap_build_message(
                    ap_message_t *msg, 
                    ap_device_id_t target, 
                    ap_transaction_id_t transaction_id, 
                    ap_message_type_t message_type, 
                    ap_message_id_t ack_for_message_id, 
                    ap_action_t action, 
                    ap_payload_t payload, 
                    ap_result_t result, 
                    int retry_count) {

    ap_message_id_t message_id;
    if (ap_build_message_id(&message_id, ap_get_device_id()) != AP_OK) {
        return AP_ERR_INVALID_ARG;
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
    return AP_OK;
}

int ap_build_ack_message(ap_message_t *ack_msg, const ap_message_t *original_msg) {
    ap_build_message(ack_msg, original_msg->sender, original_msg->transaction_id, AP_MSG_TYPE_ACK, original_msg->message_id, AP_ACTION_NONE, (ap_payload_t){0}, (ap_result_t){0}, 0);
    return AP_OK;
}