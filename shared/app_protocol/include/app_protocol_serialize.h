#ifndef APP_PROTOCOL_SERIALIZE_H
#define APP_PROTOCOL_SERIALIZE_H

#include "app_protocol_messages.h"
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

int serialize_message(const message_t* msg, uint8_t* buffer, size_t buffer_size);
int deserialize_message(const uint8_t* buffer, size_t buffer_size, message_t* msg);

#ifdef __cplusplus
}
#endif

#endif // APP_PROTOCOL_SERIALIZE_H