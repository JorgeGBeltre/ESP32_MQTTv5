#include "ESP32_MQTTv5.h"
#include <WiFi.h>

const char *ssid = "YOUR_SSID";
const char *password = "YOUR_PASSWORD";
const char *mqttHost = "broker.example.com";
uint16_t mqttPort = 1883;

WiFiClient wifiClient;
ESP32_MQTTv5 mqtt;

String clientId;

void setup() {
  Serial.begin(115200);
  delay(100);

  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print('.');
  }
  Serial.printf("\nWiFi connected — IP: %s\n",
                WiFi.localIP().toString().c_str());

  mqtt.begin(wifiClient, mqttHost, mqttPort);

  mqtt.setBufferSize(1024);

  mqtt.setKeepAlive(30);
  mqtt.setCleanStart(true);
  mqtt.setSessionExpiry(300);
  mqtt.setTopicAliasMaximum(10);

  MQTTProperties willProps;
  willProps.setWillDelay(5);
  willProps.setContentType("text/plain");
  mqtt.setWill("esp32/status", "offline", 1, true, &willProps);

  mqtt.setAutoReconnect(true, 2000, 60000);

  mqtt.onConnect([]() {
    Serial.println("[MQTT] Connected!");

    MQTTProperties onlineProps;
    onlineProps.setContentType("text/plain");
    mqtt.publish("esp32/status", "online", 1, true, &onlineProps);

    mqtt.subscribe("esp32/cmd/#", 2, true, false, 0);
    mqtt.subscribe("esp32/test/properties");
  });

  mqtt.onDisconnect([](MQTTReasonCode reason) {
    Serial.printf("[MQTT] Disconnected. Reason: 0x%02X\n", reason);
  });

  mqtt.onMessage([](const MQTTMessage &msg) {
    Serial.printf("[MQTT] Message on [%s] QoS%d: %s\n", msg.topic.c_str(),
                  msg.qos, msg.payload.c_str());
    if (msg.hasTopicAlias)
      Serial.printf("       (via alias %u)\n", msg.topicAlias);
    if (msg.contentType.length())
      Serial.printf("       Content-Type: %s\n", msg.contentType.c_str());
    if (msg.messageExpiry)
      Serial.printf("       Expiry: %u s\n", msg.messageExpiry);
  });

  mqtt.onError([](MQTTReasonCode code, const String &errMsg) {
    Serial.printf("[MQTT] Error [0x%02X]: %s\n", (int)code, errMsg.c_str());
  });

  mqtt.onAuth([](MQTTReasonCode reason, const MQTTProperties &props) {
    Serial.printf("[MQTT] AUTH challenge — reason: 0x%02X\n", reason);
    MQTTProperties resp;
    resp.setAuthenticationMethod("CUSTOM_METHOD");
    uint8_t authData[] = {0xDE, 0xAD, 0xBE, 0xEF};
    resp.setAuthenticationData(authData, sizeof(authData));
    mqtt.auth(CONTINUE_AUTHENTICATION, resp);
  });

  clientId = "esp32-" + String((uint32_t)ESP.getEfuseMac(), HEX);
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

  static unsigned long lastTelemetry = 0;
  if (mqtt.connected() && millis() - lastTelemetry > 15000) {
    lastTelemetry = millis();

    {
      MQTTProperties props;
      props.addUserProperty("sender", "esp32-s3");
      props.addUserProperty("uptime", String(millis() / 1000));
      props.setContentType("application/json");
      props.setMessageExpiry(120);

      String payload =
          "{\"status\":\"ok\",\"heap\":" + String(ESP.getFreeHeap()) + "}";
      mqtt.publish("esp32/telemetry", payload, 1, false, &props);
      Serial.println("[MQTT] QoS-1 telemetry published");
    }

    {
      MQTTProperties props;
      props.setPayloadFormat(1);
      props.setContentType("text/plain");
      mqtt.publish("esp32/alert", "QoS2 heartbeat", 2, false, &props);
      Serial.println("[MQTT] QoS-2 alert published");
    }
  }
}