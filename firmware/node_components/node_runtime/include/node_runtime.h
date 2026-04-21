#ifndef NODE_RUNTIME_H
#define NODE_RUNTIME_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "ap_message.h"
#include "provisioning_message.h"
#include "node_runtime_types.h"
#include "node_runtime_events.h"
#include "node_runtime_pending_table.h"
#include "node_rx.h"
#include "node_tx.h"
#include "node_transport_mqtt.h"
#include "node_transport_espnow.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "esp_log.h"

#define TOPIC_MAX_LEN            96
#define MAX_RETRIES_DEFAULT      3

QueueHandle_t node_runtime_event_queue;
QueueHandle_t node_runtime_rx_queue;
QueueHandle_t node_runtime_tx_high_queue;
QueueHandle_t node_runtime_tx_normal_queue;
QueueHandle_t node_runtime_tx_low_queue;

int node_runtime_init(const char *device_id);
int node_runtime_process();

#endif // NODE_RUNTIME_H
