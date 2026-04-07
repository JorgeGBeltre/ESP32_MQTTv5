#ifndef ESP32_MQTTV5_H
#define ESP32_MQTTV5_H

#include <WiFi.h>
#include <functional>

#ifndef MQTT_MAX_PROPS
#define MQTT_MAX_PROPS 20
#endif

#ifndef MQTT_MAX_ALIASES
#define MQTT_MAX_ALIASES 16
#endif

#ifndef MQTT_MAX_QOS2_OUT
#define MQTT_MAX_QOS2_OUT 8
#endif

#ifndef MQTT_MAX_QOS2_IN
#define MQTT_MAX_QOS2_IN 8
#endif

#ifndef MQTT_DEFAULT_BUFFER_SIZE
#define MQTT_DEFAULT_BUFFER_SIZE 512
#endif

#ifndef MQTT_QOS2_RETRY_MS
#define MQTT_QOS2_RETRY_MS 5000UL
#endif

#ifndef MQTT_QOS2_MAX_RETRIES
#define MQTT_QOS2_MAX_RETRIES 3
#endif

#define MQTT_CONNECT 0x10
#define MQTT_CONNACK 0x20
#define MQTT_PUBLISH 0x30
#define MQTT_PUBACK 0x40
#define MQTT_PUBREC 0x50
#define MQTT_PUBREL 0x60
#define MQTT_PUBCOMP 0x70
#define MQTT_SUBSCRIBE 0x82
#define MQTT_SUBACK 0x90
#define MQTT_UNSUBSCRIBE 0xA2
#define MQTT_UNSUBACK 0xB0
#define MQTT_PINGREQ 0xC0
#define MQTT_PINGRESP 0xD0
#define MQTT_DISCONNECT 0xE0
#define MQTT_AUTH 0xF0

enum MQTTReasonCode : uint8_t {
  SUCCESS = 0x00,
  NORMAL_DISCONNECTION = 0x00,
  GRANTED_QOS_0 = 0x00,
  GRANTED_QOS_1 = 0x01,
  GRANTED_QOS_2 = 0x02,
  DISCONNECT_WITH_WILL_MESSAGE = 0x04,
  NO_MATCHING_SUBSCRIBERS = 0x10,
  NO_SUBSCRIPTION_EXISTED = 0x11,
  CONTINUE_AUTHENTICATION = 0x18,
  RE_AUTHENTICATE = 0x19,
  UNSPECIFIED_ERROR = 0x80,
  MALFORMED_PACKET = 0x81,
  PROTOCOL_ERROR = 0x82,
  IMPLEMENTATION_SPECIFIC_ERROR = 0x83,
  UNSUPPORTED_PROTOCOL_VERSION = 0x84,
  CLIENT_IDENTIFIER_NOT_VALID = 0x85,
  BAD_USERNAME_OR_PASSWORD = 0x86,
  NOT_AUTHORIZED = 0x87,
  SERVER_UNAVAILABLE = 0x88,
  SERVER_BUSY = 0x89,
  BANNED = 0x8A,
  SERVER_SHUTTING_DOWN = 0x8B,
  BAD_AUTHENTICATION_METHOD = 0x8C,
  KEEP_ALIVE_TIMEOUT = 0x8D,
  SESSION_TAKEN_OVER = 0x8E,
  TOPIC_FILTER_INVALID = 0x8F,
  TOPIC_NAME_INVALID = 0x90,
  PACKET_IDENTIFIER_IN_USE = 0x91,
  PACKET_IDENTIFIER_NOT_FOUND = 0x92,
  RECEIVE_MAXIMUM_EXCEEDED = 0x93,
  TOPIC_ALIAS_INVALID = 0x94,
  PACKET_TOO_LARGE = 0x95,
  MESSAGE_RATE_TOO_HIGH = 0x96,
  QUOTA_EXCEEDED = 0x97,
  ADMINISTRATIVE_ACTION = 0x98,
  PAYLOAD_FORMAT_INVALID_RC = 0x99,
  RETAIN_NOT_SUPPORTED = 0x9A,
  QOS_NOT_SUPPORTED = 0x9B,
  USE_ANOTHER_SERVER = 0x9C,
  SERVER_MOVED = 0x9D,
  SHARED_SUBSCRIPTIONS_NOT_SUPPORTED = 0x9E,
  CONNECTION_RATE_EXCEEDED = 0x9F,
  MAXIMUM_CONNECT_TIME = 0xA0,
  SUBSCRIPTION_IDENTIFIERS_NOT_SUPPORTED = 0xA1,
  WILDCARD_SUBSCRIPTIONS_NOT_SUPPORTED = 0xA2
};

enum MQTTPropertyID : uint8_t {
  PAYLOAD_FORMAT_INDICATOR = 0x01,
  MESSAGE_EXPIRY_INTERVAL = 0x02,
  CONTENT_TYPE = 0x03,
  RESPONSE_TOPIC = 0x08,
  CORRELATION_DATA = 0x09,
  SUBSCRIPTION_IDENTIFIER = 0x0B,
  SESSION_EXPIRY_INTERVAL = 0x11,
  ASSIGNED_CLIENT_IDENTIFIER = 0x12,
  SERVER_KEEP_ALIVE = 0x13,
  AUTHENTICATION_METHOD = 0x15,
  AUTHENTICATION_DATA = 0x16,
  REQUEST_PROBLEM_INFORMATION = 0x17,
  WILL_DELAY_INTERVAL = 0x18,
  REQUEST_RESPONSE_INFORMATION = 0x19,
  RESPONSE_INFORMATION = 0x1A,
  SERVER_REFERENCE = 0x1C,
  REASON_STRING = 0x1F,
  RECEIVE_MAXIMUM = 0x21,
  TOPIC_ALIAS_MAXIMUM = 0x22,
  TOPIC_ALIAS = 0x23,
  MAXIMUM_QOS = 0x24,
  RETAIN_AVAILABLE = 0x25,
  USER_PROPERTY = 0x26,
  MAXIMUM_PACKET_SIZE = 0x27,
  WILDCARD_SUBSCRIPTION_AVAILABLE = 0x28,
  SUBSCRIPTION_IDENTIFIER_AVAILABLE = 0x29,
  SHARED_SUBSCRIPTION_AVAILABLE = 0x2A
};

struct MQTTMessage {
  String topic;
  String payload;
  uint8_t qos;
  bool retain;
  uint8_t reasonCode;
  uint32_t messageExpiry;
  String responseTopic;
  String correlationData;
  String contentType;
  String reasonString;
  uint16_t topicAlias;
  uint32_t subscriptionIdentifier;
  uint8_t payloadFormatIndicator;
  bool hasTopicAlias;
  bool hasSubscriptionIdentifier;
  bool hasPayloadFormatIndicator;

  MQTTMessage()
      : qos(0), retain(false), reasonCode(0), messageExpiry(0), topicAlias(0),
        subscriptionIdentifier(0), payloadFormatIndicator(0),
        hasTopicAlias(false), hasSubscriptionIdentifier(false),
        hasPayloadFormatIndicator(false) {}
};

class MQTTProperties {
public:
  enum PropType : uint8_t {
    PTYPE_U8 = 0,
    PTYPE_U16 = 1,
    PTYPE_U32 = 2,
    PTYPE_STR = 3,
    PTYPE_BIN = 4,
    PTYPE_VINT = 5
  };

  struct Property {
    uint8_t id;
    PropType dataType;
    union {
      uint8_t u8;
      uint16_t u16;
      uint32_t u32;
    } num;
    String str;
    String key;
  };

private:
  Property _props[MQTT_MAX_PROPS];
  uint8_t _propCount;

public:
  MQTTProperties();
  void clear();

  int serialize(uint8_t *buf, int bufSize) const;

  int parse(const uint8_t *data, int length);

  void addUserProperty(const String &key, const String &value);
  void setMessageExpiry(uint32_t seconds);
  void setContentType(const String &type);
  void setResponseTopic(const String &topic);
  void setCorrelationData(const String &data);
  void setCorrelationData(const uint8_t *data, size_t len);
  void setSessionExpiry(uint32_t seconds);
  void setWillDelay(uint32_t seconds);
  void setRequestProblemInfo(bool enable);
  void setRequestResponseInfo(bool enable);
  void setReasonString(const String &reason);
  void setPayloadFormat(uint8_t format);
  void setSubscriptionIdentifier(uint32_t identifier);
  void setAssignedClientId(const String &id);
  void setAuthenticationMethod(const String &method);
  void setAuthenticationData(const uint8_t *data, size_t len);
  void setResponseInfo(const String &info);
  void setServerReference(const String &ref);
  void setServerKeepAlive(uint16_t seconds);
  void setReceiveMaximum(uint16_t max);
  void setTopicAliasMaximum(uint16_t max);
  void setTopicAlias(uint16_t alias);
  void setMaximumQoS(uint8_t qos);
  void setRetainAvailable(bool available);
  void setWildcardSubscriptionAvailable(bool available);
  void setSubscriptionIdentifierAvailable(bool available);
  void setSharedSubscriptionAvailable(bool available);
  void setMaximumPacketSize(uint32_t size);

  String getUserProperty(const String &key) const;
  String getReasonString() const;
  uint32_t getSubscriptionIdentifier() const;
  uint16_t getTopicAlias() const;
  uint8_t getPayloadFormat() const;
  uint16_t getUint16(uint8_t propId, uint16_t defaultVal = 0) const;
  uint32_t getUint32(uint8_t propId, uint32_t defaultVal = 0) const;
  uint8_t getUint8(uint8_t propId, uint8_t defaultVal = 0) const;
  String getString(uint8_t propId) const;

  uint8_t count() const { return _propCount; }
  const Property &propAt(uint8_t i) const { return _props[i]; }
};

class ESP32_MQTTv5 {
public:
  ESP32_MQTTv5();
  ~ESP32_MQTTv5();

  void begin(const char *host, uint16_t port);
  void begin(WiFiClient &client, const char *host, uint16_t port);

  void setBufferSize(uint16_t size);

  void setKeepAlive(uint16_t seconds);
  void setCleanStart(bool clean);
  void setSessionExpiry(uint32_t seconds);
  void setTopicAliasMaximum(uint16_t max);

  void setWill(const String &topic, const String &payload, uint8_t qos = 0,
               bool retain = false, MQTTProperties *props = nullptr);

  void setAutoReconnect(bool enable, uint32_t initialDelayMs = 1000,
                        uint32_t maxDelayMs = 30000);

  void setDynamicBufferResize(bool enable) {
    _allowDynamicBufferResize = enable;
  }

  void setReadTimeout(uint32_t ms) { _readTimeoutMs = ms; }

  bool connect(const String &clientId);
  bool connect(const String &clientId, const String &username,
               const String &password);
  bool connect(const String &clientId, const String &username,
               const String &password, MQTTProperties *extraProps);
  void disconnect(MQTTReasonCode reason = NORMAL_DISCONNECTION);
  bool connected();
  void loop();

  void auth(MQTTReasonCode reason, MQTTProperties &props);

  bool publish(const String &topic, const String &payload);
  bool publish(const String &topic, const String &payload, uint8_t qos);
  bool publish(const String &topic, const String &payload,
               MQTTProperties &props);
  bool publish(const String &topic, const String &payload, uint8_t qos,
               bool retain, MQTTProperties *props = nullptr);

  bool subscribe(const String &topic);
  bool subscribe(const String &topic, uint8_t qos);

  bool subscribe(const String &topic, uint8_t qos, bool noLocal,
                 bool retainAsPublished, uint8_t retainHandling,
                 uint32_t subId = 0);

  bool subscribeShared(const String &group, const String &topic,
                       uint8_t qos = 0) {
    return subscribe("$share/" + group + "/" + topic, qos);
  }

  bool unsubscribe(const String &topic);

  void onConnect(std::function<void()> cb);
  void onDisconnect(std::function<void(MQTTReasonCode)> cb);

  void onMessage(std::function<void(const MQTTMessage &)> cb);

  void onError(std::function<void(MQTTReasonCode, const String &)> cb);
  void onAuth(std::function<void(MQTTReasonCode, const MQTTProperties &)> cb);

  void
  onMessageRaw(std::function<void(const String &topic, const uint8_t *payload,
                                  size_t len, uint8_t qos, bool retain)>
                   cb);

  MQTTReasonCode lastReasonCode() const { return _lastReasonCode; }
  String lastReasonString() const { return _lastReasonString; }

private:
  enum State : uint8_t {
    STATE_DISCONNECTED,
    STATE_CONNECTING,
    STATE_CONNECTED,
    STATE_DISCONNECTING
  } _state;

  WiFiClient *_client;
  bool _ownClient;
  String _host;
  uint16_t _port;

  String _clientId;
  String _username;
  String _password;
  uint16_t _keepAlive;
  bool _cleanStart;
  uint32_t _sessionExpiry;

  bool _hasWill;
  String _willTopic;
  String _willPayload;
  uint8_t _willQos;
  bool _willRetain;
  MQTTProperties _willProps;

  std::function<void()> _connectCallback;
  std::function<void(MQTTReasonCode)> _disconnectCallback;
  std::function<void(const MQTTMessage &)> _messageCallback;
  std::function<void(MQTTReasonCode, const String &)> _errorCallback;
  std::function<void(MQTTReasonCode, const MQTTProperties &)> _authCallback;

  std::function<void(const String &, const uint8_t *, size_t, uint8_t, bool)>
      _messageRawCallback;

  uint16_t _topicAliasMaximum;
  uint16_t _serverTopicAliasMaximum;
  struct OutAlias {
    String topic;
    uint16_t alias;
  };
  OutAlias _outAliases[MQTT_MAX_ALIASES];
  uint8_t _outAliasCount;
  uint16_t _nextAliasId;

  String _inAliasTopics[MQTT_MAX_ALIASES];

  uint8_t *_buffer;
  uint16_t _bufferSize;
  uint16_t _desiredBufferSize;

  bool _allowDynamicBufferResize;

  uint32_t _readTimeoutMs;

  struct QoS2OutEntry {
    uint16_t packetId;
    unsigned long lastTxMs;
    bool pubrelSent;
    bool active;
    uint8_t *pubPacket;
    uint16_t pubPacketLen;
    uint8_t retryCount;
  };
  QoS2OutEntry _qos2Out[MQTT_MAX_QOS2_OUT];
  uint8_t _qos2OutCount;

  uint16_t _qos2In[MQTT_MAX_QOS2_IN];
  uint8_t _qos2InCount;

  bool _autoReconnect;
  uint32_t _reconnectInitialDelay;
  uint32_t _reconnectMaxDelay;
  uint32_t _currentReconnectDelay;
  unsigned long _lastReconnectAttempt;

  unsigned long _lastOutbound;
  unsigned long _lastInbound;

  uint16_t _nextPacketId;

  MQTTReasonCode _lastReasonCode;
  String _lastReasonString;

  uint32_t _serverMaxPacketSize;

  bool connectTransport();
  void disconnectTransport();
  int sendPacket(const uint8_t *packet, int length);
  int readByte();
  int readBytes(uint8_t *buf, int len);
  uint16_t getNextPacketId();
  int encodeLength(uint8_t *buf, uint32_t value);
  int decodeLength(const uint8_t *buf, int &value);

  bool allocBuffer();
  void freeBuffer();
  bool parseIncoming();

  int buildConnect(uint8_t *buf, int bufSize,
                   MQTTProperties *extraProps = nullptr);
  int buildAuth(uint8_t *buf, int bufSize, MQTTReasonCode reason,
                MQTTProperties *props);
  bool sendAuth(MQTTReasonCode reason, MQTTProperties &props);

  bool addQoS2Out(uint16_t packetId, const uint8_t *packet, int len);
  void handlePubrec(uint16_t packetId);
  void handlePubcomp(uint16_t packetId);
  void retransmitQoS2();
  void freeQoS2Out(uint8_t index);

  bool isQoS2InFlight(uint16_t packetId) const;
  void addQoS2In(uint16_t packetId);
  void removeQoS2In(uint16_t packetId);

  uint16_t findOrAssignOutAlias(const String &topic, bool &sendTopicName);
  String resolveInAlias(uint16_t alias, const String &topicInPacket);

  void notifyError(MQTTReasonCode code, const String &msg);

  void resetSession();
};

#endif // ESP32_MQTTV5_H