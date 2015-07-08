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

  Json::Value& dataVal = messageRoot["data"];
  ostringstream output;
  output << dataVal;
  output.flush();
  data = output.str();
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


const string& UpstreamMessage::getData()
{
  return data;
}

void UpstreamMessage::setData(const string& value)
{
  data = value;
}
