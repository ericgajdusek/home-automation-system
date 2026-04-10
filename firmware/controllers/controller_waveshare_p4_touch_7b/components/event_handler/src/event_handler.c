#include "event_handler.h"

static const char *TAG = "event_handler";
QueueHandle_t app_event_queue = NULL;

void app_event_queue_init(void) {
    app_event_queue = xQueueCreate(
        10,                 // length (how many events can buffer)
        sizeof(app_event_t) // item size
    );
}

void btn_event_cb(lv_event_t * e) {
    if (lv_event_get_code(e) != LV_EVENT_CLICKED) return;

    app_event_t event = {
        .type = APP_EVENT_BTN,
        .data.btn.id = (btn_id_t)(uintptr_t)lv_event_get_user_data(e),
    };

    ESP_LOGI(TAG, "Button pressed: ID=%d", event.data.btn.id);

    xQueueSend(app_event_queue, &event, 0);
}