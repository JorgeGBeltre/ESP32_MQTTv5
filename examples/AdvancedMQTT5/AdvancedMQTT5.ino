#include <ESP32_MQTTv5.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>

const char *WIFI_SSID = "YOUR_SSID";
const char *WIFI_PASSWORD = "YOUR_PASSWORD";

const char *MQTT_HOST = "broker.example.com";
uint16_t MQTT_PORT = 8883;

const char *CA_CERT = nullptr;
// const char *CA_CERT = R"EOF(
// -----BEGIN CERTIFICATE-----
// < paste your CA PEM here >
// -----END CERTIFICATE-----
// )EOF";

static constexpr uint32_t SUB_ID_TELEMETRY = 1;
static constexpr uint32_t SUB_ID_CMD = 2;
static constexpr uint32_t SUB_ID_SHARED = 3;
WiFiClientSecure tlsClient;
ESP32_MQTTv5 mqtt;

String clientId;

uint32_t qos2SentCount = 0;
uint32_t qos2AckCount = 0;
bool expectedBuffer = false;

void onConnected() {
  Serial.println("[MQTT] Connected!");

  mqtt.subscribeShared("workers", "jobs/#", 1);
  Serial.println("[MQTT] Shared subscription: $share/workers/jobs/#  QoS1");

  mqtt.subscribe("esp32/telemetry/#", 1,
                 /*noLocal*/ true,
                 /*retainAsPublished*/ false,
                 /*retainHandling*/ 1,
                 /*subId*/ SUB_ID_TELEMETRY);
  Serial.println("[MQTT] Subscribed to esp32/telemetry/#  (noLocal, subId=1)");

  mqtt.subscribe("esp32/cmd/#", 2,
                 /*noLocal*/ false,
                 /*retainAsPublished*/ true,
                 /*retainHandling*/ 0,
                 /*subId*/ SUB_ID_CMD);
  Serial.println(
      "[MQTT] Subscribed to esp32/cmd/#  (retainAsPublished, subId=2)");

  MQTTProperties onlineProps;
  onlineProps.setContentType("text/plain");
  mqtt.publish("esp32/status", "online", 1, true, &onlineProps);
}

void onDisconnected(MQTTReasonCode reason) {
  Serial.printf(
      "[MQTT] Disconnected — reason: 0x%02X (auto-reconnect active)\n",
      (int)reason);
}

void onError(MQTTReasonCode code, const String &msg) {
  Serial.printf("[MQTT] Error [0x%02X]: %s\n", (int)code, msg.c_str());
}

void onAuth(MQTTReasonCode reason, const MQTTProperties &props) {

  Serial.printf("[MQTT] AUTH challenge (reason 0x%02X) — responding\n",
                (int)reason);
  MQTTProperties resp;
  resp.setAuthenticationMethod("MY_AUTH");
  uint8_t tok[] = {0x01, 0x02, 0x03};
  resp.setAuthenticationData(tok, sizeof(tok));
  mqtt.auth(CONTINUE_AUTHENTICATION, resp);
}

void onRawMessage(const String &topic, const uint8_t *payload, size_t len,
                  uint8_t qos, bool retain) {
  Serial.printf("[MQTT-RAW] topic=%s  qos=%d  retain=%d  payloadBytes=%u\n",
                topic.c_str(), qos, retain, (unsigned)len);

  if (len > 0) {
    Serial.printf("           first byte: 0x%02X\n", payload[0]);
  }
}

void onMessage(const MQTTMessage &msg) {
  Serial.printf("[MQTT] [subId=%u] %s  QoS%d: %s\n", msg.subscriptionIdentifier,
                msg.topic.c_str(), msg.qos, msg.payload.c_str());

  if (msg.hasSubscriptionIdentifier) {
    switch (msg.subscriptionIdentifier) {
    case SUB_ID_TELEMETRY:
      Serial.println("       → Telemetry message");
      break;
    case SUB_ID_CMD:
      Serial.printf("       → Command: %s\n", msg.payload.c_str());
      break;
    case SUB_ID_SHARED:
      Serial.println("       → Shared-subscription message (worker)");
      break;
    }
  }

  if (msg.contentType.length())
    Serial.printf("       Content-Type: %s\n", msg.contentType.c_str());
  if (msg.messageExpiry)
    Serial.printf("       Expiry: %us\n", msg.messageExpiry);
  if (msg.hasTopicAlias)
    Serial.printf("       (via alias %u)\n", msg.topicAlias);
}

void setup() {
  Serial.begin(115200);
  delay(200);

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print('.');
  }
  Serial.printf("\nWi-Fi connected — IP: %s\n",
                WiFi.localIP().toString().c_str());

  if (CA_CERT) {
    tlsClient.setCACert(CA_CERT);
  } else {
    tlsClient.setInsecure();
    Serial.println("[TLS] Certificate verification DISABLED — dev mode only!");
  }

  mqtt.begin(tlsClient, MQTT_HOST, MQTT_PORT);

  mqtt.setBufferSize(1024);

  mqtt.setDynamicBufferResize(true);

  mqtt.setReadTimeout(3000);

  mqtt.setKeepAlive(30);
  mqtt.setCleanStart(true);
  mqtt.setSessionExpiry(0);
  mqtt.setTopicAliasMaximum(10);

  MQTTProperties willProps;
  willProps.setWillDelay(10);
  willProps.setContentType("text/plain");
  mqtt.setWill("esp32/status", "offline", 1, true, &willProps);

  mqtt.setAutoReconnect(true, 3000, 120000);

  mqtt.onConnect(onConnected);
  mqtt.onDisconnect(onDisconnected);
  mqtt.onError(onError);
  mqtt.onAuth(onAuth);
  mqtt.onMessage(onMessage);

  mqtt.onMessageRaw(onRawMessage);

  clientId = "esp32-adv-" + String((uint32_t)ESP.getEfuseMac(), HEX);
  Serial.printf("Client ID: %s\n", clientId.c_str());
  mqtt.connect(clientId);
}

void loop() {

  if (WiFi.status() != WL_CONNECTED) {
    WiFi.reconnect();
    delay(500);
    return;
  }

  mqtt.loop();

  static unsigned long lastTx = 0;
  if (mqtt.connected() && millis() - lastTx > 20000) {
    lastTx = millis();

    MQTTProperties props;
    props.setPayloadFormat(1);
    props.setContentType("application/json");
    props.setMessageExpiry(60);
    props.addUserProperty("uptime", String(millis() / 1000));

    String payload = "{\"heap\":" + String(ESP.getFreeHeap()) +
                     ",\"rssi\":" + String(WiFi.RSSI()) + "}";

    bool ok = mqtt.publish("esp32/telemetry/stats", payload, 2, false, &props);
    if (ok) {
      qos2SentCount++;
      Serial.printf("[MQTT] QoS-2 PUBLISH #%u sent (%u bytes)\n", qos2SentCount,
                    payload.length());
    }
  }

  static unsigned long lastHb = 0;
  if (mqtt.connected() && millis() - lastHb > 5000) {
    lastHb = millis();
    mqtt.publish("esp32/heartbeat", String(millis()));
  }
}
