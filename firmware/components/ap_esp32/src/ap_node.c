int ap_node_init() {
    ap_node_runtime_init();
    ap_node_transport_init();
    return 0;
}

int ap_node_process() {
    event_t event = ap_node_runtime_get_next_task();
    switch (event.type) {
        case EVENT_TYPE_MESSAGE_RECEIVED:
            ap_node_handle_message(&event.message);
            break;
        case EVENT_TYPE_TASK_TIMEOUT:
            ap_node_handle_timeout(&event.timeout);
            break;
        default:
            // Unknown event type
            break;
    }
    return 0;
}

int ap_node_ensure_connected() {
    return ap_node_transport_ensure_connected();
}

int ap_node_reconnect() {
    return ap_node_transport_reconnect();
}

static int ap_node_handle_message(const message_t *msg) {
    // Validate message
    if (validate_message(msg) != 0) {
        // Invalid message, ignore or log error
        return -1;
    }

    // Process message based on type and action
    switch (msg->message_type) {
        case MSG_TYPE_COMMAND:
            return ap_node_runtime_handle_command(msg);
        case MSG_TYPE_STATUS:
            return ap_node_runtime_handle_status(msg);
        case MSG_TYPE_ACK:
            return ap_node_runtime_handle_ack(msg);
        default:
            // Unknown message type
            return -1;
    }
}

static int ap_node_transport_init() {
    ap_node_transport_mqtt_init();
    ap_node_transport_espnow_init();
}