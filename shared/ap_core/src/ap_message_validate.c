#include "ap_message_validate.h"

int ap_validate_message(const ap_message_t* msg) {
    if (!msg) {
        return AP_ERR_NULL_PTR;
    }
    if (msg->protocol_version_major != ap_get_protocol_version_major() || msg->protocol_version_minor != ap_get_protocol_version_minor()) {
        return AP_ERR_INVALID_ARG;
    }
    if (msg->message_id.value[0] == '\0' || msg->transaction_id.value[0] == '\0') {
        return AP_ERR_INVALID_ARG;
    }
    // Additional validation logic can be added here (e.g., check payload size, valid actions for message type, etc.)
    return AP_OK;
}