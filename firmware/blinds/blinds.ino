#include <WiFi.h>
#include <PubSubClient.h>
#include <esp_now.h>
#include <esp_wifi.h>

// ===== WiFi =====
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

const char* blindsSetTopic = "home/blinds/command";
const char* blindsStatusTopic = "home/blinds/status";

// ===== Pins =====
static const int PIN_EN   = 25;  // enable
static const int PIN_STEP = 22;  // step
static const int PIN_DIR  = 21;  // dir

static const bool EN_ACTIVE_LOW = true;

// ===== Preset movement =====
static const int32_t  STEPS_PER_TAP = 1200;
static const uint16_t US_PER_STEP   = 800;

// Change these if your motor direction is reversed
static const int8_t OPEN_DIR  = -1;
static const int8_t CLOSE_DIR = 1;

volatile bool have_cmd = false;
String latestCmd = "";

// ===== MQTT/WiFi =====
WiFiClient espClient;
PubSubClient client(espClient);

inline void enableDriver(bool on) {
  digitalWrite(PIN_EN, EN_ACTIVE_LOW ? (on ? LOW : HIGH) : (on ? HIGH : LOW));
}

void moveStepsBlocking(int32_t steps, uint16_t us_per_step, int8_t dir_hint) {
  if (steps == 0) return;

  bool dir_cw = (dir_hint > 0) ? true : (dir_hint < 0) ? false : (steps >= 0);
  digitalWrite(PIN_DIR, dir_cw ? HIGH : LOW);

  uint32_t n = (steps >= 0) ? steps : -steps;

  if (us_per_step < 200)   us_per_step = 200;
  if (us_per_step > 50000) us_per_step = 50000;

  uint16_t half = us_per_step / 2;

  for (uint32_t i = 0; i < n; ++i) {
    digitalWrite(PIN_STEP, HIGH);
    delayMicroseconds(half);
    digitalWrite(PIN_STEP, LOW);
    delayMicroseconds(us_per_step - half);
  }
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

void mqttCallback(char* topic, byte* payload, unsigned int length) {
  String msg;
  for (unsigned int i = 0; i < length; i++) {
    msg += (char)payload[i];
  }

  msg.trim();

  Serial.print("MQTT message on ");
  Serial.print(topic);
  Serial.print(": ");
  Serial.println(msg);

  if (msg == "OPEN" || msg == "CLOSE") {
    noInterrupts();
    latestCmd = msg;
    have_cmd = true;
    interrupts();
  } else {
    Serial.println("Invalid command");
    client.publish(blindsStatusTopic, "ERROR:BAD_COMMAND");
  }
}

void reconnectMQTT() {
  while (!client.connected()) {
    Serial.print("Connecting MQTT...");

    String clientId = "blinds-node-";
    clientId += String((uint32_t)ESP.getEfuseMac(), HEX);

    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      client.subscribe(blindsSetTopic);
      client.publish(blindsStatusTopic, "READY");
    } else {
      Serial.print("failed rc=");
      Serial.println(client.state());
      delay(2000);
    }
  }
}

void setup() {
  pinMode(PIN_EN, OUTPUT);
  pinMode(PIN_STEP, OUTPUT);
  pinMode(PIN_DIR, OUTPUT);

  digitalWrite(PIN_STEP, LOW);
  digitalWrite(PIN_DIR, LOW);
  enableDriver(false);

  Serial.begin(115200);
  delay(300);
  Serial.println("\nMQTT blinds node booting...");

  initESPNOW();

  // Wait here until credentials arrive
  while (!haveWifiCreds) {
    delay(1000);
    Serial.print(".");
  }

  // Stop ESP-NOW before normal WiFi connection
  esp_now_deinit();

  wifiConnected = connectWiFi(wifiCreds.ssid, wifiCreds.password);

  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(mqttCallback);
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

  if (have_cmd) {
    noInterrupts();
    String cmd = latestCmd;
    have_cmd = false;
    interrupts();

    enableDriver(true);

    if (cmd == "OPEN") {
      Serial.println("Executing OPEN");
      client.publish(blindsStatusTopic, "OPENING");
      moveStepsBlocking(STEPS_PER_TAP, US_PER_STEP, OPEN_DIR);
      client.publish(blindsStatusTopic, "OPEN");
    } 
    else if (cmd == "CLOSE") {
      Serial.println("Executing CLOSE");
      client.publish(blindsStatusTopic, "CLOSING");
      moveStepsBlocking(STEPS_PER_TAP, US_PER_STEP, CLOSE_DIR);
      client.publish(blindsStatusTopic, "CLOSED");
    }

    enableDriver(false);
    Serial.println("Move complete.");
  }

  delay(1);
}