#include node_runtime.h

int node_runtime_init(const char *device_id) {
    node_runtime_event_queue = xQueueCreate(
        10,                 // length (how many events can buffer)
        sizeof(event_item_t) // item size
    );

    node_runtime_rx_queue = xQueueCreate(
        10,                 // length
        sizeof(ingress_item_t) // item size
    );

    node_runtime_tx_high_queue = xQueueCreate(
        10,                 // length
        sizeof(egress_item_t) // item size
    );

    node_runtime_tx_normal_queue = xQueueCreate(
        10,                 // length
        sizeof(egress_item_t) // item size
    );

    node_runtime_tx_low_queue = xQueueCreate(
        10,                 // length
        sizeof(egress_item_t) // item size
    );
}

int node_runtime_process() {
    while (true) {
        // Process incoming events
        event_item_t event;
        if (xQueueReceive(node_runtime_event_queue, &event, 0) == pdTRUE) {
            // Handle event based on type
            switch (event.type) {
                case EVENT_UI_SLIDER_CHANGED:
                    // Handle UI slider change
                    break;
                case EVENT_UI_BUTTON_PRESSED:
                    // Handle button press
                    break;
                case EVENT_AP_RECEIVED:
                    // Handle received AP message
                    break;
                case EVENT_PROV_RECEIVED:
                    // Handle received provisioning message
                    break;
                case EVENT_TX_SEND_FAILED:
                    // Handle transmission failure
                    break;
                case EVENT_TX_TIMEOUT_EXPIRED:
                    // Handle transmission timeout
                    break;
                case EVENT_RETRY_REQUESTED:
                    // Handle retry request
                    break;
                case EVENT_TRANSPORT_CONNECTED:
                    // Handle transport connected
                    break;
                case EVENT_TRANSPORT_DISCONNECTED:
                    // Handle transport disconnected
                    break;
            }
        }

        // Process incoming messages
        rx_item_t rx_item;
        if (xQueueReceive(node_runtime_rx_queue, &rx_item, 0) == pdTRUE) {
            node_rx_process_next();
        }

        // Process outgoing messages by priority
        if (uxQueueMessagesWaiting(node_runtime_tx_high_queue) > 0) {
            node_tx_process_next();
        } else if (uxQueueMessagesWaiting(node_runtime_tx_normal_queue) > 0) {
            node_tx_process_next();
        } else if (uxQueueMessagesWaiting(node_runtime_tx_low_queue) > 0) {
            node_tx_process_next();
        }

        vTaskDelay(pdMS_TO_TICKS(10)); // Small delay to prevent tight loop
    }
}

