#ifndef ESP32_TRANSPORT_MQTT_H
#define ESP32_TRANSPORT_MQTT_H

#include <stdint.h>
#include <stddef.h>
#include "mqtt_client.h"
#include "esp_log.h"

static const char *TAG = "esp32_transport_mqtt";
esp_mqtt_client_handle_t client;

int esp32_transport_mqtt_connect_to_broker(char *uri);
int esp32_transport_mqtt_enqueue(const char *topic, const char *payload, size_t payload_len);
int esp32_transport_mqtt_subscribe_to_topic(const char *topic);
int esp32_transport_mqtt_unsubscribe_from_topic(const char *topic);
int esp32_transport_mqtt_disconnect_from_broker(void);

#endif // ESP32_TRANSPORT_MQTT_H