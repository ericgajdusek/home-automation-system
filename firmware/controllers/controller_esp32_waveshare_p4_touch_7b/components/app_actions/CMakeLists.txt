idf_component_register(
    SRCS "src/app_actions.c"
    INCLUDE_DIRS "include"
    REQUIRES waveshare__esp32_p4_wifi6_touch_lcd_7b event_handler net_link
)
