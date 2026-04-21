#ifndef AP_MESSAGE_TYPES_H
#define AP_MESSAGE_TYPES_H

typedef enum {
    AP_MSG_TYPE_NONE = 0,
    AP_MSG_TYPE_COMMAND,
    AP_MSG_TYPE_STATUS,
    AP_MSG_TYPE_ACK,
} ap_message_type_t;

typedef enum {
    AP_ACTION_NONE = 0,
    AP_ACTION_SET_LIGHTS,
    AP_ACTION_SET_BLINDS
} ap_action_t;

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
}
#endif


#endif // AP_MESSAGE_TYPES_H