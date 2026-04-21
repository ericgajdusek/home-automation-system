#ifndef ESP32_PROVISIONING_H
#define ESP32_PROVISIONING_H
#include "ap_message.h"

typedef enum {
    PROV_MSG_NONE = 0,
    PROV_MSG_DISCOVERY,
    PROV_MSG_RESPONSE,
    PROV_MSG_CONFIG
} provisioning_message_type_t;

typedef struct {
    provisioning_message_type_t type;
    message_id_t message_id;
    device_id_t sender;

    /* Fill in the rest of your provisioning fields */
} provisioning_message_t;

#endif // ESP32_PROVISIONING_H