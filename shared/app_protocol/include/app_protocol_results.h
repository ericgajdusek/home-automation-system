#ifndef APP_PROTOCOL_RESULTS_H
#define APP_PROTOCOL_RESULTS_H

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "app_protocol_err.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    uint8_t code;
    char message[64];
} result_t;

int build_result(result_t *result, uint8_t code, const char *message);

#ifdef __cplusplus
}
#endif  

#endif // APP_PROTOCOL_RESULTS_H