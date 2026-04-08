idf_component_register(
    SRCS "src/wifi_station.c"
    INCLUDE_DIRS "include"
    REQUIRES esp_wifi esp_event esp_netif esp_hosted nvs_flash
)