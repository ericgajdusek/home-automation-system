#include "app_actions.h"
#include "net_link.h"
#include "esp_log.h"
#include "event_handler.h"

static const char *TAG = "actions";

/* Same behavior as your Arduino controller */
static const int32_t STEPS_PER_TAP = 1200;

/* Track blinds direction toggle */
static bool s_dir_toggle = false;

void actions_init(void)
{
    s_dir_toggle = false;
    ESP_LOGI(TAG, "Actions initialized");
}

/* Toggle lights via MQTT */
void action_toggle_lights(void)
{
    esp_err_t err = net_link_publish_lights_toggle();

    if (err == ESP_OK) {
        ESP_LOGI(TAG, "Lights toggle published");
    } else {
        ESP_LOGW(TAG, "Lights toggle failed: %s", esp_err_to_name(err));
    }
}

/* Toggle blinds direction & send step move */
void action_toggle_blinds(void)
{
    s_dir_toggle = !s_dir_toggle;
    int32_t steps = s_dir_toggle ? STEPS_PER_TAP : -STEPS_PER_TAP;

    esp_err_t err = net_link_publish_blinds_move_steps(steps);

    if (err == ESP_OK) {
        ESP_LOGI(TAG, "Blinds move published (%ld steps)", (long)steps);
    } else {
        ESP_LOGW(TAG, "Blinds move failed: %s", esp_err_to_name(err));
    }
}