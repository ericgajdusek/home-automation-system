#ifndef APP_PROTOCOL_VALIDATE_H
#define APP_PROTOCOL_VALIDATE_H

#include "app_protocol_message.h"

#define PROTOCOL_OK 0
#define PROTOCOL_ERR_NULL_PTR -1
#define PROTOCOL_ERR_INVALID_ARG -2
#define PROTOCOL_ERR_TRUNCATED -3

#ifdef __cplusplus
extern "C" {
#endif

int validate_message(const message_t* msg);

#ifdef __cplusplus
}
#endif

#endif // APP_PROTOCOL_VALIDATE_H