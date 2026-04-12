#ifndef APP_PROTOCOL_PAYLOADS_H
#define APP_PROTOCOL_PAYLOADS_H

#include <stdint.h>
#include "app_protocol_validate.h"

typedef struct {
    uint8_t brightness;
} lights_payload_t;

typedef struct {
    uint8_t position;
} blinds_payload_t;

typedef union {
    lights_payload_t lights;
    blinds_payload_t blinds;
} payload_t;

#ifdef __cplusplus
extern "C" {
#endif 

int build_lights_payload(payload_t *payload, uint8_t brightness);

int build_blinds_payload(payload_t *payload, uint8_t position);

#ifdef __cplusplus
}
#endif

#endif // APP_PROTOCOL_PAYLOADS_H