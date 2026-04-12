#ifndef APP_PROTOCOL_VERSION_H
#define APP_PROTOCOL_VERSION_H

#define PROTOCOL_VERSION_MAJOR 1
#define PROTOCOL_VERSION_MINOR 0

#ifdef __cplusplus
extern "C" {
#endif  

uint8_t get_protocol_version_major() {
    return PROTOCOL_VERSION_MAJOR;
}

uint8_t get_protocol_version_minor() {
    return PROTOCOL_VERSION_MINOR;
}

#ifdef __cplusplus
}
#endif

#endif // APP_PROTOCOL_VERSION_H