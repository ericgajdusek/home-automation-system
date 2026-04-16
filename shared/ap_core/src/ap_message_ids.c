#include "ap_message_ids.h"

static device_id_t device_id = {0};
static boot_id_t boot_id = {0};
static int next_message_id_ctr = 0;
static int next_transaction_id_ctr = 0;

static int ap_get_next_message_id_ctr() {
    return next_message_id_ctr++;
}

static int ap_get_next_transaction_id_ctr() {
    return next_transaction_id_ctr++;
}

int ap_build_device_id(device_id_t *out, const char *value) {
    if (!out || !value) {
        return AP_ERR_NULL_PTR;
    }

    int err = snprintf(out->value, AP_DEVICE_ID_MAX_LENGTH, "%s", value);

    if (err < 0) {
        return AP_ERR_INVALID_ARG;
    }

    if (err >= AP_DEVICE_ID_MAX_LENGTH) {
        return AP_ERR_TRUNCATED;
    }

    return AP_OK;
}

int ap_generate_random_boot_id(boot_id_t *out) {
    static const char charset[] =
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz"
        "0123456789";
    size_t charset_len = sizeof(charset) - 1;

    if (!out) {
        return AP_ERR_NULL_PTR;
    }

    for (int i = 0; i < AP_BOOT_ID_MAX_LENGTH - 1; i++) {
        out->value[i] = charset[rand() % charset_len];
    }

    out->value[AP_BOOT_ID_MAX_LENGTH - 1] = '\0';
    return AP_OK;
}

int ap_build_message_id(message_id_t *out, device_id_t sender_id) {
    if (!out) {
        return AP_ERR_NULL_PTR;
    }
    int err = snprintf(out->value, AP_MESSAGE_ID_MAX_LENGTH, "%s_%s_%d", sender_id.value, ap_get_boot_id().value, ap_get_next_message_id_ctr());
    if (err < 0) {
        return AP_ERR_INVALID_ARG;
    }
    if (err >= AP_MESSAGE_ID_MAX_LENGTH) {
        return AP_ERR_TRUNCATED;
    }
    return AP_OK;
}

int ap_build_transaction_id(transaction_id_t *out) {
    if (!out) {
        return AP_ERR_NULL_PTR;
    }
    int err = snprintf(out->value, AP_MESSAGE_ID_MAX_LENGTH, "%s_%s_%d", ap_get_device_id().value, ap_get_boot_id().value, ap_get_next_transaction_id_ctr());
    if (err < 0) {
        return AP_ERR_INVALID_ARG;
    }
    if (err >= AP_MESSAGE_ID_MAX_LENGTH) {
        return AP_ERR_TRUNCATED;
    }
    return AP_OK;
}

int ap_init(const char *value) {
    ap_build_device_id(&device_id, value);
    ap_generate_random_boot_id(&boot_id);
    return AP_OK;
}

device_id_t ap_get_device_id() {
    return device_id;
}

boot_id_t ap_get_boot_id() {
    return boot_id;
}