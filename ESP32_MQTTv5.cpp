#include "ESP32_MQTTv5.h"

MQTTProperties::MQTTProperties() : _propCount(0) {}

void MQTTProperties::clear() { _propCount = 0; }

static inline bool _propAdd(MQTTProperties::Property *props, uint8_t &count,
                            uint8_t id, MQTTProperties::PropType type) {
  if (count >= MQTT_MAX_PROPS)
    return false;
  props[count].id = id;
  props[count].dataType = type;
  props[count].num.u32 = 0;
  props[count].str = String();
  props[count].key = String();
  return true;
}

void MQTTProperties::addUserProperty(const String &key, const String &value) {
  if (_propCount >= MQTT_MAX_PROPS)
    return;
  Property &p = _props[_propCount++];
  p.id = USER_PROPERTY;
  p.dataType = PTYPE_STR;
  p.str = value;
  p.key = key;
}

void MQTTProperties::setMessageExpiry(uint32_t s) {
  if (_propCount >= MQTT_MAX_PROPS)
    return;
  Property &p = _props[_propCount++];
  p.id = MESSAGE_EXPIRY_INTERVAL;
  p.dataType = PTYPE_U32;
  p.num.u32 = s;
}
void MQTTProperties::setContentType(const String &v) {
  if (_propCount >= MQTT_MAX_PROPS)
    return;
  Property &p = _props[_propCount++];
  p.id = CONTENT_TYPE;
  p.dataType = PTYPE_STR;
  p.str = v;
}
void MQTTProperties::setResponseTopic(const String &v) {
  if (_propCount >= MQTT_MAX_PROPS)
    return;
  Property &p = _props[_propCount++];
  p.id = RESPONSE_TOPIC;
  p.dataType = PTYPE_STR;
  p.str = v;
}
void MQTTProperties::setCorrelationData(const String &v) {
  if (_propCount >= MQTT_MAX_PROPS)
    return;
  Property &p = _props[_propCount++];
  p.id = CORRELATION_DATA;
  p.dataType = PTYPE_BIN;
  p.str = v;
}
void MQTTProperties::setCorrelationData(const uint8_t *data, size_t len) {
  if (_propCount >= MQTT_MAX_PROPS)
    return;
  Property &p = _props[_propCount++];
  p.id = CORRELATION_DATA;
  p.dataType = PTYPE_BIN;
  p.str = String((const char *)data, len);
}
void MQTTProperties::setSessionExpiry(uint32_t s) {
  if (_propCount >= MQTT_MAX_PROPS)
    return;
  Property &p = _props[_propCount++];
  p.id = SESSION_EXPIRY_INTERVAL;
  p.dataType = PTYPE_U32;
  p.num.u32 = s;
}
void MQTTProperties::setWillDelay(uint32_t s) {
  if (_propCount >= MQTT_MAX_PROPS)
    return;
  Property &p = _props[_propCount++];
  p.id = WILL_DELAY_INTERVAL;
  p.dataType = PTYPE_U32;
  p.num.u32 = s;
}
void MQTTProperties::setRequestProblemInfo(bool v) {
  if (_propCount >= MQTT_MAX_PROPS)
    return;
  Property &p = _props[_propCount++];
  p.id = REQUEST_PROBLEM_INFORMATION;
  p.dataType = PTYPE_U8;
  p.num.u8 = v ? 1 : 0;
}
void MQTTProperties::setRequestResponseInfo(bool v) {
  if (_propCount >= MQTT_MAX_PROPS)
    return;
  Property &p = _props[_propCount++];
  p.id = REQUEST_RESPONSE_INFORMATION;
  p.dataType = PTYPE_U8;
  p.num.u8 = v ? 1 : 0;
}
void MQTTProperties::setReasonString(const String &v) {
  if (_propCount >= MQTT_MAX_PROPS)
    return;
  Property &p = _props[_propCount++];
  p.id = REASON_STRING;
  p.dataType = PTYPE_STR;
  p.str = v;
}
void MQTTProperties::setPayloadFormat(uint8_t v) {
  if (_propCount >= MQTT_MAX_PROPS)
    return;
  Property &p = _props[_propCount++];
  p.id = PAYLOAD_FORMAT_INDICATOR;
  p.dataType = PTYPE_U8;
  p.num.u8 = v;
}
void MQTTProperties::setSubscriptionIdentifier(uint32_t v) {
  if (_propCount >= MQTT_MAX_PROPS)
    return;
  Property &p = _props[_propCount++];
  p.id = SUBSCRIPTION_IDENTIFIER;
  p.dataType = PTYPE_VINT;
  p.num.u32 = v;
}
void MQTTProperties::setAssignedClientId(const String &v) {
  if (_propCount >= MQTT_MAX_PROPS)
    return;
  Property &p = _props[_propCount++];
  p.id = ASSIGNED_CLIENT_IDENTIFIER;
  p.dataType = PTYPE_STR;
  p.str = v;
}
void MQTTProperties::setAuthenticationMethod(const String &v) {
  if (_propCount >= MQTT_MAX_PROPS)
    return;
  Property &p = _props[_propCount++];
  p.id = AUTHENTICATION_METHOD;
  p.dataType = PTYPE_STR;
  p.str = v;
}
void MQTTProperties::setAuthenticationData(const uint8_t *data, size_t len) {
  if (_propCount >= MQTT_MAX_PROPS)
    return;
  Property &p = _props[_propCount++];
  p.id = AUTHENTICATION_DATA;
  p.dataType = PTYPE_BIN;
  p.str = String((const char *)data, len);
}
void MQTTProperties::setResponseInfo(const String &v) {
  if (_propCount >= MQTT_MAX_PROPS)
    return;
  Property &p = _props[_propCount++];
  p.id = RESPONSE_INFORMATION;
  p.dataType = PTYPE_STR;
  p.str = v;
}
void MQTTProperties::setServerReference(const String &v) {
  if (_propCount >= MQTT_MAX_PROPS)
    return;
  Property &p = _props[_propCount++];
  p.id = SERVER_REFERENCE;
  p.dataType = PTYPE_STR;
  p.str = v;
}
void MQTTProperties::setServerKeepAlive(uint16_t v) {
  if (_propCount >= MQTT_MAX_PROPS)
    return;
  Property &p = _props[_propCount++];
  p.id = SERVER_KEEP_ALIVE;
  p.dataType = PTYPE_U16;
  p.num.u16 = v;
}
void MQTTProperties::setReceiveMaximum(uint16_t v) {
  if (_propCount >= MQTT_MAX_PROPS)
    return;
  Property &p = _props[_propCount++];
  p.id = RECEIVE_MAXIMUM;
  p.dataType = PTYPE_U16;
  p.num.u16 = v;
}
void MQTTProperties::setTopicAliasMaximum(uint16_t v) {
  if (_propCount >= MQTT_MAX_PROPS)
    return;
  Property &p = _props[_propCount++];
  p.id = TOPIC_ALIAS_MAXIMUM;
  p.dataType = PTYPE_U16;
  p.num.u16 = v;
}
void MQTTProperties::setTopicAlias(uint16_t v) {
  if (_propCount >= MQTT_MAX_PROPS)
    return;
  Property &p = _props[_propCount++];
  p.id = TOPIC_ALIAS;
  p.dataType = PTYPE_U16;
  p.num.u16 = v;
}
void MQTTProperties::setMaximumQoS(uint8_t v) {
  if (_propCount >= MQTT_MAX_PROPS)
    return;
  Property &p = _props[_propCount++];
  p.id = MAXIMUM_QOS;
  p.dataType = PTYPE_U8;
  p.num.u8 = v;
}
void MQTTProperties::setRetainAvailable(bool v) {
  if (_propCount >= MQTT_MAX_PROPS)
    return;
  Property &p = _props[_propCount++];
  p.id = RETAIN_AVAILABLE;
  p.dataType = PTYPE_U8;
  p.num.u8 = v ? 1 : 0;
}
void MQTTProperties::setWildcardSubscriptionAvailable(bool v) {
  if (_propCount >= MQTT_MAX_PROPS)
    return;
  Property &p = _props[_propCount++];
  p.id = WILDCARD_SUBSCRIPTION_AVAILABLE;
  p.dataType = PTYPE_U8;
  p.num.u8 = v ? 1 : 0;
}
void MQTTProperties::setSubscriptionIdentifierAvailable(bool v) {
  if (_propCount >= MQTT_MAX_PROPS)
    return;
  Property &p = _props[_propCount++];
  p.id = SUBSCRIPTION_IDENTIFIER_AVAILABLE;
  p.dataType = PTYPE_U8;
  p.num.u8 = v ? 1 : 0;
}
void MQTTProperties::setSharedSubscriptionAvailable(bool v) {
  if (_propCount >= MQTT_MAX_PROPS)
    return;
  Property &p = _props[_propCount++];
  p.id = SHARED_SUBSCRIPTION_AVAILABLE;
  p.dataType = PTYPE_U8;
  p.num.u8 = v ? 1 : 0;
}
void MQTTProperties::setMaximumPacketSize(uint32_t v) {
  if (_propCount >= MQTT_MAX_PROPS)
    return;
  Property &p = _props[_propCount++];
  p.id = MAXIMUM_PACKET_SIZE;
  p.dataType = PTYPE_U32;
  p.num.u32 = v;
}

int MQTTProperties::serialize(uint8_t *buf, int bufSize) const {
  int pos = 0;

#define NEED(n)                                                                \
  do {                                                                         \
    if (pos + (n) > bufSize)                                                   \
      return -1;                                                               \
  } while (0)

  for (uint8_t i = 0; i < _propCount; ++i) {
    const Property &prop = _props[i];

    NEED(1);
    buf[pos++] = prop.id;

    switch (prop.dataType) {
    case PTYPE_U8:
      NEED(1);
      buf[pos++] = prop.num.u8;
      break;

    case PTYPE_U16:
      NEED(2);
      buf[pos++] = (prop.num.u16 >> 8) & 0xFF;
      buf[pos++] = prop.num.u16 & 0xFF;
      break;

    case PTYPE_U32:
      NEED(4);
      buf[pos++] = (prop.num.u32 >> 24) & 0xFF;
      buf[pos++] = (prop.num.u32 >> 16) & 0xFF;
      buf[pos++] = (prop.num.u32 >> 8) & 0xFF;
      buf[pos++] = prop.num.u32 & 0xFF;
      break;

    case PTYPE_STR:
    case PTYPE_BIN: {
      if (prop.id == USER_PROPERTY) {

        uint16_t kl = (uint16_t)prop.key.length();
        NEED(2 + kl);
        buf[pos++] = (kl >> 8) & 0xFF;
        buf[pos++] = kl & 0xFF;
        memcpy(&buf[pos], prop.key.c_str(), kl);
        pos += kl;
      }
      uint16_t vl = (uint16_t)prop.str.length();
      NEED(2 + vl);
      buf[pos++] = (vl >> 8) & 0xFF;
      buf[pos++] = vl & 0xFF;
      memcpy(&buf[pos], prop.str.c_str(), vl);
      pos += vl;
      break;
    }

    case PTYPE_VINT: {

      uint32_t val = prop.num.u32;
      do {
        NEED(1);
        uint8_t byte = val & 0x7F;
        val >>= 7;
        if (val > 0)
          byte |= 0x80;
        buf[pos++] = byte;
      } while (val > 0);
      break;
    }
    }
  }

#undef NEED
  return pos;
}

int MQTTProperties::parse(const uint8_t *data, int length) {
  _propCount = 0;
  int pos = 0;

  while (pos < length && _propCount < MQTT_MAX_PROPS) {
    if (pos >= length)
      break;
    uint8_t id = data[pos++];

    Property &prop = _props[_propCount];
    prop.id = id;
    prop.num.u32 = 0;
    prop.str = String();
    prop.key = String();

    switch (id) {

    case PAYLOAD_FORMAT_INDICATOR:
    case REQUEST_PROBLEM_INFORMATION:
    case REQUEST_RESPONSE_INFORMATION:
    case MAXIMUM_QOS:
    case RETAIN_AVAILABLE:
    case WILDCARD_SUBSCRIPTION_AVAILABLE:
    case SUBSCRIPTION_IDENTIFIER_AVAILABLE:
    case SHARED_SUBSCRIPTION_AVAILABLE:
      if (pos >= length)
        goto done;
      prop.dataType = PTYPE_U8;
      prop.num.u8 = data[pos++];
      break;

    case SERVER_KEEP_ALIVE:
    case RECEIVE_MAXIMUM:
    case TOPIC_ALIAS_MAXIMUM:
    case TOPIC_ALIAS:
      if (pos + 2 > length)
        goto done;
      prop.dataType = PTYPE_U16;
      prop.num.u16 = ((uint16_t)data[pos] << 8) | data[pos + 1];
      pos += 2;
      break;

    case MESSAGE_EXPIRY_INTERVAL:
    case SESSION_EXPIRY_INTERVAL:
    case WILL_DELAY_INTERVAL:
    case MAXIMUM_PACKET_SIZE:
      if (pos + 4 > length)
        goto done;
      prop.dataType = PTYPE_U32;
      prop.num.u32 = ((uint32_t)data[pos] << 24) |
                     ((uint32_t)data[pos + 1] << 16) |
                     ((uint32_t)data[pos + 2] << 8) | (uint32_t)data[pos + 3];
      pos += 4;
      break;

    case CONTENT_TYPE:
    case RESPONSE_TOPIC:
    case ASSIGNED_CLIENT_IDENTIFIER:
    case AUTHENTICATION_METHOD:
    case REASON_STRING:
    case RESPONSE_INFORMATION:
    case SERVER_REFERENCE: {
      if (pos + 2 > length)
        goto done;
      uint16_t slen = ((uint16_t)data[pos] << 8) | data[pos + 1];
      pos += 2;
      if (pos + slen > length)
        goto done;
      prop.dataType = PTYPE_STR;
      prop.str = String((const char *)&data[pos], slen);
      pos += slen;
      break;
    }

    case CORRELATION_DATA:
    case AUTHENTICATION_DATA: {
      if (pos + 2 > length)
        goto done;
      uint16_t blen = ((uint16_t)data[pos] << 8) | data[pos + 1];
      pos += 2;
      if (pos + blen > length)
        goto done;
      prop.dataType = PTYPE_BIN;
      prop.str = String((const char *)&data[pos], blen);
      pos += blen;
      break;
    }

    case USER_PROPERTY: {
      if (pos + 2 > length)
        goto done;
      uint16_t kl = ((uint16_t)data[pos] << 8) | data[pos + 1];
      pos += 2;
      if (pos + kl > length)
        goto done;
      prop.key = String((const char *)&data[pos], kl);
      pos += kl;

      if (pos + 2 > length)
        goto done;
      uint16_t vl = ((uint16_t)data[pos] << 8) | data[pos + 1];
      pos += 2;
      if (pos + vl > length)
        goto done;
      prop.dataType = PTYPE_STR;
      prop.str = String((const char *)&data[pos], vl);
      pos += vl;
      break;
    }

    case SUBSCRIPTION_IDENTIFIER: {
      uint32_t val = 0, mult = 1;
      uint8_t digit;
      do {
        if (pos >= length)
          goto done;
        digit = data[pos++];
        val += (digit & 0x7F) * mult;
        mult *= 128;
      } while ((digit & 0x80) && mult <= (128 * 128 * 128));
      prop.dataType = PTYPE_VINT;
      prop.num.u32 = val;
      break;
    }

    default:

      goto done;
    }

    _propCount++;
  }

done:
  return pos;
}

String MQTTProperties::getUserProperty(const String &key) const {
  for (uint8_t i = 0; i < _propCount; ++i)
    if (_props[i].id == USER_PROPERTY && _props[i].key == key)
      return _props[i].str;
  return String();
}

String MQTTProperties::getReasonString() const {
  return getString(REASON_STRING);
}

uint32_t MQTTProperties::getSubscriptionIdentifier() const {
  for (uint8_t i = 0; i < _propCount; ++i)
    if (_props[i].id == SUBSCRIPTION_IDENTIFIER)
      return _props[i].num.u32;
  return 0;
}

uint16_t MQTTProperties::getTopicAlias() const {
  return getUint16(TOPIC_ALIAS, 0);
}

uint8_t MQTTProperties::getPayloadFormat() const {
  return getUint8(PAYLOAD_FORMAT_INDICATOR, 0);
}

uint16_t MQTTProperties::getUint16(uint8_t propId, uint16_t def) const {
  for (uint8_t i = 0; i < _propCount; ++i)
    if (_props[i].id == propId && _props[i].dataType == PTYPE_U16)
      return _props[i].num.u16;
  return def;
}

uint32_t MQTTProperties::getUint32(uint8_t propId, uint32_t def) const {
  for (uint8_t i = 0; i < _propCount; ++i)
    if (_props[i].id == propId &&
        (_props[i].dataType == PTYPE_U32 || _props[i].dataType == PTYPE_VINT))
      return _props[i].num.u32;
  return def;
}

uint8_t MQTTProperties::getUint8(uint8_t propId, uint8_t def) const {
  for (uint8_t i = 0; i < _propCount; ++i)
    if (_props[i].id == propId && _props[i].dataType == PTYPE_U8)
      return _props[i].num.u8;
  return def;
}

String MQTTProperties::getString(uint8_t propId) const {
  for (uint8_t i = 0; i < _propCount; ++i)
    if (_props[i].id == propId &&
        (_props[i].dataType == PTYPE_STR || _props[i].dataType == PTYPE_BIN))
      return _props[i].str;
  return String();
}

ESP32_MQTTv5::ESP32_MQTTv5()
    : _state(STATE_DISCONNECTED), _client(nullptr), _ownClient(false),
      _port(1883), _keepAlive(60), _cleanStart(true), _sessionExpiry(0),
      _hasWill(false), _willQos(0), _willRetain(false), _topicAliasMaximum(0),
      _serverTopicAliasMaximum(0), _outAliasCount(0), _nextAliasId(1),
      _buffer(nullptr), _bufferSize(0),
      _desiredBufferSize(MQTT_DEFAULT_BUFFER_SIZE),
      _allowDynamicBufferResize(true), _readTimeoutMs(2000), _qos2OutCount(0),
      _qos2InCount(0), _autoReconnect(false), _reconnectInitialDelay(1000),
      _reconnectMaxDelay(30000), _currentReconnectDelay(1000),
      _lastReconnectAttempt(0), _lastOutbound(0), _lastInbound(0),
      _nextPacketId(1), _lastReasonCode(SUCCESS), _serverMaxPacketSize(0) {

  for (uint8_t i = 0; i < MQTT_MAX_QOS2_OUT; ++i) {
    _qos2Out[i].active = false;
    _qos2Out[i].pubPacket = nullptr;
    _qos2Out[i].retryCount = 0;
  }
}

ESP32_MQTTv5::~ESP32_MQTTv5() {
  disconnect();
  freeBuffer();
  if (_ownClient && _client) {
    delete _client;
    _client = nullptr;
  }
}

void ESP32_MQTTv5::begin(const char *host, uint16_t port) {
  _host = host;
  _port = port;
  if (!_ownClient || !_client) {
    if (_ownClient && _client)
      delete _client;
    _client = new WiFiClient();
    _ownClient = true;
  }
}

void ESP32_MQTTv5::begin(WiFiClient &client, const char *host, uint16_t port) {
  if (_ownClient && _client) {
    delete _client;
  }
  _client = &client;
  _ownClient = false;
  _host = host;
  _port = port;
}

void ESP32_MQTTv5::setBufferSize(uint16_t size) {
  _desiredBufferSize = (size < 32) ? 32 : size;
}

void ESP32_MQTTv5::setKeepAlive(uint16_t s) { _keepAlive = s; }
void ESP32_MQTTv5::setCleanStart(bool v) { _cleanStart = v; }
void ESP32_MQTTv5::setSessionExpiry(uint32_t s) { _sessionExpiry = s; }
void ESP32_MQTTv5::setTopicAliasMaximum(uint16_t m) { _topicAliasMaximum = m; }

void ESP32_MQTTv5::setWill(const String &topic, const String &payload,
                           uint8_t qos, bool retain, MQTTProperties *props) {
  _hasWill = true;
  _willTopic = topic;
  _willPayload = payload;
  _willQos = qos & 0x03;
  _willRetain = retain;
  _willProps.clear();
  if (props) {
    for (uint8_t i = 0; i < props->count(); ++i) {
      const MQTTProperties::Property &src = props->propAt(i);
      switch (src.id) {
      case WILL_DELAY_INTERVAL:
        _willProps.setWillDelay(src.num.u32);
        break;
      case PAYLOAD_FORMAT_INDICATOR:
        _willProps.setPayloadFormat(src.num.u8);
        break;
      case MESSAGE_EXPIRY_INTERVAL:
        _willProps.setMessageExpiry(src.num.u32);
        break;
      case CONTENT_TYPE:
        _willProps.setContentType(src.str);
        break;
      case RESPONSE_TOPIC:
        _willProps.setResponseTopic(src.str);
        break;
      case CORRELATION_DATA:
        _willProps.setCorrelationData(src.str);
        break;
      case USER_PROPERTY:
        _willProps.addUserProperty(src.key, src.str);
        break;
      default:
        break;
      }
    }
  }
}

void ESP32_MQTTv5::setAutoReconnect(bool enable, uint32_t initDelay,
                                    uint32_t maxDelay) {
  _autoReconnect = enable;
  _reconnectInitialDelay = initDelay;
  _reconnectMaxDelay = maxDelay;
  _currentReconnectDelay = initDelay;
}

bool ESP32_MQTTv5::allocBuffer() {
  if (_buffer && _bufferSize == _desiredBufferSize)
    return true;
  freeBuffer();
  _buffer = (uint8_t *)malloc(_desiredBufferSize);
  if (!_buffer)
    return false;
  _bufferSize = _desiredBufferSize;
  return true;
}

void ESP32_MQTTv5::freeBuffer() {
  if (_buffer) {
    free(_buffer);
    _buffer = nullptr;
    _bufferSize = 0;
  }
}

bool ESP32_MQTTv5::connectTransport() {
  if (!_client)
    return false;
  if (!_client->connected()) {
    if (!_client->connect(_host.c_str(), _port))
      return false;
  }
  return true;
}

void ESP32_MQTTv5::disconnectTransport() {
  if (_client)
    _client->stop();
}

int ESP32_MQTTv5::sendPacket(const uint8_t *packet, int length) {
  if (!_client || !_client->connected())
    return 0;
  int sent = _client->write(packet, length);
  if (sent > 0)
    _lastOutbound = millis();
  return sent;
}

int ESP32_MQTTv5::readByte() {
  if (!_client || !_client->connected())
    return -1;
  int b = _client->read();
  if (b != -1)
    _lastInbound = millis();
  return b;
}

int ESP32_MQTTv5::readBytes(uint8_t *buf, int len) {
  if (!_client || !_client->connected())
    return 0;

  unsigned long deadline = millis() + _readTimeoutMs;
  int got = 0;
  while (got < len) {
    if (millis() >= deadline)
      break;
    if (_client->available()) {
      buf[got++] = (uint8_t)_client->read();
      deadline = millis() + _readTimeoutMs;
    } else {
      delay(0);
    }
  }
  if (got > 0)
    _lastInbound = millis();
  return got;
}

uint16_t ESP32_MQTTv5::getNextPacketId() {
  if (++_nextPacketId == 0)
    _nextPacketId = 1;
  return _nextPacketId;
}

int ESP32_MQTTv5::encodeLength(uint8_t *buf, uint32_t value) {
  int bytes = 0;
  do {
    uint8_t byte = value & 0x7F;
    value >>= 7;
    if (value > 0)
      byte |= 0x80;
    buf[bytes++] = byte;
  } while (value > 0 && bytes < 4);
  return bytes;
}

int ESP32_MQTTv5::decodeLength(const uint8_t *buf, int &value) {
  int bytes = 0, multiplier = 1;
  value = 0;
  uint8_t digit;
  do {
    digit = buf[bytes++];
    value += (digit & 0x7F) * multiplier;
    multiplier *= 128;
  } while ((digit & 0x80) && bytes < 4);
  return bytes;
}

void ESP32_MQTTv5::notifyError(MQTTReasonCode code, const String &msg) {
  _lastReasonCode = code;
  _lastReasonString = msg;
  if (_errorCallback)
    _errorCallback(code, msg);
}

void ESP32_MQTTv5::resetSession() {
  _outAliasCount = 0;
  _nextAliasId = 1;
  _serverTopicAliasMaximum = 0;
  _serverMaxPacketSize = 0;
  _qos2InCount = 0;

  for (uint8_t i = 0; i < _qos2OutCount; ++i) {
    if (_qos2Out[i].pubPacket) {
      free(_qos2Out[i].pubPacket);
      _qos2Out[i].pubPacket = nullptr;
    }
  }
  _qos2OutCount = 0;
  for (uint8_t i = 0; i < MQTT_MAX_ALIASES; ++i)
    _inAliasTopics[i] = String();
}

void ESP32_MQTTv5::onConnect(std::function<void()> cb) {
  _connectCallback = cb;
}
void ESP32_MQTTv5::onDisconnect(std::function<void(MQTTReasonCode)> cb) {
  _disconnectCallback = cb;
}
void ESP32_MQTTv5::onMessage(std::function<void(const MQTTMessage &)> cb) {
  _messageCallback = cb;
}
void ESP32_MQTTv5::onError(
    std::function<void(MQTTReasonCode, const String &)> cb) {
  _errorCallback = cb;
}
void ESP32_MQTTv5::onAuth(
    std::function<void(MQTTReasonCode, const MQTTProperties &)> cb) {
  _authCallback = cb;
}
void ESP32_MQTTv5::onMessageRaw(
    std::function<void(const String &, const uint8_t *, size_t, uint8_t, bool)>
        cb) {
  _messageRawCallback = cb;
}

int ESP32_MQTTv5::buildConnect(uint8_t *buf, int bufSize,
                               MQTTProperties *extraProps) {
  int pos = 0;

#define BC_NEED(n)                                                             \
  do {                                                                         \
    if (pos + (n) > bufSize)                                                   \
      return -1;                                                               \
  } while (0)

  BC_NEED(6);
  buf[pos++] = 0x00;
  buf[pos++] = 0x04;
  buf[pos++] = 'M';
  buf[pos++] = 'Q';
  buf[pos++] = 'T';
  buf[pos++] = 'T';

  BC_NEED(1);
  buf[pos++] = 0x05;

  uint8_t flags = 0;
  if (_cleanStart)
    flags |= 0x02;
  if (_hasWill) {
    flags |= 0x04;
    flags |= ((_willQos & 0x03) << 3);
    if (_willRetain)
      flags |= 0x20;
  }
  if (_username.length() > 0)
    flags |= 0x80;
  if (_password.length() > 0)
    flags |= 0x40;
  BC_NEED(1);
  buf[pos++] = flags;

  BC_NEED(2);
  buf[pos++] = (_keepAlive >> 8) & 0xFF;
  buf[pos++] = _keepAlive & 0xFF;

  MQTTProperties connectProps;
  if (_sessionExpiry > 0)
    connectProps.setSessionExpiry(_sessionExpiry);
  if (_topicAliasMaximum > 0)
    connectProps.setTopicAliasMaximum(_topicAliasMaximum);

  if (extraProps) {
    for (uint8_t i = 0; i < extraProps->count(); ++i) {
      const MQTTProperties::Property &p = extraProps->propAt(i);
      if (p.id == USER_PROPERTY) {
        connectProps.addUserProperty(p.key, p.str);
      } else {

        switch (p.id) {
        case AUTHENTICATION_METHOD:
          connectProps.setAuthenticationMethod(p.str);
          break;
        case REQUEST_RESPONSE_INFORMATION: /* read-only / server sets */
          break;
        case REQUEST_PROBLEM_INFORMATION: /* read-only */
          break;
        default:
          break;
        }
      }
    }
  }

  uint8_t cpBuf[512];
  int cpLen = connectProps.serialize(cpBuf, sizeof(cpBuf));
  if (cpLen < 0)
    return -1;

  int encCp = encodeLength(&buf[pos], (uint32_t)cpLen);
  BC_NEED(encCp + cpLen);
  pos += encCp;
  if (cpLen > 0) {
    memcpy(&buf[pos], cpBuf, cpLen);
    pos += cpLen;
  }

  uint16_t cidLen = (uint16_t)_clientId.length();
  BC_NEED(2 + cidLen);
  buf[pos++] = (cidLen >> 8) & 0xFF;
  buf[pos++] = cidLen & 0xFF;
  memcpy(&buf[pos], _clientId.c_str(), cidLen);
  pos += cidLen;

  if (_hasWill) {

    uint8_t wpBuf[256];
    int wpLen = _willProps.serialize(wpBuf, sizeof(wpBuf));
    if (wpLen < 0)
      return -1;
    int encWp = encodeLength(&buf[pos], (uint32_t)wpLen);
    BC_NEED(encWp + wpLen);
    pos += encWp;
    if (wpLen > 0) {
      memcpy(&buf[pos], wpBuf, wpLen);
      pos += wpLen;
    }

    uint16_t wtLen = (uint16_t)_willTopic.length();
    BC_NEED(2 + wtLen);
    buf[pos++] = (wtLen >> 8) & 0xFF;
    buf[pos++] = wtLen & 0xFF;
    memcpy(&buf[pos], _willTopic.c_str(), wtLen);
    pos += wtLen;

    uint16_t wpayLen = (uint16_t)_willPayload.length();
    BC_NEED(2 + wpayLen);
    buf[pos++] = (wpayLen >> 8) & 0xFF;
    buf[pos++] = wpayLen & 0xFF;
    memcpy(&buf[pos], _willPayload.c_str(), wpayLen);
    pos += wpayLen;
  }

  if (_username.length() > 0) {
    uint16_t ul = (uint16_t)_username.length();
    BC_NEED(2 + ul);
    buf[pos++] = (ul >> 8) & 0xFF;
    buf[pos++] = ul & 0xFF;
    memcpy(&buf[pos], _username.c_str(), ul);
    pos += ul;
  }

  if (_password.length() > 0) {
    uint16_t pl = (uint16_t)_password.length();
    BC_NEED(2 + pl);
    buf[pos++] = (pl >> 8) & 0xFF;
    buf[pos++] = pl & 0xFF;
    memcpy(&buf[pos], _password.c_str(), pl);
    pos += pl;
  }

#undef BC_NEED
  return pos;
}

bool ESP32_MQTTv5::connect(const String &clientId) {
  return connect(clientId, "", "", nullptr);
}

bool ESP32_MQTTv5::connect(const String &clientId, const String &username,
                           const String &password) {
  return connect(clientId, username, password, nullptr);
}

bool ESP32_MQTTv5::connect(const String &clientId, const String &username,
                           const String &password, MQTTProperties *extraProps) {
  if (_state != STATE_DISCONNECTED)
    disconnect();

  _clientId = clientId;
  _username = username;
  _password = password;
  _state = STATE_CONNECTING;
  _lastReconnectAttempt = millis();

  if (!allocBuffer()) {
    _state = STATE_DISCONNECTED;
    notifyError(UNSPECIFIED_ERROR, "Buffer alloc failed");
    return false;
  }

  if (!connectTransport()) {
    _state = STATE_DISCONNECTED;
    notifyError(SERVER_UNAVAILABLE, "Transport failed");
    return false;
  }

  int bodyLen = buildConnect(_buffer + 5, _bufferSize - 5, extraProps);
  if (bodyLen < 0) {
    disconnect();
    notifyError(PACKET_TOO_LARGE, "CONNECT too large; call setBufferSize()");
    return false;
  }

  _buffer[0] = MQTT_CONNECT;
  int encLen = encodeLength(&_buffer[1], (uint32_t)bodyLen);

  int bodyOff = 1 + encLen;
  if (bodyOff != 5)
    memmove(&_buffer[bodyOff], &_buffer[5], bodyLen);
  int totalLen = bodyOff + bodyLen;

  if (sendPacket(_buffer, totalLen) != totalLen) {
    disconnect();
    notifyError(UNSPECIFIED_ERROR, "Send CONNECT failed");
    return false;
  }

  unsigned long timeout = millis() + 5000;
  while (millis() < timeout) {
    if (!_client->available()) {
      delay(10);
      continue;
    }

    int hdr = readByte();
    if (hdr < 0)
      continue;
    uint8_t ptype = (uint8_t)hdr & 0xF0;

    uint8_t lenBuf[4];
    int lenBytes = 0;
    do {
      int b = readByte();
      if (b < 0)
        break;
      lenBuf[lenBytes] = (uint8_t)b;
    } while ((lenBuf[lenBytes++] & 0x80) && lenBytes < 4);
    int remLen;
    decodeLength(lenBuf, remLen);

    if (remLen < 0 || remLen > _bufferSize) {

      for (int i = 0; i < remLen && i < 4096; ++i)
        readByte();
      continue;
    }
    if (remLen > 0)
      readBytes(_buffer, remLen);

    if (ptype == MQTT_AUTH) {
      MQTTReasonCode authReason =
          (remLen >= 1) ? (MQTTReasonCode)_buffer[0] : SUCCESS;
      MQTTProperties authProps;
      if (remLen > 1) {
        int pLen;
        int enc = decodeLength(&_buffer[1], pLen);
        if (pLen > 0)
          authProps.parse(&_buffer[1 + enc], pLen);
      }
      if (_authCallback)
        _authCallback(authReason, authProps);
      timeout = millis() + 5000;
      continue;
    }

    if (ptype != MQTT_CONNACK)
      continue;

    if (remLen < 2) {
      disconnect();
      notifyError(MALFORMED_PACKET, "Short CONNACK");
      return false;
    }

    uint8_t reasonCode = _buffer[1];
    _lastReasonCode = (MQTTReasonCode)reasonCode;

    if (remLen > 2) {
      int pLen;
      int enc = decodeLength(&_buffer[2], pLen);
      if (pLen > 0) {
        MQTTProperties props;
        props.parse(&_buffer[2 + enc], pLen);
        _lastReasonString = props.getReasonString();
        _serverTopicAliasMaximum = props.getUint16(TOPIC_ALIAS_MAXIMUM, 0);
        _serverMaxPacketSize = props.getUint32(MAXIMUM_PACKET_SIZE, 0);

        if (_serverMaxPacketSize > 0 && _serverMaxPacketSize > _bufferSize) {
          uint16_t newSz =
              (uint16_t)(_serverMaxPacketSize > 65535 ? 65535
                                                      : _serverMaxPacketSize);
          _desiredBufferSize = newSz;
          if (!allocBuffer()) {
            notifyError(UNSPECIFIED_ERROR,
                        "Auto buffer resize failed after CONNACK");
          }
        }

        if (_serverTopicAliasMaximum > MQTT_MAX_ALIASES) {
          notifyError(UNSPECIFIED_ERROR, "Server TopicAliasMaximum (" +
                                             String(_serverTopicAliasMaximum) +
                                             ") > MQTT_MAX_ALIASES (" +
                                             String(MQTT_MAX_ALIASES) +
                                             "). Clamped.");
          _serverTopicAliasMaximum = MQTT_MAX_ALIASES;
        }
      }
    }

    if (reasonCode == SUCCESS) {
      _state = STATE_CONNECTED;
      _lastInbound = _lastOutbound = millis();
      resetSession();
      if (_connectCallback)
        _connectCallback();
      return true;
    } else {
      char buf[40];
      if (_lastReasonString.length())
        snprintf(buf, sizeof(buf), "CONNACK 0x%02X: %.24s", reasonCode,
                 _lastReasonString.c_str());
      else
        snprintf(buf, sizeof(buf), "CONNACK 0x%02X", reasonCode);
      disconnect();
      notifyError(_lastReasonCode, String(buf));
      return false;
    }
  }

  disconnect();
  notifyError(SERVER_UNAVAILABLE, "CONNACK timeout");
  return false;
}

void ESP32_MQTTv5::disconnect(MQTTReasonCode reason) {
  if (_state == STATE_CONNECTED) {
    _state = STATE_DISCONNECTING;
    uint8_t disc[4] = {MQTT_DISCONNECT, 0x02, (uint8_t)reason, 0x00};
    sendPacket(disc, 4);
  }
  disconnectTransport();
  _state = STATE_DISCONNECTED;
  if (_disconnectCallback)
    _disconnectCallback(reason);
}

bool ESP32_MQTTv5::connected() {
  return _state == STATE_CONNECTED && _client && _client->connected();
}

void ESP32_MQTTv5::loop() {
  if (_state == STATE_CONNECTED) {

    if (!_client || !_client->connected()) {
      _state = STATE_DISCONNECTED;
      if (_disconnectCallback)
        _disconnectCallback(UNSPECIFIED_ERROR);
    } else {
      parseIncoming();
      retransmitQoS2();

      unsigned long now = millis();

      if (_keepAlive > 0 &&
          (now - _lastOutbound) > (unsigned long)_keepAlive * 1000UL) {
        uint8_t ping[2] = {MQTT_PINGREQ, 0x00};
        sendPacket(ping, 2);
      }

      if (_keepAlive > 0 &&
          (now - _lastInbound) > (unsigned long)_keepAlive * 1500UL) {
        disconnect(KEEP_ALIVE_TIMEOUT);
      }
    }
  }

  if (_autoReconnect && _state == STATE_DISCONNECTED && _client) {
    unsigned long now = millis();
    if ((now - _lastReconnectAttempt) >= _currentReconnectDelay) {
      _lastReconnectAttempt = now;
      if (connect(_clientId, _username, _password)) {
        _currentReconnectDelay = _reconnectInitialDelay;
      } else {

        _currentReconnectDelay =
            (_currentReconnectDelay * 2 > _reconnectMaxDelay)
                ? _reconnectMaxDelay
                : _currentReconnectDelay * 2;
      }
    }
  }
}

uint16_t ESP32_MQTTv5::findOrAssignOutAlias(const String &topic,
                                            bool &sendTopicName) {
  if (_serverTopicAliasMaximum == 0) {
    sendTopicName = true;
    return 0;
  }

  for (uint8_t i = 0; i < _outAliasCount; ++i) {
    if (_outAliases[i].topic == topic) {
      sendTopicName = false;
      return _outAliases[i].alias;
    }
  }

  if (_outAliasCount < MQTT_MAX_ALIASES &&
      _nextAliasId <= _serverTopicAliasMaximum) {
    uint16_t alias = _nextAliasId++;
    _outAliases[_outAliasCount].topic = topic;
    _outAliases[_outAliasCount].alias = alias;
    ++_outAliasCount;
    sendTopicName = true;
    return alias;
  }

  sendTopicName = true;
  return 0;
}

String ESP32_MQTTv5::resolveInAlias(uint16_t alias,
                                    const String &topicInPacket) {
  if (alias == 0 || alias > MQTT_MAX_ALIASES)
    return topicInPacket;
  uint8_t idx = alias - 1;
  if (topicInPacket.length() > 0) {
    _inAliasTopics[idx] = topicInPacket;
  }
  return (_inAliasTopics[idx].length() > 0) ? _inAliasTopics[idx]
                                            : topicInPacket;
}

bool ESP32_MQTTv5::addQoS2Out(uint16_t packetId, const uint8_t *packet,
                              int len) {
  if (_qos2OutCount >= MQTT_MAX_QOS2_OUT)
    return false;
  QoS2OutEntry &e = _qos2Out[_qos2OutCount++];
  e.packetId = packetId;
  e.lastTxMs = millis();
  e.pubrelSent = false;
  e.active = true;
  e.retryCount = 0;
  e.pubPacket = nullptr;
  e.pubPacketLen = 0;
  if (packet && len > 0) {
    e.pubPacket = (uint8_t *)malloc(len);
    if (e.pubPacket) {
      memcpy(e.pubPacket, packet, len);
      e.pubPacketLen = (uint16_t)len;
    }
  }
  return true;
}

void ESP32_MQTTv5::freeQoS2Out(uint8_t index) {
  if (_qos2Out[index].pubPacket) {
    free(_qos2Out[index].pubPacket);
    _qos2Out[index].pubPacket = nullptr;
  }
  _qos2Out[index] = _qos2Out[--_qos2OutCount];
}

void ESP32_MQTTv5::handlePubrec(uint16_t packetId) {
  for (uint8_t i = 0; i < _qos2OutCount; ++i) {
    if (_qos2Out[i].active && _qos2Out[i].packetId == packetId) {

      if (_qos2Out[i].pubPacket) {
        free(_qos2Out[i].pubPacket);
        _qos2Out[i].pubPacket = nullptr;
      }
      _qos2Out[i].retryCount = 0;
      uint8_t pubrel[4] = {(uint8_t)(MQTT_PUBREL | 0x02), 0x02,
                           (uint8_t)(packetId >> 8),
                           (uint8_t)(packetId & 0xFF)};
      sendPacket(pubrel, 4);
      _qos2Out[i].pubrelSent = true;
      _qos2Out[i].lastTxMs = millis();
      return;
    }
  }
}

void ESP32_MQTTv5::handlePubcomp(uint16_t packetId) {
  for (uint8_t i = 0; i < _qos2OutCount; ++i) {
    if (_qos2Out[i].active && _qos2Out[i].packetId == packetId) {
      freeQoS2Out(i);
      return;
    }
  }
}

void ESP32_MQTTv5::retransmitQoS2() {
  unsigned long now = millis();
  for (uint8_t i = 0; i < _qos2OutCount;) {
    QoS2OutEntry &e = _qos2Out[i];
    if (!e.active) {
      ++i;
      continue;
    }

    if ((now - e.lastTxMs) >= MQTT_QOS2_RETRY_MS) {
      if (e.pubrelSent) {

        uint8_t pubrel[4] = {(uint8_t)(MQTT_PUBREL | 0x02), 0x02,
                             (uint8_t)(e.packetId >> 8),
                             (uint8_t)(e.packetId & 0xFF)};
        sendPacket(pubrel, 4);
        e.lastTxMs = now;
        ++i;
      } else {

        if (e.pubPacket && e.pubPacketLen > 0) {
          if (e.retryCount < MQTT_QOS2_MAX_RETRIES) {
            e.pubPacket[0] |= 0x08;
            sendPacket(e.pubPacket, e.pubPacketLen);
            e.lastTxMs = now;
            e.retryCount++;
            ++i;
          } else {

            notifyError(UNSPECIFIED_ERROR,
                        "QoS2 packetId " + String(e.packetId) +
                            " abandoned after " +
                            String(MQTT_QOS2_MAX_RETRIES) + " retransmits");
            freeQoS2Out(i);
          }
        } else {
          ++i;
        }
      }
    } else {
      ++i;
    }
  }
}

bool ESP32_MQTTv5::isQoS2InFlight(uint16_t packetId) const {
  for (uint8_t i = 0; i < _qos2InCount; ++i)
    if (_qos2In[i] == packetId)
      return true;
  return false;
}

void ESP32_MQTTv5::addQoS2In(uint16_t packetId) {
  if (_qos2InCount < MQTT_MAX_QOS2_IN)
    _qos2In[_qos2InCount++] = packetId;
}

void ESP32_MQTTv5::removeQoS2In(uint16_t packetId) {
  for (uint8_t i = 0; i < _qos2InCount; ++i) {
    if (_qos2In[i] == packetId) {
      _qos2In[i] = _qos2In[--_qos2InCount];
      return;
    }
  }
}

bool ESP32_MQTTv5::parseIncoming() {
  if (!_client || !_buffer)
    return false;

  while (_client->available()) {
    int hdrByte = readByte();
    if (hdrByte < 0)
      break;
    uint8_t header = (uint8_t)hdrByte;
    uint8_t ptype = header & 0xF0;

    uint8_t lenBuf[4];
    int lenBytes = 0;
    unsigned long lenDeadline = millis() + _readTimeoutMs;
    do {
      int b = -1;
      while (millis() < lenDeadline && b < 0) {
        if (_client->available())
          b = _client->read();
        else
          delay(0);
      }
      if (b < 0) {
        notifyError(MALFORMED_PACKET, "Timeout reading packet length");
        return false;
      }
      lenBuf[lenBytes] = (uint8_t)b;
    } while ((lenBuf[lenBytes++] & 0x80) && lenBytes < 4);
    int remLen;
    decodeLength(lenBuf, remLen);

    if (remLen < 0) {
      notifyError(MALFORMED_PACKET, "Bad remaining length");
      return false;
    }

    if ((int)remLen > _bufferSize) {
      if (_allowDynamicBufferResize) {
        int newSize = remLen + 512;
        if (newSize > 65535)
          newSize = 65535;
        _desiredBufferSize = (uint16_t)newSize;
        if (!allocBuffer()) {
          int drain = remLen;
          while (drain > 0) {
            uint8_t tmp[64];
            int rd = (drain < (int)sizeof(tmp)) ? drain : (int)sizeof(tmp);
            readBytes(tmp, rd);
            drain -= rd;
          }
          notifyError(UNSPECIFIED_ERROR, "Dynamic buffer resize failed");
          return true;
        }

      } else {
        int drain = remLen;
        while (drain > 0) {
          int chunk = (drain < _bufferSize) ? drain : _bufferSize;
          readBytes(_buffer, chunk);
          drain -= chunk;
        }
        notifyError(PACKET_TOO_LARGE, "Incoming packet exceeds buffer");
        return true;
      }
    }

    if (remLen > 0) {
      int got = readBytes(_buffer, remLen);
      if (got != remLen) {
        notifyError(MALFORMED_PACKET, "Incomplete packet");
        return false;
      }
    }

    switch (ptype) {

    case MQTT_PUBLISH: {
      int pos = 0;
      if (pos + 2 > remLen)
        break;
      uint16_t topicLen = ((uint16_t)_buffer[pos] << 8) | _buffer[pos + 1];
      pos += 2;
      if (pos + topicLen > remLen) {
        notifyError(MALFORMED_PACKET, "Topic overflow");
        break;
      }
      String topic = String((const char *)&_buffer[pos], topicLen);
      pos += topicLen;

      uint8_t qos = (header >> 1) & 0x03;
      bool retain = (header & 0x01) != 0;
      uint16_t packetId = 0;
      if (qos > 0) {
        if (pos + 2 > remLen)
          break;
        packetId = ((uint16_t)_buffer[pos] << 8) | _buffer[pos + 1];
        pos += 2;
      }

      if (pos >= remLen)
        break;
      int propLen;
      int encProp = decodeLength(&_buffer[pos], propLen);
      pos += encProp;
      MQTTProperties props;
      if (propLen > 0) {
        props.parse(&_buffer[pos], propLen);
        pos += propLen;
      }

      uint16_t alias = props.getTopicAlias();
      topic = resolveInAlias(alias, topic);

      bool isDup = (qos == 2) && isQoS2InFlight(packetId);
      int payloadLen = remLen - pos;

      if (qos == 1) {
        uint8_t ack[4] = {MQTT_PUBACK, 0x02, (uint8_t)(packetId >> 8),
                          (uint8_t)(packetId & 0xFF)};
        sendPacket(ack, 4);
      } else if (qos == 2) {
        uint8_t pubrec[4] = {MQTT_PUBREC, 0x02, (uint8_t)(packetId >> 8),
                             (uint8_t)(packetId & 0xFF)};
        sendPacket(pubrec, 4);
        if (!isDup)
          addQoS2In(packetId);
      }

      if (!isDup) {
        if (_messageRawCallback) {
          _messageRawCallback(topic, &_buffer[pos], (size_t)payloadLen, qos,
                              retain);
        } else if (_messageCallback) {
          MQTTMessage msg;
          msg.topic = topic;
          msg.payload = String((const char *)&_buffer[pos], payloadLen);
          msg.qos = qos;
          msg.retain = retain;
          msg.hasTopicAlias = (alias > 0);
          msg.topicAlias = alias;
          msg.payloadFormatIndicator = props.getPayloadFormat();
          msg.hasPayloadFormatIndicator =
              (props.getUint8(PAYLOAD_FORMAT_INDICATOR, 0xFF) != 0xFF);
          msg.messageExpiry = props.getUint32(MESSAGE_EXPIRY_INTERVAL, 0);
          msg.contentType = props.getString(CONTENT_TYPE);
          msg.responseTopic = props.getString(RESPONSE_TOPIC);
          msg.correlationData = props.getString(CORRELATION_DATA);
          msg.subscriptionIdentifier = props.getSubscriptionIdentifier();
          msg.hasSubscriptionIdentifier = (msg.subscriptionIdentifier > 0);
          _messageCallback(msg);
        }
      }
      break;
    }

    case MQTT_PUBACK:
      break;

    case MQTT_PUBREC: {
      if (remLen >= 2) {
        uint16_t pid = ((uint16_t)_buffer[0] << 8) | _buffer[1];
        handlePubrec(pid);
      }
      break;
    }

    case MQTT_PUBREL: {
      if (remLen >= 2) {
        uint16_t pid = ((uint16_t)_buffer[0] << 8) | _buffer[1];
        removeQoS2In(pid);
        uint8_t pubcomp[4] = {MQTT_PUBCOMP, 0x02, (uint8_t)(pid >> 8),
                              (uint8_t)(pid & 0xFF)};
        sendPacket(pubcomp, 4);
      }
      break;
    }

    case MQTT_PUBCOMP: {
      if (remLen >= 2) {
        uint16_t pid = ((uint16_t)_buffer[0] << 8) | _buffer[1];
        handlePubcomp(pid);
      }
      break;
    }

    case MQTT_SUBACK:
    case MQTT_UNSUBACK:
    case MQTT_PINGRESP:
      break;

    case MQTT_AUTH: {
      MQTTReasonCode authReason =
          (remLen >= 1) ? (MQTTReasonCode)_buffer[0] : SUCCESS;
      MQTTProperties authProps;
      if (remLen > 1) {
        int pLen;
        int enc = decodeLength(&_buffer[1], pLen);
        if (pLen > 0)
          authProps.parse(&_buffer[1 + enc], pLen);
      }
      if (_authCallback)
        _authCallback(authReason, authProps);
      break;
    }

    case MQTT_DISCONNECT: {
      MQTTReasonCode rc =
          (remLen >= 1) ? (MQTTReasonCode)_buffer[0] : NORMAL_DISCONNECTION;
      _lastReasonCode = rc;
      if (remLen > 1) {
        int pLen;
        int enc = decodeLength(&_buffer[1], pLen);
        if (pLen > 0) {
          MQTTProperties dp;
          dp.parse(&_buffer[1 + enc], pLen);
          _lastReasonString = dp.getReasonString();
        }
      }
      disconnectTransport();
      _state = STATE_DISCONNECTED;
      if (_disconnectCallback)
        _disconnectCallback(rc);
      return true;
    }

    default:
      break;
    }
  }
  return true;
}

bool ESP32_MQTTv5::publish(const String &topic, const String &payload) {
  return publish(topic, payload, 0, false, nullptr);
}
bool ESP32_MQTTv5::publish(const String &topic, const String &payload,
                           uint8_t qos) {
  return publish(topic, payload, qos, false, nullptr);
}
bool ESP32_MQTTv5::publish(const String &topic, const String &payload,
                           MQTTProperties &props) {
  return publish(topic, payload, 0, false, &props);
}

bool ESP32_MQTTv5::publish(const String &topic, const String &payload,
                           uint8_t qos, bool retain, MQTTProperties *props) {
  if (!connected())
    return false;
  if (qos > 2)
    qos = 2;

  size_t estimatedMax = 5 + topic.length() + 2 + 256 + payload.length();
  if (estimatedMax > _bufferSize) {
    notifyError(PACKET_TOO_LARGE,
                "Payload too large for buffer; call setBufferSize()");
    return false;
  }

  bool sendTopicName = true;
  uint16_t useAlias = findOrAssignOutAlias(topic, sendTopicName);

  MQTTProperties pubProps;
  if (props) {
    for (uint8_t i = 0; i < props->count(); ++i) {
      const MQTTProperties::Property &p = props->propAt(i);
      switch (p.id) {
      case USER_PROPERTY:
        pubProps.addUserProperty(p.key, p.str);
        break;
      case MESSAGE_EXPIRY_INTERVAL:
        pubProps.setMessageExpiry(p.num.u32);
        break;
      case CONTENT_TYPE:
        pubProps.setContentType(p.str);
        break;
      case RESPONSE_TOPIC:
        pubProps.setResponseTopic(p.str);
        break;
      case CORRELATION_DATA:
        pubProps.setCorrelationData(p.str);
        break;
      case PAYLOAD_FORMAT_INDICATOR:
        pubProps.setPayloadFormat(p.num.u8);
        break;
      default:
        break;
      }
    }
  }
  if (useAlias > 0)
    pubProps.setTopicAlias(useAlias);

  uint8_t propBuf[256];
  int propLen = pubProps.serialize(propBuf, sizeof(propBuf));
  if (propLen < 0) {
    notifyError(PACKET_TOO_LARGE, "PUBLISH properties too large");
    return false;
  }

  uint8_t *body = _buffer + 5;
  int bodyMax = _bufferSize - 5;
  int bpos = 0;

#define PB_NEED(n)                                                             \
  do {                                                                         \
    if (bpos + (n) > bodyMax) {                                                \
      notifyError(PACKET_TOO_LARGE, "PUBLISH overflow");                       \
      return false;                                                            \
    }                                                                          \
  } while (0)

  const String &topicToSend = sendTopicName ? topic : String("");
  uint16_t tl = (uint16_t)topicToSend.length();
  PB_NEED(2 + tl);
  body[bpos++] = (tl >> 8) & 0xFF;
  body[bpos++] = tl & 0xFF;
  if (tl > 0) {
    memcpy(&body[bpos], topicToSend.c_str(), tl);
    bpos += tl;
  }

  uint16_t packetId = 0;
  if (qos > 0) {
    packetId = getNextPacketId();
    PB_NEED(2);
    body[bpos++] = (packetId >> 8) & 0xFF;
    body[bpos++] = packetId & 0xFF;
  }

  int encProp = encodeLength(&body[bpos], (uint32_t)propLen);
  PB_NEED(encProp + propLen);
  bpos += encProp;
  if (propLen > 0) {
    memcpy(&body[bpos], propBuf, propLen);
    bpos += propLen;
  }

  uint32_t pl = (uint32_t)payload.length();
  PB_NEED((int)pl);
  memcpy(&body[bpos], payload.c_str(), pl);
  bpos += (int)pl;

#undef PB_NEED

  _buffer[0] = MQTT_PUBLISH | ((qos & 0x03) << 1) | (retain ? 0x01 : 0x00);
  int encLen = encodeLength(&_buffer[1], (uint32_t)bpos);
  int bodyOff = 1 + encLen;
  if (bodyOff != 5)
    memmove(&_buffer[bodyOff], &_buffer[5], bpos);

  if (bpos < 0) {
    notifyError(UNSPECIFIED_ERROR, "PUBLISH body length underflow");
    return false;
  }
  uint32_t totalLen = (uint32_t)(bodyOff + bpos);

  if (_serverMaxPacketSize > 0 && totalLen > _serverMaxPacketSize) {
    notifyError(PACKET_TOO_LARGE, "Exceeds server maximum packet size");
    return false;
  }

  if (sendPacket(_buffer, (int)totalLen) != (int)totalLen)
    return false;

  if (qos == 2)
    addQoS2Out(packetId, _buffer, (int)totalLen);
  return true;
}

bool ESP32_MQTTv5::subscribe(const String &topic) {
  return subscribe(topic, 0, false, false, 0);
}
bool ESP32_MQTTv5::subscribe(const String &topic, uint8_t qos) {
  return subscribe(topic, qos, false, false, 0);
}

bool ESP32_MQTTv5::subscribe(const String &topic, uint8_t qos, bool noLocal,
                             bool retainAsPublished, uint8_t retainHandling,
                             uint32_t subId) {
  if (!connected())
    return false;
  if ((size_t)topic.length() > _bufferSize - 20)
    return false;

  uint8_t *body = _buffer + 5;
  int bodyMax = _bufferSize - 5;
  int bpos = 0;

  uint16_t packetId = getNextPacketId();
  body[bpos++] = (packetId >> 8) & 0xFF;
  body[bpos++] = packetId & 0xFF;

  if (subId > 0) {

    MQTTProperties subProps;
    subProps.setSubscriptionIdentifier(subId);
    uint8_t spBuf[8];
    int spLen = subProps.serialize(spBuf, sizeof(spBuf));
    if (spLen < 0 ||
        bpos + encodeLength(nullptr, (uint32_t)spLen) + spLen > bodyMax)
      return false;
    int encSp = encodeLength(&body[bpos], (uint32_t)spLen);
    bpos += encSp;
    memcpy(&body[bpos], spBuf, spLen);
    bpos += spLen;
  } else {
    body[bpos++] = 0x00;
  }

  uint16_t tl = (uint16_t)topic.length();
  if (bpos + 2 + tl + 1 > bodyMax)
    return false;
  body[bpos++] = (tl >> 8) & 0xFF;
  body[bpos++] = tl & 0xFF;
  memcpy(&body[bpos], topic.c_str(), tl);
  bpos += tl;

  uint8_t opts = (qos & 0x03);
  if (noLocal)
    opts |= 0x04;
  if (retainAsPublished)
    opts |= 0x08;
  opts |= ((retainHandling & 0x03) << 4);
  body[bpos++] = opts;

  _buffer[0] = MQTT_SUBSCRIBE;
  int encLen = encodeLength(&_buffer[1], (uint32_t)bpos);
  int bodyOff = 1 + encLen;
  if (bodyOff != 5)
    memmove(&_buffer[bodyOff], &_buffer[5], bpos);
  int totalLen = bodyOff + bpos;
  return sendPacket(_buffer, totalLen) == totalLen;
}

bool ESP32_MQTTv5::unsubscribe(const String &topic) {
  if (!connected())
    return false;
  if ((size_t)topic.length() > _bufferSize - 10)
    return false;

  uint8_t *body = _buffer + 5;
  int bodyMax = _bufferSize - 5;
  int bpos = 0;

  uint16_t packetId = getNextPacketId();
  body[bpos++] = (packetId >> 8) & 0xFF;
  body[bpos++] = packetId & 0xFF;

  body[bpos++] = 0x00;

  uint16_t tl = (uint16_t)topic.length();
  if (bpos + 2 + tl > bodyMax)
    return false;
  body[bpos++] = (tl >> 8) & 0xFF;
  body[bpos++] = tl & 0xFF;
  memcpy(&body[bpos], topic.c_str(), tl);
  bpos += tl;

  _buffer[0] = MQTT_UNSUBSCRIBE;
  int encLen = encodeLength(&_buffer[1], (uint32_t)bpos);
  int bodyOff = 1 + encLen;
  if (bodyOff != 5)
    memmove(&_buffer[bodyOff], &_buffer[5], bpos);
  int totalLen = bodyOff + bpos;
  return sendPacket(_buffer, totalLen) == totalLen;
}

int ESP32_MQTTv5::buildAuth(uint8_t *buf, int bufSize, MQTTReasonCode reason,
                            MQTTProperties *props) {
  if (!buf || bufSize < 4)
    return -1;

#define BA_NEED(pos, n)                                                        \
  do {                                                                         \
    if ((pos) + (n) > bufSize)                                                 \
      return -1;                                                               \
  } while (0)

  int propLen = 0;
  uint8_t *propBuf = nullptr;
  if (props && props->count() > 0) {
    propBuf = (uint8_t *)malloc(bufSize);
    if (!propBuf)
      return -1;
    propLen = props->serialize(propBuf, bufSize);
    if (propLen < 0) {
      free(propBuf);
      return -1;
    }
  }

  int encPropBytes = 1;
  if (propLen > 127)
    encPropBytes = 2;
  if (propLen > 16383)
    encPropBytes = 3;
  uint32_t remLenVal = (uint32_t)(1 + encPropBytes + propLen);

  uint8_t remLenBuf[4];
  int encRemLen = encodeLength(remLenBuf, remLenVal);

  int totalNeeded = 1 + encRemLen + 1 + encPropBytes + propLen;
  if (totalNeeded > bufSize) {
    if (propBuf)
      free(propBuf);
    return -1;
  }

  int pos = 0;
  buf[pos++] = MQTT_AUTH;
  memcpy(&buf[pos], remLenBuf, encRemLen);
  pos += encRemLen;
  buf[pos++] = (uint8_t)reason;
  pos += encodeLength(&buf[pos], (uint32_t)propLen);
  if (propLen > 0) {
    memcpy(&buf[pos], propBuf, propLen);
    pos += propLen;
  }

  if (propBuf)
    free(propBuf);

#undef BA_NEED
  return pos;
}

bool ESP32_MQTTv5::sendAuth(MQTTReasonCode reason, MQTTProperties &props) {
  if (!_buffer)
    return false;
  int len = buildAuth(_buffer, _bufferSize, reason, &props);
  if (len < 0)
    return false;
  return sendPacket(_buffer, len) == len;
}

void ESP32_MQTTv5::auth(MQTTReasonCode reason, MQTTProperties &props) {
  sendAuth(reason, props);
}
