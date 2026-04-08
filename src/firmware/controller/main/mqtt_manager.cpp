#include "state.h"
#include "display_manager.h"
#include "mqtt_manager.h"

void initMQTT() {
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(mqttCallback);
}

void reconnectMQTT() {
  while (!client.connected()) {
    Serial.print("Connecting to MQTT...");

    String clientId = "ttgo-control-";
    clientId += String((uint32_t)ESP.getEfuseMac(), HEX);

    if (client.connect(clientId.c_str())) {
      Serial.println("connected");

      client.subscribe(lightsStateTopic);
      client.subscribe(blindsStateTopic);
      Serial.println("Subscribed to canonical state topics");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" retrying in 2 seconds...");
      delay(2000);
    }
  }
}

void ensureMQTTConnected() {
  if (!client.connected()) {
    reconnectMQTT();
  }
}

void mqttLoop() {
  client.loop();
}

void mqttCallback(char* topic, byte* payload, unsigned int length) {
  String msg;
  for (unsigned int i = 0; i < length; i++) {
    msg += (char)payload[i];
  }

  Serial.print("Received state update on ");
  Serial.print(topic);
  Serial.print(": ");
  Serial.println(msg);

  if (String(topic) == lightsStateTopic) {
    lightsState = (msg == "ON");
    updateDisplay();
  }
  else if (String(topic) == blindsStateTopic) {
    blindsState = (msg == "OPEN");
    updateDisplay();
  }
}

void publishLightsCommand() {
  const char* payload = lightsState ? "OFF" : "ON";

  Serial.print("Publishing to ");
  Serial.print(lightsCommandTopic);
  Serial.print(": ");
  Serial.println(payload);

  bool ok = client.publish(lightsCommandTopic, payload, true);
  Serial.print("Publish result: ");
  Serial.println(ok ? "success" : "fail");
}

void publishBlindsCommand() {
  const char* payload = blindsState ? "CLOSE" : "OPEN";

  Serial.print("Publishing to ");
  Serial.print(blindsCommandTopic);
  Serial.print(": ");
  Serial.println(payload);

  bool ok = client.publish(blindsCommandTopic, payload, true);
  Serial.print("Publish result: ");
  Serial.println(ok ? "success" : "fail");
}

void setNightMode(bool state) {
  nightModeState = state;
  updateDisplay();
}