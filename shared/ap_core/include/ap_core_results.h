#ifndef AP_CORE_RESULTS_H
#define AP_CORE_RESULTS_H

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "ap_core_err.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    uint8_t code;
    char message[64];
} result_t;

int ap_build_result(result_t *result, uint8_t code, const char *message);

#ifdef __cplusplus
}
#endif  

#endif // AP_CORE_RESULTS_H