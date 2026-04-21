#include "unity.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ap_message.h"
#include "ap_message_ids.h"
#include "ap_message_payloads.h"
#include "ap_message_results.h"
#include "ap_message_validate.h"
#include "ap_version.h"

void setUp(void) {}
void tearDown(void) {}

static void assert_alnum_string(const char *value, size_t expected_len)
{
    size_t i;

    TEST_ASSERT_NOT_NULL(value);
    TEST_ASSERT_EQUAL_UINT(expected_len, strlen(value));

    for (i = 0; i < expected_len; ++i) {
        TEST_ASSERT_TRUE(isalnum((unsigned char)value[i]));
    }
}

static int extract_counter_suffix(const char *value)
{
    const char *suffix = strrchr(value, '_');

    TEST_ASSERT_NOT_NULL(suffix);
    TEST_ASSERT_NOT_EQUAL('\0', suffix[1]);
    return atoi(suffix + 1);
}

static void assert_generated_id_prefix(const char *value, const char *device_id, const char *boot_id)
{
    char expected_prefix[AP_MESSAGE_ID_MAX_LENGTH];
    int prefix_len = snprintf(expected_prefix, sizeof(expected_prefix), "%s_%s_", device_id, boot_id);

    TEST_ASSERT_TRUE(prefix_len > 0);
    TEST_ASSERT_TRUE(prefix_len < (int)sizeof(expected_prefix));
    TEST_ASSERT_EQUAL_MEMORY(expected_prefix, value, (size_t)prefix_len);
}

static void build_valid_command_message(ap_message_t *msg, ap_transaction_id_t *transaction_id_out)
{
    ap_device_id_t target = {0};
    ap_transaction_id_t transaction_id = {0};
    ap_payload_t payload = {0};
    ap_result_t result = {0};

    TEST_ASSERT_EQUAL_INT(AP_OK, ap_init("ctrl-01_B7G8H9"));
    TEST_ASSERT_EQUAL_INT(AP_OK, ap_build_device_id(&target, "lights-01_A3FM45"));
    TEST_ASSERT_EQUAL_INT(AP_OK, ap_build_transaction_id(&transaction_id));
    TEST_ASSERT_EQUAL_INT(AP_OK, ap_build_lights_payload(&payload, 42));
    TEST_ASSERT_EQUAL_INT(AP_OK, ap_build_result(&result, 0, "ok"));
    TEST_ASSERT_EQUAL_INT(
        AP_OK,
        ap_build_message(
            msg,
            target,
            transaction_id,
            AP_MSG_TYPE_COMMAND,
            (ap_message_id_t){0},
            AP_ACTION_SET_LIGHTS,
            payload,
            result,
            2));

    if (transaction_id_out != NULL) {
        *transaction_id_out = transaction_id;
    }
}

void test_ap_get_protocol_version_returns_compile_time_constants(void)
{
    TEST_ASSERT_EQUAL_UINT8(AP_VERSION_MAJOR, ap_get_protocol_version_major());
    TEST_ASSERT_EQUAL_UINT8(AP_VERSION_MINOR, ap_get_protocol_version_minor());
}

void test_ap_build_device_id_copies_a_valid_value(void)
{
    ap_device_id_t device_id = {0};

    TEST_ASSERT_EQUAL_INT(AP_OK, ap_build_device_id(&device_id, "lights-01_A3FM45"));
    TEST_ASSERT_EQUAL_STRING("lights-01_A3FM45", device_id.value);
}

void test_ap_build_device_id_validates_null_arguments(void)
{
    ap_device_id_t device_id = {0};

    TEST_ASSERT_EQUAL_INT(AP_ERR_NULL_PTR, ap_build_device_id(NULL, "lights-01_A3FM45"));
    TEST_ASSERT_EQUAL_INT(AP_ERR_NULL_PTR, ap_build_device_id(&device_id, NULL));
}

void test_ap_build_device_id_rejects_truncated_values(void)
{
    ap_device_id_t device_id = {0};

    TEST_ASSERT_EQUAL_INT(
        AP_ERR_TRUNCATED,
        ap_build_device_id(&device_id, "12345678901234567890"));
}

void test_ap_generate_random_boot_id_builds_an_alphanumeric_value(void)
{
    ap_boot_id_t boot_id = {0};

    srand(1);
    TEST_ASSERT_EQUAL_INT(AP_OK, ap_generate_random_boot_id(&boot_id));
    assert_alnum_string(boot_id.value, AP_BOOT_ID_MAX_LENGTH - 1);
}

void test_ap_generate_random_boot_id_requires_output_storage(void)
{
    TEST_ASSERT_EQUAL_INT(AP_ERR_NULL_PTR, ap_generate_random_boot_id(NULL));
}

void test_ap_init_sets_the_current_device_and_boot_ids(void)
{
    ap_device_id_t device_id;
    ap_boot_id_t boot_id;

    srand(2);
    TEST_ASSERT_EQUAL_INT(AP_OK, ap_init("ctrl-01_B7G8H9"));

    device_id = ap_get_device_id();
    boot_id = ap_get_boot_id();

    TEST_ASSERT_EQUAL_STRING("ctrl-01_B7G8H9", device_id.value);
    assert_alnum_string(boot_id.value, AP_BOOT_ID_MAX_LENGTH - 1);
}

void test_ap_build_message_id_uses_the_supplied_sender_and_increments(void)
{
    ap_device_id_t sender = {0};
    ap_boot_id_t boot_id;
    ap_message_id_t first = {0};
    ap_message_id_t second = {0};
    int first_counter;
    int second_counter;

    srand(3);
    TEST_ASSERT_EQUAL_INT(AP_OK, ap_init("ctrl-01_B7G8H9"));
    boot_id = ap_get_boot_id();
    TEST_ASSERT_EQUAL_INT(AP_OK, ap_build_device_id(&sender, "lights-01_A3FM45"));

    TEST_ASSERT_EQUAL_INT(AP_OK, ap_build_message_id(&first, sender));
    TEST_ASSERT_EQUAL_INT(AP_OK, ap_build_message_id(&second, sender));

    assert_generated_id_prefix(first.value, sender.value, boot_id.value);
    assert_generated_id_prefix(second.value, sender.value, boot_id.value);

    first_counter = extract_counter_suffix(first.value);
    second_counter = extract_counter_suffix(second.value);

    TEST_ASSERT_EQUAL_INT(first_counter + 1, second_counter);
}

void test_ap_build_message_id_requires_output_storage(void)
{
    ap_device_id_t sender = {0};

    TEST_ASSERT_EQUAL_INT(AP_OK, ap_build_device_id(&sender, "lights-01_A3FM45"));
    TEST_ASSERT_EQUAL_INT(AP_ERR_NULL_PTR, ap_build_message_id(NULL, sender));
}

void test_ap_build_transaction_id_uses_the_current_device_and_increments(void)
{
    ap_device_id_t device_id;
    ap_boot_id_t boot_id;
    ap_transaction_id_t first = {0};
    ap_transaction_id_t second = {0};
    int first_counter;
    int second_counter;

    srand(4);
    TEST_ASSERT_EQUAL_INT(AP_OK, ap_init("ctrl-01_B7G8H9"));
    device_id = ap_get_device_id();
    boot_id = ap_get_boot_id();

    TEST_ASSERT_EQUAL_INT(AP_OK, ap_build_transaction_id(&first));
    TEST_ASSERT_EQUAL_INT(AP_OK, ap_build_transaction_id(&second));

    assert_generated_id_prefix(first.value, device_id.value, boot_id.value);
    assert_generated_id_prefix(second.value, device_id.value, boot_id.value);

    first_counter = extract_counter_suffix(first.value);
    second_counter = extract_counter_suffix(second.value);

    TEST_ASSERT_EQUAL_INT(first_counter + 1, second_counter);
}

void test_ap_build_transaction_id_requires_output_storage(void)
{
    TEST_ASSERT_EQUAL_INT(AP_ERR_NULL_PTR, ap_build_transaction_id(NULL));
}

void test_ap_build_lights_payload_sets_brightness(void)
{
    ap_payload_t payload = {0};

    TEST_ASSERT_EQUAL_INT(AP_OK, ap_build_lights_payload(&payload, 100));
    TEST_ASSERT_EQUAL_UINT8(100, payload.lights.brightness);
}

void test_ap_build_lights_payload_validates_arguments(void)
{
    ap_payload_t payload = {0};

    TEST_ASSERT_EQUAL_INT(AP_ERR_INVALID_ARG, ap_build_lights_payload(NULL, 50));
    TEST_ASSERT_EQUAL_INT(AP_ERR_INVALID_ARG, ap_build_lights_payload(&payload, 101));
}

void test_ap_build_blinds_payload_sets_position(void)
{
    ap_payload_t payload = {0};

    TEST_ASSERT_EQUAL_INT(AP_OK, ap_build_blinds_payload(&payload, 100));
    TEST_ASSERT_EQUAL_UINT8(100, payload.blinds.position);
}

void test_ap_build_blinds_payload_validates_arguments(void)
{
    ap_payload_t payload = {0};

    TEST_ASSERT_EQUAL_INT(AP_ERR_NULL_PTR, ap_build_blinds_payload(NULL, 50));
    TEST_ASSERT_EQUAL_INT(AP_ERR_INVALID_ARG, ap_build_blinds_payload(&payload, 101));
}

void test_ap_build_result_sets_code_and_message(void)
{
    ap_result_t result = {0};

    TEST_ASSERT_EQUAL_INT(AP_OK, ap_build_result(&result, 7, "accepted"));
    TEST_ASSERT_EQUAL_UINT8(7, result.code);
    TEST_ASSERT_EQUAL_STRING("accepted", result.message);
}

void test_ap_build_result_validates_arguments_and_truncation(void)
{
    ap_result_t result = {0};
    char long_message[80];

    memset(long_message, 'a', sizeof(long_message) - 1);
    long_message[sizeof(long_message) - 1] = '\0';

    TEST_ASSERT_EQUAL_INT(AP_ERR_NULL_PTR, ap_build_result(NULL, 0, "ok"));
    TEST_ASSERT_EQUAL_INT(AP_ERR_NULL_PTR, ap_build_result(&result, 0, NULL));
    TEST_ASSERT_EQUAL_INT(AP_ERR_TRUNCATED, ap_build_result(&result, 0, long_message));
}

void test_ap_build_message_populates_fields_from_inputs_and_runtime_state(void)
{
    ap_message_t msg = {0};
    ap_transaction_id_t transaction_id = {0};
    ap_boot_id_t boot_id;

    build_valid_command_message(&msg, &transaction_id);
    boot_id = ap_get_boot_id();

    TEST_ASSERT_EQUAL_STRING("lights-01_A3FM45", msg.target.value);
    TEST_ASSERT_EQUAL_STRING("ctrl-01_B7G8H9", msg.sender.value);
    TEST_ASSERT_EQUAL_UINT8(ap_get_protocol_version_major(), msg.protocol_version_major);
    TEST_ASSERT_EQUAL_UINT8(ap_get_protocol_version_minor(), msg.protocol_version_minor);
    TEST_ASSERT_EQUAL_STRING(transaction_id.value, msg.transaction_id.value);
    TEST_ASSERT_EQUAL_UINT8(AP_MSG_TYPE_COMMAND, msg.message_type);
    TEST_ASSERT_EQUAL_STRING("", msg.ack_for_message_id.value);
    TEST_ASSERT_EQUAL_UINT8(AP_ACTION_SET_LIGHTS, msg.action);
    TEST_ASSERT_EQUAL_UINT8(42, msg.payload.lights.brightness);
    TEST_ASSERT_EQUAL_UINT8(0, msg.result.code);
    TEST_ASSERT_EQUAL_STRING("ok", msg.result.message);
    TEST_ASSERT_EQUAL_UINT32(0, msg.timestamp_ms);
    TEST_ASSERT_EQUAL_UINT8(2, msg.retry_count);
    TEST_ASSERT_NOT_EQUAL('\0', msg.message_id.value[0]);
    assert_generated_id_prefix(msg.message_id.value, "ctrl-01_B7G8H9", boot_id.value);
}

void test_ap_build_ack_message_targets_the_original_sender(void)
{
    ap_message_t original = {0};
    ap_message_t ack = {0};

    srand(5);
    TEST_ASSERT_EQUAL_INT(AP_OK, ap_init("lights-01_A3FM45"));
    TEST_ASSERT_EQUAL_INT(AP_OK, ap_build_device_id(&original.sender, "ctrl-01_B7G8H9"));
    TEST_ASSERT_EQUAL_INT(AP_OK, ap_build_device_id(&original.target, "lights-01_A3FM45"));
    snprintf(original.message_id.value, sizeof(original.message_id.value), "%s", "remote_msg_9");
    snprintf(original.transaction_id.value, sizeof(original.transaction_id.value), "%s", "remote_tx_3");

    TEST_ASSERT_EQUAL_INT(AP_OK, ap_build_ack_message(&ack, &original));

    TEST_ASSERT_EQUAL_STRING("ctrl-01_B7G8H9", ack.target.value);
    TEST_ASSERT_EQUAL_STRING("lights-01_A3FM45", ack.sender.value);
    TEST_ASSERT_EQUAL_UINT8(ap_get_protocol_version_major(), ack.protocol_version_major);
    TEST_ASSERT_EQUAL_UINT8(ap_get_protocol_version_minor(), ack.protocol_version_minor);
    TEST_ASSERT_EQUAL_UINT8(AP_MSG_TYPE_ACK, ack.message_type);
    TEST_ASSERT_EQUAL_STRING(original.message_id.value, ack.ack_for_message_id.value);
    TEST_ASSERT_EQUAL_STRING(original.transaction_id.value, ack.transaction_id.value);
    TEST_ASSERT_EQUAL_UINT8(AP_ACTION_NONE, ack.action);
    TEST_ASSERT_EQUAL_UINT8(0, ack.retry_count);
    TEST_ASSERT_NOT_EQUAL('\0', ack.message_id.value[0]);
}

void test_ap_validate_message_accepts_a_valid_message(void)
{
    ap_message_t msg = {0};

    build_valid_command_message(&msg, NULL);
    TEST_ASSERT_EQUAL_INT(AP_OK, ap_validate_message(&msg));
}

void test_ap_validate_message_rejects_null_messages(void)
{
    TEST_ASSERT_EQUAL_INT(AP_ERR_NULL_PTR, ap_validate_message(NULL));
}

void test_ap_validate_message_rejects_wrong_protocol_versions(void)
{
    ap_message_t msg = {0};

    build_valid_command_message(&msg, NULL);
    msg.protocol_version_major++;

    TEST_ASSERT_EQUAL_INT(AP_ERR_INVALID_ARG, ap_validate_message(&msg));
}

void test_ap_validate_message_rejects_missing_identifiers(void)
{
    ap_message_t msg = {0};

    build_valid_command_message(&msg, NULL);
    msg.message_id.value[0] = '\0';
    TEST_ASSERT_EQUAL_INT(AP_ERR_INVALID_ARG, ap_validate_message(&msg));

    build_valid_command_message(&msg, NULL);
    msg.transaction_id.value[0] = '\0';
    TEST_ASSERT_EQUAL_INT(AP_ERR_INVALID_ARG, ap_validate_message(&msg));
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_ap_get_protocol_version_returns_compile_time_constants);
    RUN_TEST(test_ap_build_device_id_copies_a_valid_value);
    RUN_TEST(test_ap_build_device_id_validates_null_arguments);
    RUN_TEST(test_ap_build_device_id_rejects_truncated_values);
    RUN_TEST(test_ap_generate_random_boot_id_builds_an_alphanumeric_value);
    RUN_TEST(test_ap_generate_random_boot_id_requires_output_storage);
    RUN_TEST(test_ap_init_sets_the_current_device_and_boot_ids);
    RUN_TEST(test_ap_build_message_id_uses_the_supplied_sender_and_increments);
    RUN_TEST(test_ap_build_message_id_requires_output_storage);
    RUN_TEST(test_ap_build_transaction_id_uses_the_current_device_and_increments);
    RUN_TEST(test_ap_build_transaction_id_requires_output_storage);
    RUN_TEST(test_ap_build_lights_payload_sets_brightness);
    RUN_TEST(test_ap_build_lights_payload_validates_arguments);
    RUN_TEST(test_ap_build_blinds_payload_sets_position);
    RUN_TEST(test_ap_build_blinds_payload_validates_arguments);
    RUN_TEST(test_ap_build_result_sets_code_and_message);
    RUN_TEST(test_ap_build_result_validates_arguments_and_truncation);
    RUN_TEST(test_ap_build_message_populates_fields_from_inputs_and_runtime_state);
    RUN_TEST(test_ap_build_ack_message_targets_the_original_sender);
    RUN_TEST(test_ap_validate_message_accepts_a_valid_message);
    RUN_TEST(test_ap_validate_message_rejects_null_messages);
    RUN_TEST(test_ap_validate_message_rejects_wrong_protocol_versions);
    RUN_TEST(test_ap_validate_message_rejects_missing_identifiers);
    return UNITY_END();
}
