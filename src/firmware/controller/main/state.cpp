#include "state.h"

char* apPassword = nullptr;
WiFiManager wm;
bool apModeActive = false;

TFT_eSPI tft = TFT_eSPI(135, 240);

const char* mqtt_server = "10.16.156.122";
const int mqtt_port = 1883;

const char* lightsCommandTopic = "home/lights/command";
const char* blindsCommandTopic = "home/blinds/command";
const char* lightsStateTopic = "home/lights/state";
const char* blindsStateTopic = "home/blinds/state";

const int LIGHTS_BUTTON_PIN = 17;
const int BLINDS_BUTTON_PIN = 21;
const int WIFI_RESET_BUTTON_PIN = 22;


bool lastLightsReading = HIGH;
bool lastBlindsReading = HIGH;
bool lastWifiResetReading = HIGH;
bool lightsStableState = HIGH;
bool blindsStableState = HIGH;
bool wifiResetStableState = HIGH;

unsigned long lastLightsDebounceTime = 0;
unsigned long lastBlindsDebounceTime = 0;
unsigned long lastWifiResetDebounceTime = 0;
const unsigned long debounceDelay = 50;

WiFiClient espClient;
PubSubClient client(espClient);

bool lightsState = false;
bool blindsState = false;
bool nightModeState = false;