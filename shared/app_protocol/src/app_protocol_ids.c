#include "app_protocol_ids.h"

int next_message_id_ctr = 0;
int next_transaction_id_ctr = 0;

int get_next_message_id_ctr() {
    return next_message_id_ctr++;
}

int get_next_transaction_id_ctr() {
    return next_transaction_id_ctr++;
}

int build_device_id(device_id_t *out, const char *device_name, const char *mac_suffix) {
    if (!out || !device_name || !mac_suffix) {
        return PROTOCOL_ERR_NULL_PTR;
    }

    int err = snprintf(out->id, DEVICE_ID_MAX_LENGTH, "%s_%s", device_name, mac_suffix);

    if (err < 0) {
        return PROTOCOL_ERR_INVALID_ARG;
    }

    if (err >= DEVICE_ID_MAX_LENGTH) {
        return PROTOCOL_ERR_TRUNCATED;
    }

    return PROTOCOL_OK;
}

int generate_random_boot_id(boot_id_t *out) {
    if (!out) {
        return PROTOCOL_ERR_NULL_PTR;
    }
    int err = snprintf(out->id, BOOT_ID_MAX_LENGTH, "boot_id_placeholder");
    if (err < 0) {
        return PROTOCOL_ERR_INVALID_ARG;
    }
    if (err >= BOOT_ID_MAX_LENGTH) {
        return PROTOCOL_ERR_TRUNCATED;
    }
    return PROTOCOL_OK;
}

int build_message_id(message_id_t *out, device_id_t sender_id, boot_id_t boot_id) {
    if (!out) {
        return PROTOCOL_ERR_NULL_PTR;
    }
    int err = snprintf(out->id, MESSAGE_ID_MAX_LENGTH, "%s_%s_%d", sender_id.id, boot_id.id, get_next_message_id_ctr());
    if (err < 0) {
        return PROTOCOL_ERR_INVALID_ARG;
    }
    if (err >= MESSAGE_ID_MAX_LENGTH) {
        return PROTOCOL_ERR_TRUNCATED;
    }
    return PROTOCOL_OK;
}

int build_transaction_id(transaction_id_t *out, device_id_t sender_id, boot_id_t boot_id) {
    if (!out) {
        return PROTOCOL_ERR_NULL_PTR;
    }
    int err = snprintf(out->id, TRANSACTION_ID_MAX_LENGTH, "%s_%s_%d", sender_id.id, boot_id.id, get_next_transaction_id_ctr());
    if (err < 0) {
        return PROTOCOL_ERR_INVALID_ARG;
    }
    if (err >= TRANSACTION_ID_MAX_LENGTH) {
        return PROTOCOL_ERR_TRUNCATED;
    }
    return PROTOCOL_OK;
}

device_id_t get_device_id() {
    return device_id;
}

boot_id_t get_boot_id() {
    return boot_id;
}