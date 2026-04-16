#ifndef AP_MESSAGE_IDS_H
#define AP_MESSAGE_IDS_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "ap_err.h"

#define AP_DEVICE_ID_MAX_LENGTH 20
#define AP_MESSAGE_ID_MAX_LENGTH 32
#define AP_BOOT_ID_MAX_LENGTH 9

typedef struct {
    char value[AP_DEVICE_ID_MAX_LENGTH];
} device_id_t;

typedef struct {
    char value[AP_BOOT_ID_MAX_LENGTH];
} boot_id_t;

typedef struct {
    char value[AP_MESSAGE_ID_MAX_LENGTH];
} message_id_t;

typedef struct {
    char value[AP_MESSAGE_ID_MAX_LENGTH];
} transaction_id_t;

#ifdef __cplusplus
extern "C" {
#endif

static device_id_t device_id;
static boot_id_t boot_id;
static int next_message_id_ctr;
static int next_transaction_id_ctr;
static int ap_get_next_message_id_ctr();
static int ap_get_next_transaction_id_ctr();
int ap_init(const char *value);
int ap_build_device_id(device_id_t *out, const char *value);
int ap_generate_random_boot_id(boot_id_t *out);
int ap_build_message_id(message_id_t *out, device_id_t sender_id);
int ap_build_transaction_id(transaction_id_t *out);
device_id_t ap_get_device_id();
boot_id_t ap_get_boot_id();

#ifdef __cplusplus
}
#endif

#endif // AP_CORE_IDS_H