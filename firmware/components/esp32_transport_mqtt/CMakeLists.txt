idf_component_register(
    SRCS 
        src/esp32_transport_mqtt.c
    INCLUDE_DIRS 
        "include"
    REQUIRES 
        mqtt
)