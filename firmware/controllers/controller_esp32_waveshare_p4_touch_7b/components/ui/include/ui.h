#pragma once

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "esp_log.h"

#include "lvgl.h"
#include "event_handler.h"
#include "bsp/esp-bsp.h"
#include "bsp/display.h"

#ifdef __cplusplus
extern "C" {
#endif

void create_buttons(lv_obj_t * scr);
void ui_create(void);
void ui_init(void);

#ifdef __cplusplus
}
#endif