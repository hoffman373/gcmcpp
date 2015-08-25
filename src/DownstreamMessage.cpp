#include "DownstreamMessage.hpp"
#include <iostream>

using namespace obr_basementcontrol;

DownstreamMessage::DownstreamMessage()
  : DownstreamMessageBase()
{
}

GcmStanzaMsgType DownstreamMessage::getMessageType() {
  return GcmStanzaMsgType::DOWNSTREAM;
}

void DownstreamMessage::parse(Json::Value& messageRoot) {
  DownstreamMessageBase::parse(messageRoot);
  data = messageRoot["data"];
}

string DownstreamMessage::buildPayload() {
  Json::Value messageRoot;

  DownstreamMessageBase::buildPayload(messageRoot);

  messageRoot["data"] = data;
  
  ostringstream output;
  output << messageRoot;
  output.flush();
  return output.str();
}

MessageBase* DownstreamMessage::clone()
{
  DownstreamMessage* returnValue = new DownstreamMessage();
  DownstreamMessageBase::clone(returnValue);
  returnValue->data = data;
  return returnValue;
}

const Json::Value& DownstreamMessage::getData()
{
  return data;
}

void DownstreamMessage::setData(const Json::Value& value)
{
  data = value;
}

void DownstreamMessage::accept(IGcmMessageVisitor *visitor)
{
  return visitor->visit(*this);
}
