#ifndef APP_PROTOCOL_IDS_H
#define APP_PROTOCOL_IDS_H

#include <stdio.h>
#include <string.h>
#include "app_protocol_err.h"

#define DEVICE_ID_MAX_LENGTH 20
#define MESSAGE_ID_MAX_LENGTH 32
#define TRANSACTION_ID_MAX_LENGTH 32
#define BOOT_ID_MAX_LENGTH 9

typedef struct {
    char id[DEVICE_ID_MAX_LENGTH];
} device_id_t;

typedef struct {
    char id[BOOT_ID_MAX_LENGTH];
} boot_id_t;

typedef struct {
    char id[MESSAGE_ID_MAX_LENGTH];
} message_id_t;

typedef struct {
    char id[TRANSACTION_ID_MAX_LENGTH];
} transaction_id_t;

#ifdef __cplusplus
extern "C" {
#endif

extern device_id_t device_id;
extern boot_id_t boot_id;
extern int next_message_id_ctr;
extern int next_transaction_id_ctr;
int get_next_message_id_ctr();
int get_next_transaction_id_ctr();
int build_device_id(device_id_t *out, const char *device_name, const char *mac_suffix);
int generate_random_boot_id(boot_id_t *out);
int build_message_id(message_id_t *out, device_id_t sender_id, boot_id_t boot_id);
int build_transaction_id(transaction_id_t *out, device_id_t sender_id, boot_id_t boot_id);
device_id_t get_device_id();
boot_id_t get_boot_id();

#ifdef __cplusplus
}
#endif

#endif // APP_PROTOCOL_IDS_H