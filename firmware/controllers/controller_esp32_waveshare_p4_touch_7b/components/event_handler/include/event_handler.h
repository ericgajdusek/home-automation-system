#pragma once

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "esp_log.h"
#include "lvgl.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    APP_EVENT_BTN = 0,
    APP_EVENT_ESPNOW,
    APP_EVENT_SENSOR,
} app_event_type_t;

typedef enum {
    BTN_LIGHT_TOGGLE = 0,
    BTN_BLINDS_TOGGLE,
    BTN_COUNT,
} btn_id_t;

typedef struct {
    btn_id_t id;
} btn_payload_t;

typedef struct {
    uint8_t mac[6];
    uint8_t cmd;
    int16_t value;
} espnow_payload_t;

typedef struct {
    float temp;
    float hum;
} sensor_payload_t;

typedef struct {
    app_event_type_t type;
    union {
        btn_payload_t    btn;
        espnow_payload_t espnow;
        sensor_payload_t sensor;
    } data;
} app_event_t;

extern QueueHandle_t app_event_queue;

void app_event_queue_init(void);
void btn_event_cb(lv_event_t * e);

#ifdef __cplusplus
}
#endif