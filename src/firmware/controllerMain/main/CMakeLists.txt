idf_component_register(
    SRCS "main.c"
    INCLUDE_DIRS "include"
    REQUIRES waveshare__esp32_p4_wifi6_touch_lcd_7b ui event_handler net_link app_actions wifi_station nvs_flash
)
