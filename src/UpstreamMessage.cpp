#include "UpstreamMessage.hpp"
#include <iostream>

using namespace obr_basementcontrol;

GcmStanzaMsgType UpstreamMessage::getMessageType() {
  return GcmStanzaMsgType::UPSTREAM;
}

void UpstreamMessage::parse(Json::Value& messageRoot) {
  from = messageRoot.get("from", "").asString();
  setMessageId(messageRoot.get("message_id", "").asString());
  category = messageRoot.get("category", "").asString();

  data = messageRoot["data"];  
}

string UpstreamMessage::buildPayload() {
  Json::Value messageRoot;
  messageRoot["message_type"] = "ack";
  messageRoot["from"] = from;
  messageRoot["message_id"] = getMessageId();
  messageRoot["category"] = category;
  messageRoot["data"] = data;
  
  ostringstream output;
  output << messageRoot;
  output.flush();
  return output.str();
}

const string& UpstreamMessage::getFrom()
{
  return from;
}

void UpstreamMessage::setFrom(const string& value)
{
  from = value;
}

MessageBase* UpstreamMessage::clone()
{
  UpstreamMessage* returnValue = new UpstreamMessage();
  returnValue->from = from;
  returnValue->setMessageId(getMessageId());
  returnValue->data = data;
  return returnValue;
}

const string& UpstreamMessage::getCategory()
{
  return category;
}

void UpstreamMessage::setCategory(const string& value)
{
  category = value;
}


const Json::Value& UpstreamMessage::getData()
{
  return data;
}

void UpstreamMessage::setData(const Json::Value& value)
{
  data = value;
}
