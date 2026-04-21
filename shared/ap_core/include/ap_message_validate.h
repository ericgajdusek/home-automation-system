#ifndef AP_MESSAGE_VALIDATE_H
#define AP_MESSAGE_VALIDATE_H

#include "ap_message.h"

#ifdef __cplusplus
extern "C" {
#endif

int ap_validate_message(const ap_message_t* msg);

#ifdef __cplusplus
}
#endif

#endif // AP_MESSAGE_VALIDATE_H