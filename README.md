# ESP32_MQTTv5

[![Platform](https://img.shields.io/badge/platform-ESP32-blue)](https://www.espressif.com/en/products/socs/esp32)
[![MQTT Version](https://img.shields.io/badge/MQTT-5.0-brightgreen)](https://docs.oasis-open.org/mqtt/mqtt/v5.0/os/mqtt-v5.0-os.html)
[![License](https://img.shields.io/badge/license-MIT-green)](LICENSE)

## Why ESP32_MQTTv5?

- Full MQTT 5.0 support (most Arduino libs are MQTT 3.1.1 only)
- Zero heap fragmentation (safe for long-running devices)
- Native support for advanced features like AUTH and shared subscriptions
- Designed for production IoT systems, not just demos

**MQTTv5.0 client library for ESP32 ArduinoIDE**  
Supports all core MQTTv5.0 features: properties, reason codes, topic aliases, shared subscriptions, subscription identifiers, will messages with properties, enhanced authentication (AUTH), QoS 0/1/2, auto-reconnect, and efficient memory usage.

---

##  Table of Contents

- [Features](#-features)
- [Installation](#-installation)
- [Backend Implementation](#Backend-Implementation)
- [Dependencies](#-dependencies)
- [Quick Start](#-quick-start)
- [API Reference](#-api-reference)
  - [ESP32_MQTTv5 class](#esp32_mqttv5-class)
  - [MQTTProperties class](#mqttproperties-class)
  - [MQTTMessage struct](#mqttmessage-struct)
- [Examples](#-examples)
  - [Basic MQTT 5.0 (TCP)](#1-basic-mqtt-50-tcp-basicmqtt5ino)
  - [Advanced MQTT 5.0 (TLS, Shared Subs, Raw Callback)](#2-advanced-mqtt-50-tls-shared-subs-raw-callback-advancedmqtt5ino)
- [Configuration Macros](#-configuration-macros)
- [Memory & Performance](#-memory--performance)
- [Broker Compatibility](#-broker-compatibility)
- [License](#-license)

---

##  Features

- **Full MQTT 5.0 protocol** – CONNECT/CONNACK, PUBLISH (QoS 0,1,2), SUBSCRIBE/UNSUBSCRIBE, PING, DISCONNECT, AUTH
- **MQTT 5.0 Properties** – User Properties, Message Expiry, Content Type, Response Topic, Correlation Data, Subscription Identifier, Topic Alias, Will Delay, Session Expiry, Maximum Packet Size, etc.
- **Reason Codes** – All MQTT 5.0 reason codes for fine‑grained error handling
- **Topic Aliases** – Bidirectional (outgoing & incoming) with configurable maximum
- **Shared Subscriptions** – Built‑in `subscribeShared()` helper
- **Subscription Identifiers** – Assign IDs per subscription and receive them in PUBLISH messages
- **Will Message with Properties** – Full will configuration including delay, content type, user properties
- **Enhanced Authentication (AUTH)** – Challenge‑response via `onAuth()` callback
- **QoS 2** – Complete retransmission with DUP flag and configurable retries
- **Auto‑Reconnect** – Exponential backoff, automatic resubscription
- **Raw Message Callback** – Zero‑copy access to payload (`uint8_t*`) to avoid RAM duplication
- **Dynamic Buffer Resizing** – Grows buffer automatically when receiving large packets
- **Sliding Read Timeout** – Robust timeout handling for unreliable networks
- **No Dynamic Containers** – Uses static arrays only (no `std::vector`, `std::map`) – **no heap fragmentation**

---

##  Installation

1. **Arduino Library Manager**  
   *Soon available* – search for `ESP32 MQTTv5`

2. **Manual install**  
   - Download this repository as ZIP  
   - In Arduino IDE: `Sketch → Include Library → Add .ZIP Library`

3. **Git clone**  
   ```bash
   git clone https://github.com/JorgeGBeltre/ESP32_MQTTv5.git
   ```
   Then copy the folder to your Arduino `libraries` directory.

---

##  Dependencies

- [Arduino core for ESP32](https://github.com/espressif/arduino-esp32) (tested with v2.0.x / v3.0.x)
- Built‑in `WiFiClient` and `WiFiClientSecure` (no extra libraries)

---

##  Quick Start

```cpp
#include <WiFi.h>
#include <ESP32_MQTTv5.h>

WiFiClient wifiClient;
ESP32_MQTTv5 mqtt;

void setup() {
  Serial.begin(115200);
  WiFi.begin("SSID", "PASSWORD");
  while (WiFi.status() != WL_CONNECTED) delay(500);

  mqtt.begin(wifiClient, "broker.hivemq.com", 1883);
  mqtt.onConnect([]() { mqtt.subscribe("test/topic"); });
  mqtt.onMessage([](const MQTTMessage& msg) {
    Serial.println(msg.payload);
  });
  mqtt.connect("esp32-client");
}

void loop() {
  mqtt.loop();
  mqtt.publish("test/topic", "Hello MQTT 5.0!");
  delay(5000);
}
```

---

##  API Reference

### `ESP32_MQTTv5` class

#### Initialization
| Method | Description |
|--------|-------------|
| `begin(host, port)` | Use default `WiFiClient` (TCP) |
| `begin(client, host, port)` | Use provided `WiFiClient` (e.g. `WiFiClientSecure`) |

#### Configuration
| Method | Description |
|--------|-------------|
| `setBufferSize(size)` | Set internal buffer size (min 32, default 512) |
| `setKeepAlive(seconds)` | Keep alive interval (default 60) |
| `setCleanStart(clean)` | MQTT clean start flag (default true) |
| `setSessionExpiry(seconds)` | Session expiry interval (0 = delete on disconnect) |
| `setTopicAliasMaximum(max)` | Max outgoing topic aliases to negotiate (default 0) |
| `setWill(topic, payload, qos, retain, props)` | Configure last will |
| `setAutoReconnect(enable, initDelay, maxDelay)` | Exponential backoff auto‑reconnect |
| `setDynamicBufferResize(enable)` | Allow automatic buffer growth (default true) |
| `setReadTimeout(ms)` | Timeout for reading packet length and payload (default 2000) |

#### Connection
| Method | Description |
|--------|-------------|
| `connect(clientId)` | Simple connect |
| `connect(clientId, username, password)` | With username/password |
| `connect(clientId, username, password, extraProps)` | With additional CONNECT properties (e.g. API key as User Property) |
| `disconnect(reason)` | Graceful disconnect with reason code |
| `connected()` | Returns connection status |
| `loop()` | **Must be called frequently** – processes incoming packets and keepalive |

#### Publish
| Method | Description |
|--------|-------------|
| `publish(topic, payload)` | QoS 0 |
| `publish(topic, payload, qos)` | QoS 0,1,2 |
| `publish(topic, payload, props)` | With properties |
| `publish(topic, payload, qos, retain, props)` | Full version |

#### Subscribe / Unsubscribe
| Method | Description |
|--------|-------------|
| `subscribe(topic)` | QoS 0 |
| `subscribe(topic, qos)` | QoS 0,1,2 |
| `subscribe(topic, qos, noLocal, retainAsPublished, retainHandling, subId)` | Advanced options |
| `subscribeShared(group, topic, qos)` | Shared subscription (`$share/group/topic`) |
| `unsubscribe(topic)` | Unsubscribe |

#### Callbacks
| Callback | Description |
|----------|-------------|
| `onConnect(cb)` | Called after successful CONNACK |
| `onDisconnect(cb)` | Called on disconnect (reason code) |
| `onMessage(cb)` | Called for each PUBLISH (payload copied to `String`) |
| `onMessageRaw(cb)` | **Zero‑copy** callback – payload as `const uint8_t*` (takes precedence over `onMessage`) |
| `onError(cb)` | Protocol errors, timeouts, buffer overflows |
| `onAuth(cb)` | Enhanced authentication challenge (AUTH packet) |

#### Authentication
| Method | Description |
|--------|-------------|
| `auth(reason, props)` | Send AUTH packet (used inside `onAuth` callback) |

#### Utility
| Method | Description |
|--------|-------------|
| `lastReasonCode()` | Last reason code received |
| `lastReasonString()` | Optional reason string from server |

---

### `MQTTProperties` class

Stores and serializes MQTT 5.0 properties. All methods are chainable.

| Method | Description |
|--------|-------------|
| `clear()` | Remove all properties |
| `addUserProperty(key, value)` | User Property (UTF‑8 pair) |
| `setMessageExpiry(seconds)` | Message expiry interval |
| `setContentType(type)` | e.g. `"application/json"` |
| `setResponseTopic(topic)` | Response topic for request‑response |
| `setCorrelationData(data)` | Binary correlation data |
| `setSessionExpiry(seconds)` | Session expiry |
| `setWillDelay(seconds)` | Will delay interval |
| `setRequestProblemInfo(bool)` | Request problem info |
| `setRequestResponseInfo(bool)` | Request response info |
| `setReasonString(reason)` | Human‑readable reason |
| `setPayloadFormat(format)` | 0 = bytes, 1 = UTF‑8 |
| `setSubscriptionIdentifier(id)` | Subscription identifier |
| `setAuthenticationMethod(method)` | e.g. `"CUSTOM"` |
| `setAuthenticationData(data, len)` | Binary auth data |
| `setTopicAlias(alias)` | Topic alias (outgoing) |
| `setTopicAliasMaximum(max)` | Maximum alias the client accepts |
| `setMaximumPacketSize(size)` | Client’s maximum packet size |

**Getters** (for received properties):
- `getUserProperty(key)`
- `getReasonString()`
- `getSubscriptionIdentifier()`
- `getTopicAlias()`
- `getPayloadFormat()`
- `getUint8(id, default)`, `getUint16()`, `getUint32()`, `getString(id)`

---

### `MQTTMessage` struct

| Field | Description |
|-------|-------------|
| `topic` | Topic name (String) |
| `payload` | Payload (String) – only valid if using `onMessage` |
| `qos` | QoS level (0,1,2) |
| `retain` | Retain flag |
| `messageExpiry` | Message expiry interval (if present) |
| `contentType` | Content type |
| `responseTopic` | Response topic |
| `correlationData` | Correlation data |
| `subscriptionIdentifier` | Subscription ID (if present) |
| `topicAlias` | Topic alias (if present) |
| `hasTopicAlias`, `hasSubscriptionIdentifier`, `hasPayloadFormatIndicator` | Presence flags |

---

##  Examples

### 1. Basic MQTT 5.0 (TCP) – `BasicMQTT5.ino`

Demonstrates:
- TCP connection to a public broker
- Will message with properties
- Auto‑reconnect with exponential backoff
- Subscribing with advanced options (`noLocal`, `retainHandling`)
- Publishing with QoS 1 and QoS 2, user properties, message expiry

[View full code](examples/BasicMQTT5/BasicMQTT5.ino)

**Key snippets**:

```cpp
// Will message with delay and content type
MQTTProperties willProps;
willProps.setWillDelay(5);
willProps.setContentType("text/plain");
mqtt.setWill("esp32/status", "offline", 1, true, &willProps);

// Subscribe with noLocal and retainHandling
mqtt.subscribe("esp32/cmd/#", 2, true, false, 0);

// Publish with user properties
MQTTProperties props;
props.addUserProperty("sender", "esp32-s3");
props.addUserProperty("uptime", String(millis()/1000));
props.setMessageExpiry(120);
mqtt.publish("esp32/telemetry", payload, 1, false, &props);
```

---

### 2. Advanced MQTT 5.0 (TLS, Shared Subs, Raw Callback) – `AdvancedMQTT5.ino`

Demonstrates:
- TLS connection with `WiFiClientSecure`
- Shared subscriptions (`subscribeShared`)
- Subscription identifiers
- Raw message callback (zero‑copy)
- QoS 2 retransmission demonstration
- Dynamic buffer resize and custom read timeout

[View full code](examples/AdvancedMQTT5/AdvancedMQTT5.ino)

**Key snippets**:

```cpp
// TLS with optional CA certificate
WiFiClientSecure tlsClient;
if (CA_CERT) tlsClient.setCACert(CA_CERT);
else tlsClient.setInsecure();
mqtt.begin(tlsClient, MQTT_HOST, MQTT_PORT);

// Shared subscription
mqtt.subscribeShared("workers", "jobs/#", 1);

// Raw callback (no String copy)
mqtt.onMessageRaw([](const String& topic, const uint8_t* payload,
                     size_t len, uint8_t qos, bool retain) {
  Serial.printf("Raw payload, first byte: 0x%02X\n", payload[0]);
});

// Subscription with ID
mqtt.subscribe("esp32/telemetry/#", 1, true, false, 1, SUB_ID_TELEMETRY);
```

---

##  Configuration Macros

You can override these defaults before including the library:

| Macro | Default | Description |
|-------|---------|-------------|
| `MQTT_MAX_PROPS` | 20 | Max properties per MQTTProperties object |
| `MQTT_MAX_ALIASES` | 16 | Max topic aliases (incoming+outgoing) |
| `MQTT_MAX_QOS2_OUT` | 8 | Max concurrent QoS 2 outgoing messages |
| `MQTT_MAX_QOS2_IN` | 8 | Max concurrent QoS 2 incoming messages |
| `MQTT_DEFAULT_BUFFER_SIZE` | 512 | Default buffer size (bytes) |
| `MQTT_QOS2_RETRY_MS` | 5000 | Retry interval for QoS 2 messages (ms) |
| `MQTT_QOS2_MAX_RETRIES` | 3 | Max retransmissions before abandoning |

Example:
```cpp
#define MQTT_MAX_ALIASES 32
#define MQTT_DEFAULT_BUFFER_SIZE 2048
#include <ESP32_MQTTv5.h>
```

---

##  Memory & Performance

- **Static arrays only** – No `std::vector`, no `std::map`, no heap fragmentation.
- **Single dynamic buffer** – Grows on demand (if enabled) and is reused.
- **Raw callback** – Avoids `String` copy for large payloads.
- **Typical RAM usage** (default settings): ~2.5 KB + buffer (512 B).  
  With QoS 2 retransmission buffers, additional memory is allocated temporarily.
- **CPU**: Non‑blocking `loop()`, uses `delay(0)` to yield.

---

## Troubleshooting

### Connection fails
- Check broker supports MQTT 5.0
- Verify TLS certificates

### Messages not received
- Ensure loop() is called frequently

---

## Backend Implementation

To use MQTTOTA in your project, you'll need an MQTT/MQTTS/MQTTv5.0 server to manage OTA updates. You can implement your own backend using our reference repository:

**MQTT/MQTTS/MQTTv5.0 Broker support OTA Updates**
- **Repository:** [github.com/Ruben890/Mqtt-Broker](https://github.com/Ruben890/Mqtt-Broker)
- **Description:** Complete backend for managing broker via MQTT/MQTTS/MQTTv5.0
- **Features:**
  - Configurable MQTT/MQTTS/MQTTv5.0 server
  - IoT device management
  - Firmware update delivery
  - OTA progress tracking
  - Error handling and retry mechanisms

**Steps to use the backend:**
1. Clone the backend repository
2. Configure the MQTT broker according to your needs
3. Implement the update delivery logic
4. Connect your ESP32 devices to the broker
5. Manage OTA updates from a centralized interface

**Example workflow:**
```javascript
// From your backend
1. Prepare firmware in base64 format
2. Publish MQTT/MQTTS/MQTT5 message to target device
3. Monitor progress via callbacks
4. Confirm successful completion
5. Log results in database
```

The backend provides a scalable architecture for managing multiple devices simultaneously, with support for mass updates and firmware version management.

---

##  Broker Compatibility

Tested with:
- [Mosquitto](https://mosquitto.org/) 2.0+ (with MQTT 5.0 enabled)
- [EMQX](https://www.emqx.com/) 5.0+
- [HiveMQ Cloud](https://www.hivemq.com/) (MQTT 5.0 plan)
- [AWS IoT Core](https://aws.amazon.com/iot-core/) (via custom authentication)

If you encounter issues, please open an issue with broker version and configuration.

---

##  License

This library is licensed under the **MIT License**. See [LICENSE](LICENSE) for details.

---


## Contact

Author: **Jorge Gaspar Beltre Rivera**  
Project: MQTTOTAv5 - For OTA Updates via MQTTv5/MQTTSv5

- [![GitHub](https://img.shields.io/badge/GitHub-181717?style=for-the-badge&logo=github&logoColor=white)](https://github.com/JorgeGBeltre)
- [![LinkedIn](https://img.shields.io/badge/LinkedIn-0A66C2?style=for-the-badge&logo=linkedin&logoColor=white)](https://www.linkedin.com/in/jorge-gaspar-beltre-rivera/)
- [![Email](https://img.shields.io/badge/Email-EA4335?style=for-the-badge&logo=gmail&logoColor=white)](mailto:Jorgegaspar3021@gmail.com)

---

##  Support

This project is developed independently.

Even a small contribution helps me dedicate more time to development, testing, and releasing new features.

- [![Buy Me a Coffee](https://img.shields.io/badge/Buy_Me_a_Coffee-FFDD00?style=for-the-badge&logo=buy-me-a-coffee&logoColor=black)](https://www.paypal.com/donate/?hosted_button_id=2VLA8BWT967LU)