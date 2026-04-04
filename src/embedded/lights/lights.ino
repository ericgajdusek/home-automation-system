#include <WiFi.h>
#include <PubSubClient.h>
#include <esp_now.h>
#include <esp_wifi.h>

// ===== WIFI =====
typedef struct wifi_creds_struct_message {
  char ssid[32];
  char password[32];
} wifi_creds_struct_message;

wifi_creds_struct_message wifiCreds;
bool haveWifiCreds = false;
bool wifiConnected = false;

// ===== MQTT =====
const char* mqtt_server = "10.16.156.122";

// ===== PINS =====
#define GATE_PIN 22

const int pwmChannel = 0;
const int pwmFreq = 5000;
const int pwmResolution = 8;

// ===== MQTT TOPICS =====
const char* lightsSetTopic = "home/lights/set";
const char* lightsStatusTopic = "home/lights/status";

WiFiClient espClient;
PubSubClient client(espClient);

void applyBrightness(int level) {
  level = constrain(level, 0, 255);

  // If your hardware is inverted, keep 255 - level.
  // If behavior is backwards, change to ledcWrite(pwmChannel, level);
  ledcWrite(pwmChannel, 255 - level);
}

void OnDataRecv(const uint8_t *mac, const uint8_t *incomingData, int len) {
  if (len != sizeof(wifiCreds)) {
    Serial.println("Received invalid WiFi credential packet size");
    return;
  }

  memcpy(&wifiCreds, incomingData, sizeof(wifiCreds));

  // Force null termination just in case
  wifiCreds.ssid[31] = '\0';
  wifiCreds.password[31] = '\0';

  Serial.println("Received WiFi credentials over ESP-NOW");
  Serial.print("SSID: ");
  Serial.println(wifiCreds.ssid);
  Serial.print("Password: ");
  Serial.println(wifiCreds.password);

  haveWifiCreds = true;
}

void initESPNOW() {
  WiFi.mode(WIFI_STA);
  // WiFi.disconnect(true, true);
  delay(100);

  // Set this to your router's actual 2.4 GHz channel
  esp_err_t chErr = esp_wifi_set_channel(9, WIFI_SECOND_CHAN_NONE);
  if (chErr != ESP_OK) {
    Serial.print("Failed to set channel, err=");
    Serial.println(chErr);
  }

  uint8_t ch;
  wifi_second_chan_t second;
  esp_wifi_get_channel(&ch, &second);
  Serial.print("ESP-NOW listen channel: ");
  Serial.println(ch);

  Serial.print("Receiver STA MAC: ");
  Serial.println(WiFi.macAddress());

  if (esp_now_init() != ESP_OK) {
    Serial.println("ESP-NOW init failed");
    return;
  }

  esp_now_register_recv_cb(esp_now_recv_cb_t(OnDataRecv));
  Serial.println("ESP-NOW ready, waiting for WiFi credentials...");
}

bool connectWiFi(const char* ssid, const char* password) {
  Serial.print("Connecting to WiFi: ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  unsigned long start = millis();
  const unsigned long timeoutMs = 15000;

  while (WiFi.status() != WL_CONNECTED && millis() - start < timeoutMs) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("WiFi connected");
    Serial.print("IP: ");
    Serial.println(WiFi.localIP());
    return true;
  }

  Serial.println("WiFi connection failed");
  return false;
}

void callback(char* topic, byte* payload, unsigned int length) {
  String msg;
  for (unsigned int i = 0; i < length; i++) {
    msg += (char)payload[i];
  }

  Serial.print("Received on ");
  Serial.print(topic);
  Serial.print(": ");
  Serial.println(msg);

  if (msg == "ON") {
    applyBrightness(255);
    client.publish(lightsStatusTopic, "ON");
  } 
  else if (msg == "OFF") {
    applyBrightness(0);
    client.publish(lightsStatusTopic, "OFF");
  }
}

void reconnectMQTT() {
  while (!client.connected()) {
    Serial.print("Connecting MQTT...");

    String clientId = "lights-node-";
    clientId += String((uint32_t)ESP.getEfuseMac(), HEX);

    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      client.subscribe(lightsSetTopic);
    } else {
      Serial.print("failed rc=");
      Serial.println(client.state());
      delay(2000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  delay(1000);

  ledcSetup(pwmChannel, pwmFreq, pwmResolution);
  ledcAttachPin(GATE_PIN, pwmChannel);
  applyBrightness(0);  // start OFF

  initESPNOW();

  // Wait here until credentials arrive
  while (!haveWifiCreds) {
    delay(1000);
    Serial.print(".");
  }

  // Stop ESP-NOW before normal WiFi connection
  esp_now_deinit();

  wifiConnected = connectWiFi(wifiCreds.ssid, wifiCreds.password);

  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void loop() {
  if (!wifiConnected || WiFi.status() != WL_CONNECTED) {
    Serial.println("WiFi disconnected");
    wifiConnected = connectWiFi(wifiCreds.ssid, wifiCreds.password);
    delay(1000);
    return;
  }

  if (!client.connected()) {
    reconnectMQTT();
  }

  client.loop();
}