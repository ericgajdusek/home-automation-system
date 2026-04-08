#include "state.h"
#include "display_manager.h"
#include "wifi_manager.h"

static uint8_t blindsControllerMac[] = {0x14, 0x33, 0x5C, 0x02, 0xAD, 0x70};
static uint8_t lightsControllerMac[] = {0x6C, 0xC8, 0x40, 0x89, 0x73, 0xE8};
static uint8_t ambientSensorMac[]    = {0x14, 0x33, 0x5C, 0x0A, 0x37, 0x30};

static esp_now_peer_info_t blindsControllerInfo = {};
static esp_now_peer_info_t lightsControllerInfo = {};
static esp_now_peer_info_t ambientSensorInfo = {};

wifi_creds_struct_message wifiCreds;

Preferences prefs;

bool loadSavedWifiCreds() {
  prefs.begin("wifi", true);  // read-only
  String savedSsid = prefs.getString("ssid", "");
  String savedPass = prefs.getString("pass", "");
  prefs.end();

  if (savedSsid.length() == 0 || savedPass.length() == 0) {
    return false;
  }

  savedSsid.toCharArray(wifiCreds.ssid, sizeof(wifiCreds.ssid));
  savedPass.toCharArray(wifiCreds.password, sizeof(wifiCreds.password));

  Serial.println("Loaded WiFi credentials from Preferences");
  Serial.print("SSID: ");
  Serial.println(wifiCreds.ssid);

  return true;
}

void saveWifiCreds(const char* ssid, const char* pass) {
  prefs.begin("wifi", false);  // read/write
  prefs.putString("ssid", ssid);
  prefs.putString("pass", pass);
  prefs.end();

  Serial.println("Saved WiFi credentials to Preferences");
}

void clearSavedWifiCreds() {
  prefs.begin("wifi", false);
  prefs.clear();
  prefs.end();

  Serial.println("Cleared saved WiFi credentials");
}

void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}

void initESPNOW() {
  WiFi.mode(WIFI_STA);

  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  memcpy(blindsControllerInfo.peer_addr, blindsControllerMac, 6);
  blindsControllerInfo.channel = 0;  
  blindsControllerInfo.encrypt = false;  
  if (esp_now_add_peer(&blindsControllerInfo) != ESP_OK){
    Serial.println("Failed to add blinds peer");
    return;
  }

  memcpy(lightsControllerInfo.peer_addr, lightsControllerMac, 6);
  lightsControllerInfo.channel = 0;  
  lightsControllerInfo.encrypt = false;    
  if (esp_now_add_peer(&lightsControllerInfo) != ESP_OK){
    Serial.println("Failed to add lights peer");
    return;
  }

  memcpy(ambientSensorInfo.peer_addr, ambientSensorMac, 6);
  ambientSensorInfo.channel = 0;  
  ambientSensorInfo.encrypt = false;
  if (esp_now_add_peer(&ambientSensorInfo) != ESP_OK){
    Serial.println("Failed to add ambient sensor peer");
    return;
  }
}

void sendWifiCredsToPeers(const char* ssid, const char* pw) {
  strncpy(wifiCreds.ssid, ssid, sizeof(wifiCreds.ssid) - 1);
  wifiCreds.ssid[sizeof(wifiCreds.ssid) - 1] = '\0';

  strncpy(wifiCreds.password, pw, sizeof(wifiCreds.password) - 1);
  wifiCreds.password[sizeof(wifiCreds.password) - 1] = '\0';

  initESPNOW();

  esp_err_t blindsResult = esp_now_send(blindsControllerMac, (uint8_t*)&wifiCreds, sizeof(wifiCreds));
  Serial.println(blindsResult == ESP_OK ? "Successfully sent to blinds module" : "Error sending data to blinds module");

  esp_err_t lightsResult = esp_now_send(lightsControllerMac, (uint8_t*)&wifiCreds, sizeof(wifiCreds));
  Serial.println(lightsResult == ESP_OK ? "Successfully sent to lights module" : "Error sending data to lights module");

  esp_err_t ambientSensorResult = esp_now_send(ambientSensorMac, (uint8_t*)&wifiCreds, sizeof(wifiCreds));
  Serial.println(ambientSensorResult == ESP_OK ? "Successfully sent to ambient sensor module" : "Error sending data to ambient sensor module");
}

char* generateRandomPassword(int length) {
  const char charset[] =
      "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
      "abcdefghijklmnopqrstuvwxyz"
      "0123456789";

  char* password = (char*)malloc(length + 1);
  if (!password) return NULL;

  for (int i = 0; i < length; i++) {
    uint32_t randNum = esp_random();
    int index = randNum % (sizeof(charset) - 1);
    password[i] = charset[index];
  }

  password[length] = '\0';
  return password;
}

void configModeCallback(WiFiManager* myWiFiManager) {
  Serial.println("Entered AP setup mode");
  apModeActive = true;
  updateDisplay();
}

void connectToWiFi(bool reset) {
  Serial.println("Connecting to WiFi...");
  WiFi.mode(WIFI_STA);

  if (reset) {
    wm.resetSettings();
    clearSavedWifiCreds();
  }

  // First try your own saved credentials
  if (!reset && loadSavedWifiCreds()) {
    Serial.println("Trying saved credentials from Preferences...");
    WiFi.begin(wifiCreds.ssid, wifiCreds.password);

    unsigned long start = millis();
    while (WiFi.status() != WL_CONNECTED && millis() - start < 10000) {
      delay(500);
      Serial.print(".");
    }
    Serial.println();

    if (WiFi.status() == WL_CONNECTED) {
      Serial.println("Connected using saved credentials");
      Serial.print("IP address: ");
      Serial.println(WiFi.localIP());

      apModeActive = false;
      updateDisplay();
      sendWifiCredsToPeers(wifiCreds.ssid, wifiCreds.password);
      return;
    }

    Serial.println("Saved credentials failed, falling back to WiFiManager");
  }

  wm.setAPCallback(configModeCallback);

  if (apPassword != nullptr) {
    free(apPassword);
    apPassword = nullptr;
  }

  apPassword = generateRandomPassword(12);

  Serial.print("AP Password: ");
  Serial.println(apPassword);

  apModeActive = true;
  updateDisplay();

  if (!wm.autoConnect("RoomLightingSystemAP", apPassword)) {
    Serial.println("Failed to connect");
    apModeActive = true;
    updateDisplay();
    return;
  }

  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  String ssid = WiFi.SSID();
  String pass = WiFi.psk();

  ssid.toCharArray(wifiCreds.ssid, sizeof(wifiCreds.ssid));
  pass.toCharArray(wifiCreds.password, sizeof(wifiCreds.password));

  Serial.print("Connected SSID: ");
  Serial.println(wifiCreds.ssid);

  saveWifiCreds(wifiCreds.ssid, wifiCreds.password);

  sendWifiCredsToPeers(wifiCreds.ssid, wifiCreds.password);

  apModeActive = false;
  updateDisplay();
}

void ensureWiFiAndESPNOWConnected() {
  if (WiFi.status() != WL_CONNECTED) {
    connectToWiFi(false);
  }

  static bool espNowReady = false;
  if (!espNowReady) {
    if (esp_now_init() != ESP_OK) {
      Serial.println("Error initializing ESP-NOW");
      return;
    }
    esp_now_register_send_cb(OnDataSent);
    espNowReady = true;
  }
}