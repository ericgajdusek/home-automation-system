#include "ui.h"

static const char *TAG = "ui";

void create_buttons(lv_obj_t * scr) {
    static const struct {
        const char * label;
        int16_t y_offset;
        btn_id_t id;
    } btn_desc[BTN_COUNT] = {
        { "Toggle Lights", -30, BTN_LIGHT_TOGGLE },
        { "Toggle Blinds", 30, BTN_BLINDS_TOGGLE },
    };

    for (int i = 0; i < BTN_COUNT; i++) {
        lv_obj_t * btn = lv_btn_create(scr);
        lv_obj_align(btn, LV_ALIGN_CENTER, 0, btn_desc[i].y_offset);

        lv_obj_t * label = lv_label_create(btn);
        lv_label_set_text(label, btn_desc[i].label);
        lv_obj_center(label);

        // Attach callback + pass which button it is
        lv_obj_add_event_cb(btn, btn_event_cb, LV_EVENT_CLICKED,
                            (void*)(uintptr_t)btn_desc[i].id);
    }
}

void ui_create(void) {
    lv_obj_t *scr = lv_scr_act();
    create_buttons(scr);
}

void ui_init(void) {
    ESP_LOGI(TAG, "Initializing UI");
    bsp_display_cfg_t cfg = {
        .lvgl_port_cfg = ESP_LVGL_PORT_INIT_CONFIG(),
        .buffer_size = BSP_LCD_DRAW_BUFF_SIZE,
        .double_buffer = BSP_LCD_DRAW_BUFF_DOUBLE,
        .flags = {
            .buff_dma = true,
            .buff_spiram = false,
            .sw_rotate = false,
        }
    };
    lv_display_t *disp = bsp_display_start_with_config(&cfg);
    if (!disp) {
        ESP_LOGE(TAG, "bsp_display_start_with_config failed");
        return;
    }
    bsp_display_backlight_on();
    ui_create();
}