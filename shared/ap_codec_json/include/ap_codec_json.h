#ifndef AP_CODEC_JSON_H
#define AP_CODEC_JSON_H

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "ap_core_message.h"
#include "ap_core_validate.h"
#include "cJSON.h"

#ifdef __cplusplus
extern "C" {
#endif

int ap_encode_json(const message_t *msg, char *out_buf, size_t out_buf_size);
int ap_decode_json(const uint8_t *buffer, size_t buffer_size, message_t *out_msg);

#ifdef __cplusplus
}
#endif

#endif // AP_CODEC_JSON_H