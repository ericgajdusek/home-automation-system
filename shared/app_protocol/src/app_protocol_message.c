#include "app_protocol_message.h"

int build_message(message_t *msg, device_id_t target, transaction_id_t transaction_id, message_type_t message_type, action_t action, payload_t payload, result_t result) {
    message_id_t message_id;
    if (generate_message_id(&message_id, get_device_id(), get_boot_id()) != PROTOCOL_OK) {
        return PROTOCOL_ERR_INVALID_ARG;
    }
    msg->target = target;
    msg->sender = get_device_id();
    msg->protocol_version_major = get_protocol_version_major();
    msg->protocol_version_minor = get_protocol_version_minor();
    msg->message_id = message_id;
    msg->transaction_id = transaction_id;
    msg->message_type = message_type;
    msg->ack_for_message_id.id[0] = '\0';
    msg->action = action;
    msg->payload = payload;
    msg->result = result;
    msg->timestamp_ms = 0;
    msg->retry_count = 0;
    if (validate_message(msg) != PROTOCOL_OK) {
        return PROTOCOL_ERR_INVALID_ARG;
    }
    return PROTOCOL_OK;
}

int build_acknowledgment_message(message_t *ack_msg, const message_t *original_msg) {
    build_message(ack_msg, original_msg->sender, original_msg->transaction_id, MSG_TYPE_ACK, ACTION_NONE, (payload_t){0}, (result_t){0});
    if (validate_message(ack_msg) != PROTOCOL_OK) {
        return PROTOCOL_ERR_INVALID_ARG;
    }
    return PROTOCOL_OK;
}

int build_new_transaction_message(message_t *msg, device_id_t target, message_type_t message_type, action_t action, payload_t payload) {
    transaction_id_t tx_id;
    if (generate_transaction_id(&tx_id, get_device_id(), get_boot_id()) != PROTOCOL_OK) {
        return PROTOCOL_ERR_INVALID_ARG;
    }
    build_message(&msg, target, tx_id, message_type, action, payload, (result_t){0});
    if (validate_message(msg) != PROTOCOL_OK) {
        return PROTOCOL_ERR_INVALID_ARG;
    }
    return PROTOCOL_OK;
}