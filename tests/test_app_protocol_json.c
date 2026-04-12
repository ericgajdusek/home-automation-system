#include "unity.h"
#include "app_protocol_json.h"
#include <string.h>

void setUp(void) {}
void tearDown(void) {}

void test_message_round_trip_lights(void)
{
    message_t msg = {0};
    message_t parsed = {0};
    payload_t payload = {0};
    lights_payload_t lights_payload = {0};
    device_id_t target = {0};
    message_id_t message_id = {0};
    transaction_id_t transaction_id = {0};
    message_type_t message_type = MSG_TYPE_COMMAND;
    action_t action = ACTION_SET_LIGHTS;
    char json[512];

    build_device_id(&target, "lights-01", "A3FM45");
    build_device_id(&device_id, "ctrl-01", "B7G8H9");
    generate_random_boot_id(&boot_id);
    build_message_id(&message_id, device_id, boot_id);
    build_lights_payload(&payload, 40);
    build_new_transaction_message(&msg, target, message_type, action, payload);

    strcpy(msg.target.id, target.id);
    strcpy(msg.sender.id, device_id.id);
    msg.protocol_version_major = get_protocol_version_major();
    msg.protocol_version_minor = get_protocol_version_minor();
    strcpy(msg.message_id.id, message_id.id);
    strcpy(msg.transaction_id.id, transaction_id.id);
    msg.message_type = message_type;
    msg.action = action;
    msg.payload = payload;
    msg.result.code = 0;
    msg.result.message[0] = '\0';
    msg.retry_count = 0;
    msg.timestamp_ms = 12345;

    TEST_ASSERT_EQUAL_INT(0, app_protocol_message_to_json(&msg, json, sizeof(json)));
    TEST_ASSERT_EQUAL_INT(
        0,
        app_protocol_message_from_json((const uint8_t *)json, strlen(json), &parsed)
    );

    TEST_ASSERT_EQUAL_STRING("lights-01_A3FM45", parsed.target.id);
    TEST_ASSERT_EQUAL_STRING("ctrl-01_B7G8H9", parsed.sender.id);
    TEST_ASSERT_EQUAL_UINT8(MSG_TYPE_COMMAND, parsed.message_type);
    TEST_ASSERT_EQUAL_UINT8(ACTION_SET_LIGHTS, parsed.action);
    TEST_ASSERT_EQUAL_UINT8(40, parsed.payload.lights.brightness);
    TEST_ASSERT_EQUAL_UINT8(0, parsed.result.code);
    TEST_ASSERT_EQUAL_STRING("", parsed.result.message);
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_message_round_trip_lights);
    return UNITY_END();
}