#ifndef APP_PROTOCOL_IDS_H
#define APP_PROTOCOL_IDS_H

#define DEVICE_ID_MAX_LENGTH 16
#define MESSAGE_ID_MAX_LENGTH 32
#define TRANSACTION_ID_MAX_LENGTH 32

typedef struct {
    char id[DEVICE_ID_MAX_LENGTH];
} device_id_t;

typedef struct {
    char id[MESSAGE_ID_MAX_LENGTH];
} message_id_t;

typedef struct {
    char id[TRANSACTION_ID_MAX_LENGTH];
} transaction_id_t;

#ifdef __cplusplus
extern "C" {
#endif

int next_message_id;
int next_transaction_id;

#ifdef __cplusplus
}
#endif

#endif // APP_PROTOCOL_IDS_H