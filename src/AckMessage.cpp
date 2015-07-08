#include "AckMessage.hpp"
#include <iostream>

using namespace obr_basementcontrol;

GcmStanzaMsgType AckMessage::getMessageType() {
  return GcmStanzaMsgType::ACK;
}

void AckMessage::parse(Json::Value& messageRoot) {
  from = messageRoot.get("from", "").asString();
  to = messageRoot.get("to", "").asString();
  setMessageId(messageRoot.get("message_id", "").asString());
}

string AckMessage::buildPayload() {
  Json::Value messageRoot;
  messageRoot["message_type"] = "ack";
  if(!from.empty()) {
    messageRoot["from"] = from;
  } else {
    messageRoot["to"] = to;
  }
  
  messageRoot["message_id"] = getMessageId();
  
  ostringstream output;
  output << messageRoot;
  output.flush();
  return output.str();
}

const string& AckMessage::getFrom()
{
  return from;
}

void AckMessage::setFrom(const string& value)
{
  from = value;
}

MessageBase* AckMessage::clone()
{
  AckMessage* returnValue = new AckMessage();
  returnValue->from = from;
  returnValue->to = to;
  returnValue->setMessageId(getMessageId());
  return returnValue;
}

const string& AckMessage::getTo()
{
  return to;
}

void AckMessage::setTo(const string& value)
{
  to = value;
}
