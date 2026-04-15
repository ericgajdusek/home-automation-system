#ifndef AP_CORE_VALIDATE_H
#define AP_CORE_VALIDATE_H

#include "ap_core_message.h"

#ifdef __cplusplus
extern "C" {
#endif

int ap_validate_message(const message_t* msg);

#ifdef __cplusplus
}
#endif

#endif // AP_CORE_VALIDATE_H