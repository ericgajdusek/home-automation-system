#pragma once
#include "esp_err.h"

esp_err_t net_link_init(void);
esp_err_t net_link_publish_lights_toggle(void);
esp_err_t net_link_publish_blinds_move_steps(int steps);