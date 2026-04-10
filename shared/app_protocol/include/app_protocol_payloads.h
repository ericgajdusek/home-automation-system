#ifndef APP_PROTOCOL_PAYLOADS_H
#define APP_PROTOCOL_PAYLOADS_H

#include <stdint.h>

typedef struct {
    uint8_t brightness;
} lights_payload_t;

typedef struct {
    uint8_t position;
} blinds_payload_t;

typedef struct {
    uint8_t code;
    char message[64];
} error_payload_t;

typedef union {
    lights_payload_t lights;
    blinds_payload_t blinds;
    error_payload_t error;
} payload_t;

#ifdef __cplusplus
extern "C" {
#endif 



#ifdef __cplusplus
}
#endif

#endif // APP_PROTOCOL_PAYLOADS_H