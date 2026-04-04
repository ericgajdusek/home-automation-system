#include "state.h"
#include "mqtt_manager.h"
#include "wifi_manager.h"
#include "buttons.h"

void initButtons() {
  pinMode(LIGHTS_BUTTON_PIN, INPUT_PULLUP);
  pinMode(BLINDS_BUTTON_PIN, INPUT_PULLUP);
  pinMode(WIFI_RESET_BUTTON_PIN, INPUT_PULLUP);
}

void updateButtons() {
  bool lightsReading = digitalRead(LIGHTS_BUTTON_PIN);
  bool blindsReading = digitalRead(BLINDS_BUTTON_PIN);
  bool wifiResetReading = digitalRead(WIFI_RESET_BUTTON_PIN);

  if (lightsReading != lastLightsReading) {
    lastLightsDebounceTime = millis();
  }

  if ((millis() - lastLightsDebounceTime) > debounceDelay) {
    if (lightsReading != lightsStableState) {
      lightsStableState = lightsReading;
      if (lightsStableState == LOW) {
        Serial.println("Lights button pressed");
        publishLightsCommand();
      }
    }
  }

  if (blindsReading != lastBlindsReading) {
    lastBlindsDebounceTime = millis();
  }

  if ((millis() - lastBlindsDebounceTime) > debounceDelay) {
    if (blindsReading != blindsStableState) {
      blindsStableState = blindsReading;
      if (blindsStableState == LOW) {
        Serial.println("Blinds button pressed");
        publishBlindsCommand();
      }
    }
  }

  if (wifiResetReading != lastWifiResetReading) {
    lastWifiResetDebounceTime = millis();
  }

  if ((millis() - lastWifiResetDebounceTime) > debounceDelay) {
    if (wifiResetReading != wifiResetStableState) {
      wifiResetStableState = wifiResetReading;
      if (wifiResetStableState == LOW) {
        Serial.println("WiFi Reset button pressed");
        connectToWiFi(true);
      }
    }
  }

  lastLightsReading = lightsReading;
  lastBlindsReading = blindsReading;
  lastWifiResetReading = wifiResetReading;
}