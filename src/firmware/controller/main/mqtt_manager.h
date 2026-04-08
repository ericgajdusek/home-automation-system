#ifndef MQTT_MANAGER_H
#define MQTT_MANAGER_H

void initMQTT();
void reconnectMQTT();
void ensureMQTTConnected();
void mqttLoop();
void mqttCallback(char* topic, byte* payload, unsigned int length);
void publishLightsCommand();
void publishBlindsCommand();
void setNightMode(bool state);

#endif