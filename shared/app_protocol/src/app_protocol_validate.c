#include "app_protocol_validate.h"

int validate_message(const message_t* msg) {
    if (!msg) {
        return PROTOCOL_ERR_NULL_PTR;
    }
    if (msg->protocol_version_major != get_protocol_version_major() || msg->protocol_version_minor != get_protocol_version_minor()) {
        return PROTOCOL_ERR_INVALID_ARG;
    }
    if (msg->message_id.id[0] == '\0' || msg->transaction_id.id[0] == '\0') {
        return PROTOCOL_ERR_INVALID_ARG;
    }
    // Additional validation logic can be added here (e.g., check payload size, valid actions for message type, etc.)
    return PROTOCOL_OK;
}