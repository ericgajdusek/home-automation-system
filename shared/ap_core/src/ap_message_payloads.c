#include "ap_message_payloads.h"

int ap_build_lights_payload(ap_payload_t *payload, uint8_t brightness) {
    if (!payload) {
        return AP_ERR_INVALID_ARG;
    }
    if (brightness > 100 || brightness < 0) {
        return AP_ERR_INVALID_ARG;
    }
    payload->lights.brightness = brightness;
    return AP_OK;
}

int ap_build_blinds_payload(ap_payload_t *payload, uint8_t position) {
    if (!payload) {
        return AP_ERR_NULL_PTR;
    }
    if (position > 100 || position < 0) {
        return AP_ERR_INVALID_ARG;
    }
    payload->blinds.position = position;
    return AP_OK;
}