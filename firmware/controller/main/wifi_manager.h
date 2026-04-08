#ifndef WIFI_MANAGER_H
#define WIFI_MANAGER_H

typedef struct wifi_creds_struct_message {
  char ssid[32];
  char password[32];
} wifi_creds_struct_message;

// typedef struct peer_ack_struct_message {
//   bool connected;
// } peer_ack_struct_message;

bool loadSavedWifiCreds();
void saveWifiCreds(const char* ssid, const char* pass);
void clearSavedWifiCreds();
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status);
void initESPNOW();
void sendWifiCredsToPeers(const char* ssid, const char* pw);
char* generateRandomPassword(int length);
void configModeCallback(WiFiManager* myWiFiManager);
void connectToWiFi(bool reset);
void ensureWiFiAndESPNOWConnected();

#endif