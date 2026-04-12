#include "app_protocol_payloads.h"

int build_lights_payload(payload_t *payload, uint8_t brightness) {
    if (!payload) {
        return PROTOCOL_ERR_NULL_PTR;
    }
    if (brightness > 100 || brightness < 0) {
        return PROTOCOL_ERR_INVALID_ARG;
    }
    payload->lights.brightness = brightness;
    return PROTOCOL_OK;
}

int build_blinds_payload(payload_t *payload, uint8_t position) {
    if (!payload) {
        return PROTOCOL_ERR_NULL_PTR;
    }
    if (position > 100 || position < 0) {
        return PROTOCOL_ERR_INVALID_ARG;
    }
    payload->blinds.position = position;
    return PROTOCOL_OK;
}