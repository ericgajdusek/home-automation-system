#ifndef APP_PROTOCOL_VERSION_H
#define APP_PROTOCOL_VERSION_H

#include <stdint.h>

#define PROTOCOL_VERSION_MAJOR ((uint8_t)1)
#define PROTOCOL_VERSION_MINOR ((uint8_t)0)

#ifdef __cplusplus
extern "C" {
#endif  

uint8_t get_protocol_version_major();

uint8_t get_protocol_version_minor();

#ifdef __cplusplus
}
#endif

#endif // APP_PROTOCOL_VERSION_H