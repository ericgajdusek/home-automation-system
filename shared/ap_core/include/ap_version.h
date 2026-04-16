#ifndef AP_VERSION_H
#define AP_VERSION_H

#include <stdint.h>

#define AP_VERSION_MAJOR ((uint8_t)1)
#define AP_VERSION_MINOR ((uint8_t)0)

#ifdef __cplusplus
extern "C" {
#endif  

uint8_t ap_get_protocol_version_major();
uint8_t ap_get_protocol_version_minor();

#ifdef __cplusplus
}
#endif

#endif // AP_VERSION_H