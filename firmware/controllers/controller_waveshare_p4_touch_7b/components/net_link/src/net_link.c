#include "net_link.h"
#include "mqtt_client.h"
#include "esp_log.h"
#include <stdio.h>

static const char *TAG = "net_link";
static esp_mqtt_client_handle_t s_client = NULL;
static bool lights_on = true;
static bool blinds_open = true;
const char* payload;

esp_err_t net_link_init(void)
{
    esp_mqtt_client_config_t cfg = {
        .broker.address.uri = CONFIG_APP_MQTT_URI,
    };

    s_client = esp_mqtt_client_init(&cfg);
    if (!s_client) return ESP_FAIL;

    ESP_ERROR_CHECK(esp_mqtt_client_start(s_client));
    ESP_LOGI(TAG, "MQTT started");
    return ESP_OK;
}

esp_err_t net_link_publish_lights_toggle(void)
{
    if (!s_client) return ESP_ERR_INVALID_STATE;
    if (lights_on) {
        payload = "OFF";
    } else {
        payload = "ON";
    }
    lights_on = !lights_on;
    int msg_id = esp_mqtt_client_publish(s_client, "home/lights/command", payload, 0, 1, 0);
    return (msg_id >= 0) ? ESP_OK : ESP_FAIL;
}

esp_err_t net_link_publish_blinds_move_steps(int steps)
{
    if (!s_client) return ESP_ERR_INVALID_STATE;

    if (blinds_open) {
        payload = "CLOSE";
    } else {
        payload = "OPEN";
    }
    blinds_open = !blinds_open;

    int msg_id = esp_mqtt_client_publish(s_client, "home/blinds/command", payload, 0, 1, 0);
    return (msg_id >= 0) ? ESP_OK : ESP_FAIL;
}