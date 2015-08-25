#include "NackMessage.hpp"
#include <iostream>

using namespace obr_basementcontrol;

GcmStanzaMsgType NackMessage::getMessageType() {
  return GcmStanzaMsgType::NACK;
}

void NackMessage::parse(Json::Value& messageRoot) {
  from = messageRoot.get("from", "").asString();
  setMessageId(messageRoot.get("message_id", "").asString());
  string errorStr = messageRoot.get("error", "EMPTY").asString();
  if(errorStr == "INVALID_JSON"){
    error = GcmStanzaErrorType::INVALID_JSON;	
  } else if(errorStr == "BAD_REGISTRATION") {
    error = GcmStanzaErrorType::BAD_REGISTRATION;
  } else if(errorStr == "DEVICE_UNREGISTERED") {
    error = GcmStanzaErrorType::DEVICE_UNREGISTERED;
  } else if(errorStr == "BAD_ACK") {
    error = GcmStanzaErrorType::BAD_ACK;
  } else if(errorStr == "SERVICE_UNAVAILABLE") {
    error = GcmStanzaErrorType::SERVICE_UNAVAILABLE;
  } else if(errorStr == "INTERNAL_SERVER") {
    error = GcmStanzaErrorType::INTERNAL_SERVER_ERROR;
  } else if(errorStr == "DEVICE_MESSAGE_RATE_EXCEEDED") {
    error = GcmStanzaErrorType::DEVICE_MESSAGE_RATE_EXCEEDED;
  } else if(errorStr == "TOPICS_MESSAGE_RATE_EXCEEDED") {
    error = GcmStanzaErrorType::TOPICS_MESSAGE_RATE_EXCEEDED;
  } else if(errorStr == "CONNECTION_DRAINING") {
    error = GcmStanzaErrorType::CONNECTION_DRAINING;
  } else {
    error = GcmStanzaErrorType::INVALID_JSON;
  }
      
  errorDescription = messageRoot.get("error_description", "").asString();
}

string NackMessage::buildPayload() {
  Json::Value messageRoot;
  messageRoot["message_type"] = "nack";
  messageRoot["from"] = from;
  messageRoot["message_id"] = getMessageId();
  messageRoot["error"] = getGcmStanzaErrorTypeName(error);
  if(!errorDescription.empty()) {
    messageRoot["error_description"] = errorDescription;
  }
  
  ostringstream output;
  output << messageRoot;
  output.flush();
  return output.str();
}

const string& NackMessage::getFrom()
{
  return from;
}

void NackMessage::setFrom(const string& value)
{
  from = value;
}

void NackMessage::setError(GcmStanzaErrorType value)
{
  error = value;
}

void NackMessage::setErrorDescription(string value)
{
  errorDescription = value;
}

const string& NackMessage::getErrorDescription()
{
  return errorDescription;
}

const GcmStanzaErrorType NackMessage::getError()
{
  return error;
}

MessageBase* NackMessage::clone()
{
  NackMessage* returnValue = new NackMessage();
  returnValue->from = from;
  returnValue->setMessageId(getMessageId());
  returnValue->error = error;
  returnValue->errorDescription = errorDescription;
  return returnValue;
}

void NackMessage::accept(IGcmMessageVisitor *visitor)
{
  return visitor->visit(*this);
}
