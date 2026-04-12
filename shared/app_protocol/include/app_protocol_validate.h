#ifndef APP_PROTOCOL_VALIDATE_H
#define APP_PROTOCOL_VALIDATE_H

#include "app_protocol_message.h"

#ifdef __cplusplus
extern "C" {
#endif

int validate_message(const message_t* msg);

#ifdef __cplusplus
}
#endif

#endif // APP_PROTOCOL_VALIDATE_H