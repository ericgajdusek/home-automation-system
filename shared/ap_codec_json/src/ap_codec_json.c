#include "ap_codec_json.h"

/* ------------------------- Helper functions ------------------------- */

static int json_add_string(cJSON *parent, const char *name, const char *value) {
    cJSON *item;
    if (parent == NULL || 
        name == NULL || 
        value == NULL) return -1;
    item = cJSON_CreateString(value);
    if (item == NULL) return -1;
    cJSON_AddItemToObject(parent, name, item);
    return 0;
}

static int json_add_u32(cJSON *parent, const char *name, uint32_t value) {
    cJSON *item;
    if (parent == NULL || name == NULL) return -1;
    item = cJSON_CreateNumber((double)value);
    if (item == NULL) return -1;
    cJSON_AddItemToObject(parent, name, item);
    return 0;
}

static int json_add_object(cJSON *parent, const char *name, cJSON **out_object) {
    cJSON *obj;
    if (parent == NULL || 
        name == NULL || 
        out_object == NULL) return -1;
    obj = cJSON_CreateObject();
    if (obj == NULL) return -1;
    cJSON_AddItemToObject(parent, name, obj);
    *out_object = obj;
    return 0;
}

static int copy_json_string(char *dst, size_t dst_size, const char *src) {
    size_t len;
    if (dst == NULL || 
        src == NULL || 
        dst_size == 0) return -1;
    len = strlen(src);
    if (len >= dst_size) return -1;
    memcpy(dst, src, len + 1);
    return 0;
}

static int json_get_required_object(const cJSON *parent, const char *name, const cJSON **out_obj) {
    const cJSON *item;
    if (parent == NULL || 
        name == NULL || 
        out_obj == NULL) return -1;
    item = cJSON_GetObjectItemCaseSensitive((cJSON *)parent, name);
    if (!cJSON_IsObject(item)) return -1;
    *out_obj = item;
    return 0;
}

static int json_read_string_field(const cJSON *parent, const char *name, char *dst, size_t dst_size) {
    const cJSON *item;
    if (parent == NULL || 
        name == NULL || 
        dst == NULL || 
        dst_size == 0) return -1;
    item = cJSON_GetObjectItemCaseSensitive((cJSON *)parent, name);
    if (!cJSON_IsString(item) || 
        item->valuestring == NULL) return -1;
    return copy_json_string(dst, dst_size, item->valuestring);
}

static int json_read_u8_field(const cJSON *parent, const char *name, uint8_t *out_value) {
    const cJSON *item;
    double d;
    uint32_t temp;
    if (parent == NULL || 
        name == NULL || 
        out_value == NULL) return -1;
    item = cJSON_GetObjectItemCaseSensitive((cJSON *)parent, name);
    if (!cJSON_IsNumber(item)) return -1;
    d = item->valuedouble;
    if (d < 0.0 || d > 255.0) return -1;
    temp = (uint32_t)d;
    if ((double)temp != d) return -1;
    *out_value = (uint8_t)temp;
    return 0;
}

static int json_read_u32_field(const cJSON *parent, const char *name, uint32_t *out_value) {
    const cJSON *item;
    double d;
    uint32_t temp;
     if (parent == NULL || 
        name == NULL || 
        out_value == NULL) return -1;
    item = cJSON_GetObjectItemCaseSensitive((cJSON *)parent, name);
    if (!cJSON_IsNumber(item)) return -1;
    d = item->valuedouble;
    if (d < 0.0 || d > 4294967295.0) return -1;
    temp = (uint32_t)d;
    if ((double)temp != d) return -1;
    *out_value = temp;
    return 0;
}

static int build_json_payload(cJSON *payload_json, const ap_message_t *msg) {
    if (payload_json == NULL || msg == NULL) return -1;
    switch (msg->action) {
        case AP_ACTION_NONE:
            return 0;
        case AP_ACTION_SET_LIGHTS:
            return json_add_u32(payload_json, "brightness", msg->payload.lights.brightness);
        case AP_ACTION_SET_BLINDS:
            return json_add_u32(payload_json, "position", msg->payload.blinds.position);
        default:
            return -1;
    }
}

static int build_json_result(cJSON *result_json, const ap_message_t *msg) {
    if (result_json == NULL || msg == NULL) return -1;
    if (json_add_u32(result_json, "code", msg->result.code) != 0)return -1;
    if (json_add_string(result_json, "message", msg->result.message) != 0) return -1;
    return 0;
}

static int parse_json_payload(const cJSON *payload_json, ap_message_t *out_msg) {
    uint8_t temp_u8;
    if (payload_json == NULL || out_msg == NULL) return -1;
    switch (out_msg->action) {
        case AP_ACTION_NONE:
            memset(&out_msg->payload, 0, sizeof(out_msg->payload));
            return 0;
        case AP_ACTION_SET_LIGHTS:
            if (json_read_u8_field(payload_json, "brightness", &temp_u8) != 0) return -1;
            out_msg->payload.lights.brightness = temp_u8;
            return 0;
        case AP_ACTION_SET_BLINDS:
            if (json_read_u8_field(payload_json, "position", &temp_u8) != 0) return -1;
            out_msg->payload.blinds.position = temp_u8;
            return 0;
        default:
            return -1;
    }
}

static int parse_json_result(const cJSON *result_json, ap_message_t *out_msg) {
    if (result_json == NULL || out_msg == NULL) return -1;

    if (json_read_u8_field(result_json, "code", &out_msg->result.code) != 0) return -1;

    if (json_read_string_field(result_json,
                               "message",
                               out_msg->result.message,
                               sizeof(out_msg->result.message)) != 0) return -1;
    return 0;
}

/*
Optional basic validation.
Tighten this later if you want stricter protocol rules.
*/
static int validate_message_basic(const ap_message_t *msg) {
    if (msg == NULL) return -1;
    switch (msg->action) {
        case AP_ACTION_NONE:
        case AP_ACTION_SET_LIGHTS:
        case AP_ACTION_SET_BLINDS:
            break;
        default:
            return -1;
    }
    return 0;
}

/* ------------------------- Serialization ------------------------- */

int ap_encode_json(const ap_message_t *msg, char *out_buf, size_t out_buf_size) {
    cJSON *root = NULL;
    cJSON *payload_json = NULL;
    cJSON *result_json = NULL;
    char *json_string = NULL;
    size_t json_len;
    int rc = -1;

    if (msg == NULL || 
        out_buf == NULL || 
        out_buf_size == 0) return -1;
    if (validate_message_basic(msg) != 0) return -1;
    root = cJSON_CreateObject();
    if (root == NULL) return -1;

    if (json_add_string(root, "target", msg->target.value) != 0) {
        goto cleanup;
    }
    if (json_add_string(root, "sender", msg->sender.value) != 0) {
        goto cleanup;
    }
    if (json_add_u32(root, "protocol_version_major", msg->protocol_version_major) != 0) {
        goto cleanup;
    }
    if (json_add_u32(root, "protocol_version_minor", msg->protocol_version_minor) != 0) {
        goto cleanup;
    }
    if (json_add_string(root, "message_id", msg->message_id.value) != 0) {
        goto cleanup;
    }
    if (json_add_string(root, "transaction_id", msg->transaction_id.value) != 0) {
        goto cleanup;
    }
    if (json_add_u32(root, "message_type", msg->message_type) != 0) {
        goto cleanup;
    }
    if (json_add_string(root, "ack_for_message_id", msg->ack_for_message_id.value) != 0) {
        goto cleanup;
    }
    if (json_add_u32(root, "action", msg->action) != 0) {
        goto cleanup;
    }
    if (json_add_object(root, "payload", &payload_json) != 0) {
        goto cleanup;
    }
    if (build_json_payload(payload_json, msg) != 0) {
        goto cleanup;
    }
    if (json_add_object(root, "result", &result_json) != 0) {
        goto cleanup;
    }
    if (build_json_result(result_json, msg) != 0) {
        goto cleanup;
    }
    if (json_add_u32(root, "retry_count", msg->retry_count) != 0) {
        goto cleanup;
    }
    if (json_add_u32(root, "timestamp_ms", msg->timestamp_ms) != 0) {
        goto cleanup;
    }
    json_string = cJSON_PrintUnformatted(root);
    if (json_string == NULL) {
        goto cleanup;
    }
    json_len = strlen(json_string);
    if (json_len + 1 > out_buf_size) {
        goto cleanup;
    }

    memcpy(out_buf, json_string, json_len + 1);
    rc = 0;

cleanup:
    if (json_string != NULL) {
        cJSON_free(json_string);
    }
    if (root != NULL) {
        cJSON_Delete(root);
    }

    return rc;
}

/* ------------------------- Deserialization ------------------------- */

int ap_decode_json(const uint8_t *buffer, size_t buffer_size, ap_message_t *out_msg)
{
    char *json_text = NULL;
    cJSON *root = NULL;
    const cJSON *payload_json = NULL;
    const cJSON *result_json = NULL;
    ap_message_t temp_msg;
    uint8_t temp_u8;
    uint32_t temp_u32;
    int rc = -1;

    if (buffer == NULL || 
        out_msg == NULL || 
        buffer_size == 0) return -1;

    memset(&temp_msg, 0, sizeof(temp_msg));
    json_text = (char *)malloc(buffer_size + 1);
    if (json_text == NULL) return -1;

    memcpy(json_text, buffer, buffer_size);
    json_text[buffer_size] = '\0';

    root = cJSON_Parse(json_text);
    if (root == NULL) {
        goto cleanup;
    }
    if (json_read_string_field(root, "target",
                               temp_msg.target.value,
                               sizeof(temp_msg.target.value)) != 0) {
        goto cleanup;
    }
    if (json_read_string_field(root, "sender",
                               temp_msg.sender.value,
                               sizeof(temp_msg.sender.value)) != 0) {
        goto cleanup;
    }
    if (json_read_u8_field(root, "protocol_version_major", &temp_msg.protocol_version_major) != 0) {
        goto cleanup;
    }
    if (json_read_u8_field(root, "protocol_version_minor", &temp_msg.protocol_version_minor) != 0) {
        goto cleanup;
    }
    if (json_read_string_field(root, "message_id",
                               temp_msg.message_id.value,
                               sizeof(temp_msg.message_id.value)) != 0) {
        goto cleanup;
    }
    if (json_read_string_field(root, "transaction_id",
                               temp_msg.transaction_id.value,
                               sizeof(temp_msg.transaction_id.value)) != 0) {
        goto cleanup;
    }
    if (json_read_u8_field(root, "message_type", &temp_u8) != 0) {
        goto cleanup;
    }
    temp_msg.message_type = (ap_message_type_t)temp_u8;
    if (json_read_string_field(root, "ack_for_message_id",
                               temp_msg.ack_for_message_id.value,
                               sizeof(temp_msg.ack_for_message_id.value)) != 0) {
        goto cleanup;
    }
    if (json_read_u8_field(root, "action", &temp_u8) != 0) {
        goto cleanup;
    }
    temp_msg.action = (ap_action_t)temp_u8;
    if (json_get_required_object(root, "payload", &payload_json) != 0) {
        goto cleanup;
    }
    if (json_get_required_object(root, "result", &result_json) != 0) {
        goto cleanup;
    }
    if (parse_json_payload(payload_json, &temp_msg) != 0) {
        goto cleanup;
    }
    if (parse_json_result(result_json, &temp_msg) != 0) {
        goto cleanup;
    }
    if (json_read_u8_field(root, "retry_count", &temp_msg.retry_count) != 0) {
        goto cleanup;
    }
    if (json_read_u32_field(root, "timestamp_ms", &temp_u32) != 0) {
        goto cleanup;
    }
    temp_msg.timestamp_ms = temp_u32;
    if (validate_message_basic(&temp_msg) != 0) {
        goto cleanup;
    }
    *out_msg = temp_msg;
    rc = 0;

cleanup:
    if (root != NULL) cJSON_Delete(root);
    free(json_text);

    return rc;
}