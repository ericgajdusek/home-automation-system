#include "unity.h"
#include "ap_codec_json.h"
#include <string.h>

void setUp(void) {}
void tearDown(void) {}

void test_message_round_trip_lights(void)
{
    ap_message_t msg = {0};
    ap_message_t parsed = {0};
    ap_payload_t payload = {0};
    ap_result_t result = {0};
    ap_device_id_t target = {0};
    ap_transaction_id_t transaction_id = {0};
    ap_message_type_t message_type = AP_MSG_TYPE_COMMAND;
    ap_action_t action = AP_ACTION_SET_LIGHTS;
    char json[512];

    ap_init("ctrl-01_B7G8H9");
    ap_build_device_id(&target, "lights-01_A3FM45");
    ap_build_transaction_id(&transaction_id);
    ap_build_lights_payload(&payload, 40);
    ap_build_result(&result, 0, "");

    ap_build_message(&msg, target, transaction_id, message_type, (ap_message_id_t){0}, action, payload, result, 0);

    TEST_ASSERT_EQUAL_INT(0, ap_encode_json(&msg, json, sizeof(json)));
    
    TEST_ASSERT_EQUAL_INT(
        0,
        ap_decode_json((const uint8_t *)json, strlen(json), &parsed)
    );

    TEST_ASSERT_EQUAL_STRING(msg.target.value, parsed.target.value);
    TEST_ASSERT_EQUAL_STRING("lights-01_A3FM45", parsed.target.value);
    TEST_ASSERT_EQUAL_STRING(msg.sender.value, parsed.sender.value);
    TEST_ASSERT_EQUAL_STRING("ctrl-01_B7G8H9", parsed.sender.value);
    TEST_ASSERT_EQUAL_UINT8(msg.protocol_version_major, parsed.protocol_version_major);
    TEST_ASSERT_EQUAL_UINT8(1, parsed.protocol_version_major);
    TEST_ASSERT_EQUAL_UINT8(msg.protocol_version_minor, parsed.protocol_version_minor);
    TEST_ASSERT_EQUAL_UINT8(0, parsed.protocol_version_minor);
    TEST_ASSERT_EQUAL_STRING(msg.message_id.value, parsed.message_id.value);
    TEST_ASSERT_EQUAL_STRING(msg.transaction_id.value, parsed.transaction_id.value);
    TEST_ASSERT_EQUAL_UINT8(msg.message_type, parsed.message_type);
    TEST_ASSERT_EQUAL_UINT8(AP_MSG_TYPE_COMMAND, parsed.message_type);
    TEST_ASSERT_EQUAL_STRING(msg.ack_for_message_id.value, parsed.ack_for_message_id.value);
    TEST_ASSERT_EQUAL_UINT8(msg.action, parsed.action);
    TEST_ASSERT_EQUAL_UINT8(AP_ACTION_SET_LIGHTS, parsed.action);
    TEST_ASSERT_EQUAL_UINT8(msg.payload.lights.brightness, parsed.payload.lights.brightness);
    TEST_ASSERT_EQUAL_UINT8(40, parsed.payload.lights.brightness);
    TEST_ASSERT_EQUAL_UINT8(msg.result.code, parsed.result.code);
    TEST_ASSERT_EQUAL_UINT8(0, parsed.result.code);
    TEST_ASSERT_EQUAL_STRING(msg.result.message, parsed.result.message);
    TEST_ASSERT_EQUAL_STRING("", parsed.result.message);
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_message_round_trip_lights);
    return UNITY_END();
}
