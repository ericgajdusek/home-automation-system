#ifndef ESP32_RUNTIME_H
#define ESP32_RUNTIME_H

#include "ap_message.h"

int esp32_runtime_init(const char *device_id);
int esp32_runtime_process();

#endif // ESP32_RUNTIME_H