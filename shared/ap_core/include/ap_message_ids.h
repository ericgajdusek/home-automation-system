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
} ap_device_id_t;

typedef struct {
    char value[AP_BOOT_ID_MAX_LENGTH];
} ap_boot_id_t;

typedef struct {
    char value[AP_MESSAGE_ID_MAX_LENGTH];
} ap_message_id_t;

typedef struct {
    char value[AP_MESSAGE_ID_MAX_LENGTH];
} ap_transaction_id_t;

#ifdef __cplusplus
extern "C" {
#endif

static ap_device_id_t ap_device_id;
static ap_boot_id_t ap_boot_id;
static int next_message_id_ctr;
static int next_transaction_id_ctr;
static int ap_get_next_message_id_ctr();
static int ap_get_next_transaction_id_ctr();
int ap_init(const char *value);
int ap_build_device_id(ap_device_id_t *out, const char *value);
int ap_generate_random_boot_id(ap_boot_id_t *out);
int ap_build_message_id(ap_message_id_t *out, ap_device_id_t sender_id);
int ap_build_transaction_id(ap_transaction_id_t *out);
ap_device_id_t ap_get_device_id();
ap_boot_id_t ap_get_boot_id();

#ifdef __cplusplus
}
#endif

#endif // AP_CORE_IDS_H