#ifndef APP_PROTOCOL_JSON_H
#define APP_PROTOCOL_JSON_H

#include <stddef.h>
#include <stdint.h>

#include "app_protocol_message.h"
#include "app_protocol_types.h"
#include "app_protocol_validate.h"
#include "cJSON.h"

#ifdef __cplusplus
extern "C" {
#endif

int app_protocol_message_to_json(const message_t *msg, char *out_buf, size_t out_buf_size);
int app_protocol_message_from_json(const uint8_t *buffer, size_t buffer_size, message_t *out_msg);

#ifdef __cplusplus
}
#endif

#endif // APP_PROTOCOL_JSON_H