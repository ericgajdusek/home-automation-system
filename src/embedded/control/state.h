#ifndef STATE_H
#define STATE_H

#include <WiFi.h>
#include <PubSubClient.h>
#include <WiFiManager.h>
#include <TFT_eSPI.h>
#include <esp_now.h>
#include <stdint.h>
#include <Preferences.h>

// ===== AP setup vars =====
extern char* apPassword;
extern WiFiManager wm;
extern bool apModeActive;

// ===== Display =====
extern TFT_eSPI tft;
#define TFT_BL 4

// ===== MQTT broker =====
extern const char* mqtt_server;
extern const int mqtt_port;

// ===== MQTT Topics =====
extern const char* lightsCommandTopic;
extern const char* blindsCommandTopic;
extern const char* lightsStateTopic;
extern const char* blindsStateTopic;

// ===== Button pins =====
extern const int LIGHTS_BUTTON_PIN;
extern const int BLINDS_BUTTON_PIN;
extern const int WIFI_RESET_BUTTON_PIN;

// ===== Button state tracking =====
extern bool lastLightsReading;
extern bool lastBlindsReading;
extern bool lastWifiResetReading;
extern bool lightsStableState;
extern bool blindsStableState;
extern bool wifiResetStableState;
extern unsigned long lastLightsDebounceTime;
extern unsigned long lastBlindsDebounceTime;
extern unsigned long lastWifiResetDebounceTime;
extern const unsigned long debounceDelay;

// ===== MQTT/WiFi clients =====
extern WiFiClient espClient;
extern PubSubClient client;

// ===== System states =====
extern bool lightsState;
extern bool blindsState;
extern bool nightModeState;

#endif