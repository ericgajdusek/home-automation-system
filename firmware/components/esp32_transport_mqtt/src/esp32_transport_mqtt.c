#include "esp32_transport_mqtt.h"

int esp32_transport_mqtt_connect_to_broker(char *uri) {
    esp_mqtt_client_config_t cfg = {
        .broker.address.uri = uri,
    };
    esp_mqtt_client_handle_t client = esp_mqtt_client_init(&cfg);
    return esp_mqtt_client_start(client);
}

int esp32_transport_mqtt_enqueue(const char *topic, const char *payload, size_t payload_len) {
    int ret = esp_mqtt_client_enqueue(client, topic, payload, payload_len, 1, 0);
    return (ret >= 0) ? ESP_OK : ESP_FAIL;
}

int esp32_transport_mqtt_subscribe_to_topic(const char *topic) {
    return esp_mqtt_client_subscribe(client, topic, 1);
}

int esp32_transport_mqtt_unsubscribe_from_topic(const char *topic) {
    return esp_mqtt_client_unsubscribe(client, topic);
}

int esp32_transport_mqtt_disconnect_from_broker(void) {
    return esp_mqtt_client_stop(client);
}