#include "Arduino.h"
#include "state.h"
#include "display_manager.h"
#include "wifi_manager.h"
#include "mqtt_manager.h"
#include "buttons.h"

void setup() {
  Serial.begin(115200);

  initDisplay();
  updateDisplay();

  initButtons();

  connectToWiFi(false);
  initMQTT();

  Serial.println("Setup complete");
}

void loop() {
  ensureWiFiAndESPNOWConnected();
  ensureMQTTConnected();
  mqttLoop();
  updateButtons();
}