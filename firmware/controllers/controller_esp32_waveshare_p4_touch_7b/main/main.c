#include "app_actions.h"
#include "event_handler.h"
#include "ui.h"
#include "net_link.h"
#include "wifi_station.h"

#include "esp_log.h"
#include "esp_event.h"
#include "esp_netif.h"
#include "nvs_flash.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "freertos/event_groups.h"

static const char *TAG = "main";

/* Wait-until-connected bit */
static EventGroupHandle_t s_wifi_event_group;
#define WIFI_GOT_IP_BIT BIT0


static void net_task(void *arg)
{
    (void)arg;

    ESP_ERROR_CHECK(wifi_station_init());
    ESP_LOGI(TAG, "Waiting for IP...");
    ESP_ERROR_CHECK(wifi_station_wait_for_ip());

    // now start MQTT
    ESP_ERROR_CHECK(net_link_init());
    ESP_LOGI(TAG, "Network + MQTT ready");

    vTaskDelete(NULL);
}

static void app_event_loop_task(void *arg)
{
    (void)arg;
    app_event_t event;

    while (true) {
        if (xQueueReceive(app_event_queue, &event, portMAX_DELAY) == pdTRUE) {
            switch (event.type) {
            case APP_EVENT_BTN:
                ESP_LOGI(TAG, "Button event id=%d", event.data.btn.id);

                switch (event.data.btn.id) {
                case BTN_LIGHT_TOGGLE:
                    action_toggle_lights();
                    break;
                case BTN_BLINDS_TOGGLE:
                    action_toggle_blinds();
                    break;
                default:
                    ESP_LOGW(TAG, "Unknown button id=%d", event.data.btn.id);
                    break;
                }
                break;

            default:
                ESP_LOGW(TAG, "Unknown event type=%d", event.type);
                break;
            }
        }
    }
}

void app_main(void)
{
    app_event_queue_init();

    ESP_ERROR_CHECK(nvs_flash_init());
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());

    ui_init();  // SHOW UI immediately (e.g., “Connecting…” screen)

    // Start Wi-Fi + MQTT without blocking UI
    xTaskCreate(net_task, "net_task", 6144, NULL, 5, NULL);

    /* Now actions can safely publish (MQTT client exists) */
    actions_init();

    xTaskCreate(app_event_loop_task, "app_event_loop", 4096, NULL, 5, NULL);

    ESP_LOGI(TAG, "System ready");
}