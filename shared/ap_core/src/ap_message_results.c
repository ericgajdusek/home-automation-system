#include "ap_message_results.h"

int ap_build_result(ap_result_t *result, uint8_t code, const char *message) {
    if (!result || !message) {
        return AP_ERR_NULL_PTR;
    }
    if (code > 255) {
        return AP_ERR_INVALID_ARG;
    }
    result->code = code;
    int err = snprintf(result->message, sizeof(result->message), "%s", message);
    if (err < 0) {
        return AP_ERR_INVALID_ARG;
    }
    if (err >= sizeof(result->message)) {
        return AP_ERR_TRUNCATED;
    }
    return AP_OK;
}