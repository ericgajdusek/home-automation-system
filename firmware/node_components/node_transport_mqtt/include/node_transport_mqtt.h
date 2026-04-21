#ifndef NODE_TRANSPORT_MQTT_H
#define NODE_TRANSPORT_MQTT_H

#include <stdint.h>
#include <stddef.h>
#include "mqtt_client.h"
#include "esp_log.h"

typedef struct {
    char topic[TOPIC_MAX_LEN];
} mqtt_address_t;

static const char *TAG = "node_transport_mqtt";
esp_mqtt_client_handle_t client;

int node_transport_mqtt_connect_to_broker(char *uri);
int node_transport_mqtt_enqueue(const char *topic, const char *payload, size_t payload_len);
int node_transport_mqtt_subscribe_to_topic(const char *topic);
int node_transport_mqtt_unsubscribe_from_topic(const char *topic);
int node_transport_mqtt_disconnect_from_broker(void);

#endif // NODE_TRANSPORT_MQTT_H
