#include "app_protocol_results.h"

int build_result(result_t *result, uint8_t code, const char *message) {
    if (!result || !message) {
        return PROTOCOL_ERR_NULL_PTR;
    }
    if (code > 255) {
        return PROTOCOL_ERR_INVALID_ARG;
    }
    result->code = code;
    int err = snprintf(result->message, sizeof(result->message), "%s", message);
    if (err < 0) {
        return PROTOCOL_ERR_INVALID_ARG;
    }
    if (err >= sizeof(result->message)) {
        return PROTOCOL_ERR_TRUNCATED;
    }
    return PROTOCOL_OK;
}