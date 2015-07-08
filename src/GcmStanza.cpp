#include "GcmStanza.hpp"
#include "json/json.h"
#include <iostream>
#include "AckMessage.hpp"
#include "NackMessage.hpp"
#include "ReceiptMessage.hpp"
#include "ConnectionDrainingMessage.hpp"
#include "UpstreamMessage.hpp"
#include "NotificationMessage.hpp"
#include "DownstreamMessage.hpp"

using namespace obr_basementcontrol;

GcmStanza::~GcmStanza()
{
  delete(message);
}

GcmStanza::GcmStanza(const GcmStanza& source)
  : StanzaExtension(ExtUser + 100), filter(source.filter)
{
  message = source.message->clone();
}

GcmStanza::GcmStanza(GcmStanza&& source)
  : StanzaExtension(ExtUser + 100), filter(std::move(source.filter))
{
  message = source.message;
  source.message = nullptr;
}

GcmStanza::GcmStanza(MessageBase* msgBase)
  : StanzaExtension(ExtUser + 100)
{
  message = msgBase;
}

GcmStanza& GcmStanza::operator=(const GcmStanza& source)
{
  delete(message);
  message = source.message->clone();
  filter = source.filter;
}
    
GcmStanza& GcmStanza::operator=(GcmStanza&& source)
{
  delete(message);
  message = source.message;
  filter = std::move(source.filter);
}

const string& GcmStanza::filterString() const
{
  return filter;
}

StanzaExtension* GcmStanza::newInstance(const Tag* tag) const
{
  return new GcmStanza(tag);
}

Tag* GcmStanza::tag() const
{
  Tag* returnValue = new Tag("gcm", message->buildPayload());
  returnValue->setXmlns("google:mobile:data");
  return returnValue;
}

StanzaExtension*  GcmStanza::clone() const
{
  return new GcmStanza(*this);
}

GcmStanza::GcmStanza(const Tag* tag) : StanzaExtension(ExtUser + 100)
{
  //filter = "/message/gcm[@xmlns='google:mobile:data']";
  filter = "/message/gcm";
  if(tag == 0)
    return;
  Json::Value messageRoot;
  istringstream input(tag->cdata());
  input >> messageRoot;
  if(messageRoot.isMember("message_type")) {    
     string messageType = messageRoot.get("message_type", "EMPTY").asString();    
     if(messageType == "ack") {
       message = new AckMessage();       
     } else if (messageType == "nack") {
       message = new NackMessage();
     } else if(messageType == "receipt") {
       message = new ReceiptMessage();
     } else if (messageType == "control") {
       message = new ConnectionDrainingMessage();
     } else {
       throw ("Unknown messageType " + messageType);
     }

     message->parse(messageRoot);
  } else if(messageRoot.isMember("category")) {
    message = new UpstreamMessage();
    message->parse(messageRoot);
  } else if (messageRoot.isMember("notification")) {
    message = new NotificationMessage();
    message->parse(messageRoot);
  } else if (messageRoot.isMember("to")){
    message = new DownstreamMessage();
    message->parse(messageRoot);
  }
}

const char* obr_basementcontrol::getGcmStanzaMsgTypeName(GcmStanzaMsgType toConvert)
  {
    switch(toConvert)
      {
      case GcmStanzaMsgType::ACK:
	return "ACK";
      case GcmStanzaMsgType::NACK:
	return "NACK";
      case GcmStanzaMsgType::CONNECTION_DRAINING:
	return "CONNECTION_DRAINING";
      case GcmStanzaMsgType::RECEIPT:
	return "RECEIPT";
      case GcmStanzaMsgType::DOWNSTREAM:
	return "DOWNSTREAM";
      case GcmStanzaMsgType::UPSTREAM:
	return "UPSTREAM";
      case GcmStanzaMsgType::NOTIFICATION:
	return "NOTIFICATION";
      default:
	return "UNKNOWN";
      }
  }

const char* obr_basementcontrol::getGcmStanzaErrorTypeName(GcmStanzaErrorType toConvert)
  {
    switch(toConvert)
      {
      case GcmStanzaErrorType::INVALID_JSON:
	return "INVALID_JSON";
      case GcmStanzaErrorType::BAD_REGISTRATION:
	return "BAD_REGISTRATION";
      case GcmStanzaErrorType::DEVICE_UNREGISTERED:
	return "DEVICE_UNREGISTERED";
      case GcmStanzaErrorType::BAD_ACK:
	return "BAD_ACK";
      case GcmStanzaErrorType::SERVICE_UNAVAILABLE:
	return "SERVICE_UNAVAILABLE";
      case GcmStanzaErrorType::INTERNAL_SERVER_ERROR:
	return "INTERNAL_SERVER_ERROR";
      case GcmStanzaErrorType::DEVICE_MESSAGE_RATE_EXCEEDED:
	return "DEVICE_MESSAGE_RATE_EXCEEDED";
      case GcmStanzaErrorType::TOPICS_MESSAGE_RATE_EXCEEDED:
	return "TOPICS_MESSAGE_RATE_EXCEEDED";
      case GcmStanzaErrorType::CONNECTION_DRAINING:
	return "CONNECTION_DRAINING";
      default:
	return "UNKNOWN";
      }
  }

MessageBase* GcmStanza::pilferMesage()  
{
  MessageBase* returnValue = message;
  message = nullptr;
  return returnValue;
}
