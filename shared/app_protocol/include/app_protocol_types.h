#ifndef APP_PROTOCOL_TYPES_H
#define APP_PROTOCOL_TYPES_H

typedef enum {
    MSG_TYPE_NONE = 0,
    MSG_TYPE_COMMAND,
    MSG_TYPE_STATUS,
    MSG_TYPE_ACK,
    MSG_TYPE_ERROR
} message_type_t;

typedef enum {
    ACTION_NONE = 0,
    ACTION_SET_LIGHTS,
    ACTION_SET_BLINDS
} action_t;

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
}
#endif


#endif // APP_PROTOCOL_TYPES_H