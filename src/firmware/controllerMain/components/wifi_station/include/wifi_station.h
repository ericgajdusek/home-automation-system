#pragma once

#include "esp_err.h"

esp_err_t wifi_station_init(void);
esp_err_t wifi_station_wait_for_ip(void);