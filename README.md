# ESP32_MQTTv5

[![Platform](https://img.shields.io/badge/platform-ESP32-blue)](https://www.espressif.com/en/products/socs/esp32)
[![MQTT Version](https://img.shields.io/badge/MQTT-5.0-brightgreen)](https://docs.oasis-open.org/mqtt/mqtt/v5.0/os/mqtt-v5.0-os.html)
[![License](https://img.shields.io/badge/license-MIT-green)](LICENSE)
[![Ask DeepWiki](https://deepwiki.com/badge.svg)](https://deepwiki.com/JorgeGBeltre/ESP32_MQTTv5)

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
- [Contact](#contact)
- [Support](#support)

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
| `hasWill()` | Returns `true` if a last will message is configured |
| `setRawExclusiveTopic(topic)` | Deliver `topic` exclusively to `onMessageRaw` without allocating a `String` copy for `onMessage` |
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
| `onMessage(cb)` | Called for each PUBLISH (payload copied to `String`, except for `setRawExclusiveTopic`) |
| `onMessageRaw(cb)` | **Zero‑copy** callback – payload as `const uint8_t*` (coexists with `onMessage`) |
| `onError(cb)` | Protocol errors, rejected `CONNACK` (`CONNACK 0xXX: <reason>`), timeouts, buffer overflows |
| `onAuth(cb)` | Enhanced authentication challenge (AUTH packet) |

#### Authentication
| Method | Description |
|--------|-------------|
| `auth(reason, props)` | Send AUTH packet (used inside `onAuth` callback) |

#### Utility
| Method | Description |
|--------|-------------|
| `lastReasonCode()` | Last reason code received from `CONNACK` or `DISCONNECT` |
| `lastReasonString()` | Reason string received from server `CONNACK` or `DISCONNECT` (preserved across disconnect cleanup) |
| `lastInboundProperties()` | Parsed `MQTTProperties` from the most recent inbound `PUBLISH` packet |

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
| `MQTT_MAX_AUTO_BUFFER_SIZE` | 8192 | Upper bound (bytes) for automatic buffer growth from `CONNACK` `MAXIMUM_PACKET_SIZE` |
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
- **Dedicated RX & TX reusable buffers** – Separate inbound (`_buffer`) and outbound (`_txBuffer`) buffers grow safely on demand (bounded by `MQTT_MAX_AUTO_BUFFER_SIZE` on `CONNACK` and capped at `65535 - 512` bytes on dynamic inbound resize) so reentrant publishes inside callbacks never corrupt incoming packets.
- **Strict Packet & Property Bounds Validation** – Validates variable-byte integer headers and property lengths (`propLen` / `pLen`) against `remLen` across `PUBLISH`, `CONNACK`, `PUBACK`, `AUTH`, and `DISCONNECT` frames before parsing properties or computing payload length, and verifies remaining TX capacity before encoding variable-byte headers in `buildConnect()` and `publish()`.
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

## Changelog

### v1.0.7
- Hardened dynamic receive buffer resizing in `parseIncoming()` (`_allowDynamicBufferResize`): requires `remLen <= (65535 - 512)` before growing the 16-bit `_desiredBufferSize` (`uint16_t`) and verifies `(int)remLen <= _bufferSize` after `allocBuffer()`, draining oversized frames in 64-byte chunks instead of overflowing `_buffer` (`CVE-2026-87121` class mitigation).
- Enforced strict variable-header and MQTT 5.0 property-length bounds validation (`enc > 0 && propLen >= 0 && (offset + enc + propLen) <= remLen`) across `MQTT_PUBLISH`, `MQTT_CONNACK`, `MQTT_PUBACK`, `MQTT_AUTH`, and `MQTT_DISCONNECT` in `parseIncoming()` and `connect()`, and verified `payloadLen >= 0` before invoking `_messageRawCallback` or `_messageCallback` (`CVE-2018-19417` and `CVE-2026-114550` class mitigations).
- Guarded `encodeLength(uint8_t *buf, uint32_t value)` when `buf == nullptr` and computed encoded variable-byte integer lengths before `BC_NEED` / `PB_NEED` bounds checks in `buildConnect()` and `publish()` so header bytes are never written prior to capacity verification.
- Updated `ESP32_MQTTV5_VERSION` macro and `library.properties` version to `"1.0.7"`.

### v1.0.6
- Drained buffered socket RX bytes (`_client->available()`) in `readByte()`, `readBytes()`, `connect()`, and `loop()` when the broker closes the TCP socket immediately after sending a rejected `CONNACK` or server-initiated `DISCONNECT` (e.g., `0x87 Not Authorized`), preventing premature `UNSPECIFIED_ERROR` disconnects before the control packet is parsed, and exiting `connect()` immediately if the peer closes the socket with no bytes pending.
- Preserved the exact server `ReasonCode` and `ReasonString` (`lastReasonCode()` / `lastReasonString()`) across `CONNACK` and `DISCONNECT` handling: cleared stale reason strings between frames, prevented `TopicAliasMaximum` clamping or `disconnect()` cleanup from overwriting a rejected `CONNACK` reason, and expanded the `CONNACK` error formatting buffer from `40` to `128` bytes (`CONNACK 0x%02X: %.96s`) so full diagnostic or cryptographic reason payloads (such as `ACCOUNT_UNLINKED:<proof>`) reach `onError()` and `lastReasonString()` intact.
- Added post-`parseIncoming()` state and socket checks in `loop()` to return cleanly without running QoS 2 retransmissions or `PINGREQ` keepalives after an inbound `DISCONNECT` or peer socket close.
- Updated `ESP32_MQTTV5_VERSION` macro and `library.properties` version to `"1.0.6"`.

### v1.0.5
- Added dedicated `_txBuffer` for `publish()`, `subscribe()`, and `unsubscribe()` so reentrant transmissions inside `onMessage` / `onConnect` callbacks never overwrite the inbound `_buffer`.
- Hardened `allocBuffer()` to allocate replacement buffers before freeing existing ones and bounded `CONNACK` `MAXIMUM_PACKET_SIZE` auto-resizing with `MQTT_MAX_AUTO_BUFFER_SIZE` (`8192` bytes by default) to prevent ESP32 heap exhaustion.

### v1.0.4
- Enabled clean coexistence between `onMessageRaw` and `onMessage` callbacks regardless of registration order, adding `setRawExclusiveTopic()` to avoid duplicate `String` payload allocations on dedicated binary topics (such as OTA streams).
- Added `lastInboundProperties()` so raw message handlers can inspect MQTT 5.0 User Properties, `Subscription Identifier`, `Correlation Data`, and `Response Topic` on incoming `PUBLISH` frames.
- Added `hasWill()` query helper and `ESP32_MQTTV5_VERSION` macro (`"1.0.4"`).

### v1.0.3
- Fixed a `StoreProhibited` core panic in `encodeLength` when calculating length without a buffer during topic subscription.

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
Project: ESP32_MQTTv5


<p align="center">
  <a href="https://www.linkedin.com/in/jorge-gaspar-beltre-rivera/" target="_blank"><img src="https://user-images.githubusercontent.com/74038190/235294012-0a55e343-37ad-4b0f-924f-c8431d9d2483.gif" alt="LinkedIn" width="100"></a>
  <a href="https://github.com/JorgeGBeltre" target="_blank"><img src="https://user-images.githubusercontent.com/74038190/212257468-1e9a91f1-b626-4baa-b15d-5c385dfa7ed2.gif" alt="GitHub" width="100"></a>
  <a href="mailto:Jorgegaspar3021@gmail.com"><img src="https://user-images.githubusercontent.com/74038190/216122065-2f028bae-25d6-4a3c-bc9f-175394ed5011.png" alt="E-Mail" width="100"></a>

</p>

## Support

This project is developed independently. Even a small contribution helps me dedicate more time to development, testing, and releasing new features.


 <p align="center">
  <a href="https://www.paypal.com/donate/?hosted_button_id=2VLA8BWT967LU">
    <img src="https://www.paypalobjects.com/webstatic/icon/pp258.png"
         alt="Donate with PayPal"
         height="60">
  </a>
</p>
