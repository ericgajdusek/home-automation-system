#include <WiFi.h>
#include <PubSubClient.h>
#include <esp_now.h>
#include <esp_wifi.h>

// ===== WIFI CREDS STRUCT =====
typedef struct wifi_creds_struct_message {
  char ssid[32];
  char password[32];
} wifi_creds_struct_message;

wifi_creds_struct_message wifiCreds;
bool haveWifiCreds = false;
bool wifiConnected = false;

// ===== MQTT =====
const char* mqtt_server = "10.16.156.122";
const int mqtt_port = 1883;

// ===== MQTT TOPICS =====
const char* lightsCommandTopic     = "home/lights/command";
const char* nightModeCommandTopic  = "home/nightmode/command";
// const char* ambientLightTopic      = "home/ambient/light";
// const char* ambientDistanceTopic   = "home/ambient/distance";

// ===== PINS =====
#define LIGHT_SENSOR_PIN 35
#define ULTRASONIC_ECHO_PIN 25
#define ULTRASONIC_TRIG_PIN 26

// ===== THRESHOLDS =====
const int LIGHT_THRESHOLD = 500;
const float DISTANCE_THRESHOLD_CM = 25;

// ===== TIMING =====
unsigned long lastSensorPrint = 0;
const unsigned long sensorIntervalMs = 1000;

// ===== STATE =====
bool nightModeActive = false;

// ===== CLIENTS =====
WiFiClient espClient;
PubSubClient client(espClient);

// ======================================================
// ESP-NOW receive callback
// ======================================================
void OnDataRecv(const uint8_t *mac, const uint8_t *incomingData, int len) {
  if (len != sizeof(wifiCreds)) {
    Serial.println("Received invalid WiFi credential packet size");
    return;
  }

  memcpy(&wifiCreds, incomingData, sizeof(wifiCreds));
  wifiCreds.ssid[31] = '\0';
  wifiCreds.password[31] = '\0';

  Serial.println("Received WiFi credentials over ESP-NOW");
  Serial.print("SSID: ");
  Serial.println(wifiCreds.ssid);
  Serial.print("Password: ");
  Serial.println(wifiCreds.password);

  haveWifiCreds = true;
}

// ======================================================
// ESP-NOW init
// ======================================================
void initESPNOW() {
  WiFi.mode(WIFI_STA);
  delay(100);

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

  esp_now_register_recv_cb(OnDataRecv);
  Serial.println("ESP-NOW ready, waiting for WiFi credentials...");
}

// ======================================================
// WiFi connect
// ======================================================
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

// ======================================================
// MQTT callback
// ======================================================
void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Received on ");
  Serial.print(topic);
  Serial.print(": ");

  for (unsigned int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}

// ======================================================
// MQTT reconnect
// ======================================================
void reconnectMQTT() {
  while (!client.connected()) {
    Serial.print("Connecting MQTT...");

    String clientId = "ambient-node-";
    clientId += String((uint32_t)ESP.getEfuseMac(), HEX);

    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
    } else {
      Serial.print("failed rc=");
      Serial.println(client.state());
      delay(2000);
    }
  }
}

// ======================================================
// Read photoresistor
// ======================================================
int readLightValue() {
  return analogRead(LIGHT_SENSOR_PIN);
}

// ======================================================
// Read ultrasonic distance in cm
// ======================================================
float readDistanceCm() {
  digitalWrite(ULTRASONIC_TRIG_PIN, LOW);
  delayMicroseconds(2);

  digitalWrite(ULTRASONIC_TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(ULTRASONIC_TRIG_PIN, LOW);

  long duration = pulseIn(ULTRASONIC_ECHO_PIN, HIGH, 30000);

  if (duration == 0) {
    return -1.0;
  }

  float distanceCm = duration * 0.0343 / 2.0;
  return distanceCm;
}

// ======================================================
// Publish sensor readings
// ======================================================
// void publishSensorData(int lightValue, float distanceCm) {
//   char lightPayload[16];
//   char distancePayload[16];

//   snprintf(lightPayload, sizeof(lightPayload), "%d", lightValue);

//   if (distanceCm < 0) {
//     snprintf(distancePayload, sizeof(distancePayload), "NO_ECHO");
//   } else {
//     dtostrf(distanceCm, 0, 2, distancePayload);
//   }

//   client.publish(ambientLightTopic, lightPayload, true);
//   client.publish(ambientDistanceTopic, distancePayload, true);
// }

// ======================================================
// Night mode + light trigger logic
// ======================================================
void handleNightModeAndLightTrigger(int lightValue, float distanceCm) {
  bool shouldBeNightMode = (lightValue < LIGHT_THRESHOLD);

  // Only publish night mode when it changes
  if (shouldBeNightMode != nightModeActive) {
    nightModeActive = shouldBeNightMode;

    if (nightModeActive) {
      Serial.println("Night mode -> ON");
      client.publish(nightModeCommandTopic, "ON", true);
    } else {
      Serial.println("Night mode -> OFF");
      client.publish(nightModeCommandTopic, "OFF", true);
    }
  }

  // While in night mode, trigger lights on if object is close enough
  if (nightModeActive && distanceCm > 0 && distanceCm < DISTANCE_THRESHOLD_CM) {
    Serial.println("Distance threshold met -> sending lights ON");
    client.publish(lightsCommandTopic, "ON", false);
  }
}

// ======================================================
// Setup
// ======================================================
void setup() {
  Serial.begin(115200);
  delay(1000);

  pinMode(LIGHT_SENSOR_PIN, INPUT);
  pinMode(ULTRASONIC_TRIG_PIN, OUTPUT);
  pinMode(ULTRASONIC_ECHO_PIN, INPUT);

  initESPNOW();

  while (!haveWifiCreds) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();

  esp_now_deinit();

  wifiConnected = connectWiFi(wifiCreds.ssid, wifiCreds.password);

  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);

  Serial.println("Ambient sensor setup complete");
}

// ======================================================
// Loop
// ======================================================
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

  if (millis() - lastSensorPrint >= sensorIntervalMs) {
    lastSensorPrint = millis();

    int lightValue = readLightValue();
    float distanceCm = readDistanceCm();

    Serial.print("Light: ");
    Serial.print(lightValue);
    Serial.print(" | Distance (cm): ");

    if (distanceCm < 0) {
      Serial.println("NO_ECHO");
    } else {
      Serial.println(distanceCm, 2);
    }

    publishSensorData(lightValue, distanceCm);
    handleNightModeAndLightTrigger(lightValue, distanceCm);
  }
}