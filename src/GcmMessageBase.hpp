#ifndef GCM_MESSAGE_BASE_HPP
#define GCM_MESSAGE_BASE_HPP

#include <string>
#include "json/json.h"
#include <ostream>

using namespace std;

namespace obr_basementcontrol
{
  enum class GcmStanzaMsgType {ACK, NACK, CONNECTION_DRAINING, RECEIPT, UPSTREAM, NOTIFICATION, DOWNSTREAM};

  enum class GcmStanzaErrorType {INVALID_JSON, BAD_REGISTRATION, DEVICE_UNREGISTERED,
			   BAD_ACK, SERVICE_UNAVAILABLE,
			   INTERNAL_SERVER_ERROR, DEVICE_MESSAGE_RATE_EXCEEDED, TOPICS_MESSAGE_RATE_EXCEEDED,
			   CONNECTION_DRAINING};

  const char* getGcmStanzaMsgTypeName(GcmStanzaMsgType toConvert);

  const char* getGcmStanzaErrorTypeName(GcmStanzaErrorType toConvert);

  class MessageBase {
  public:
    virtual GcmStanzaMsgType getMessageType() = 0;

    virtual void parse(Json::Value& messageRoot) = 0;

    virtual string buildPayload() = 0;

    virtual MessageBase* clone() = 0;     
  };
}

#endif
