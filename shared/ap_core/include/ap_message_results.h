#ifndef AP_MESSAGE_RESULTS_H
#define AP_MESSAGE_RESULTS_H

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "ap_err.h"

// typedef enum {
//     AP_COMPONENT_NONE = 0,
//     AP_COMPONENT_CORE,
//     AP_COMPONENT_JSON_CODEC,
//     AP_COMPONENT_ESP32_RUNTIME,
//     AP_COMPONENT_ESP32_TRANSPORT_MQTT,
//     AP_COMPONENT_ESP32_TRANSPORT_ESPNOW,
//     AP_COMPONENT_SERVER_TRANSPORT_MQTT
// } ap_component_t;

// typedef struct {
//     uint16_t code;
//     ap_component_t source_component;
//     char message[64];
// } result_t;

typedef struct {
    uint8_t code;
    char message[64];
} result_t;

#ifdef __cplusplus
extern "C" {
#endif

int ap_build_result(result_t *result, uint8_t code, const char *message);

#ifdef __cplusplus
}
#endif  

#endif // AP_MESSAGE_RESULTS_H