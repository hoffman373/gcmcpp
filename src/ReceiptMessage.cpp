#include "ReceiptMessage.hpp"
#include <iostream>

using namespace obr_basementcontrol;

GcmStanzaMsgType ReceiptMessage::getMessageType() {
  return GcmStanzaMsgType::ACK;
}

void ReceiptMessage::parse(Json::Value& messageRoot) {
  from = messageRoot.get("from", "").asString();
  messageId = messageRoot.get("message_id", "").asString();
  category = messageRoot.get("category", "").asString();
  Json::Value& data = messageRoot["data"];
  messageStatus = data.get("message_status", "").asString();
  originalMessageId = data.get("original_message_id", "").asString();
  deviceRegistrationId = data.get("device_registration_id", "").asString();
}

string ReceiptMessage::buildPayload() {
  Json::Value messageRoot;
  messageRoot["message_type"] = "receipt";
  messageRoot["from"] = from;
  messageRoot["message_id"] = messageId;
  if(!messageStatus.empty()) {
    messageRoot["data"]["message_status"] = messageStatus;
  }

  if(!originalMessageId.empty()) {
    messageRoot["data"]["original_message_id"] = originalMessageId;
  }

  if(!deviceRegistrationId.empty()) {
    messageRoot["data"]["device_registration_id"] = deviceRegistrationId;
  }
  
  ostringstream output;
  output << messageRoot;
  output.flush();
  return output.str();
}

const string& ReceiptMessage::getFrom()
{
  return from;
}

void ReceiptMessage::setFrom(const string& value)
{
  from = value;
}

const string& ReceiptMessage::getMessageId()
{
  return messageId;
}

void ReceiptMessage::setMessageId(const string& value)
{
  messageId = value;
}

MessageBase* ReceiptMessage::clone()
{
  ReceiptMessage* returnValue = new ReceiptMessage(*this);
  // returnValue->from = from;
  // returnValue->messageId = messageId;
  // returnValue->category = category;
  // returnValue->messageStatus = messageStatus;
  // returnValue->originalMessageId = originalMessageId;
  // returnValue->deviceRegistrationId = deviceRegistrationId;
  return returnValue;
}

const string& ReceiptMessage::getCategory()
{
  return category;
}

void ReceiptMessage::setCategory(const string& value)
{
  category = value;
}

const string& ReceiptMessage::getMessageStatus()
{
  return messageStatus;
}

void ReceiptMessage::setMessageStatus(const string& value)
{
  messageStatus = value;
}

const string& ReceiptMessage::getOriginalMessageId()
{
  return originalMessageId;
}

void ReceiptMessage::setOriginalMessageId(const string& value)
{
  originalMessageId = value;
}

const string& ReceiptMessage::getDeviceRegistrationId()
{
  return deviceRegistrationId;
}

void ReceiptMessage::setDeviceRegistrationId(const string& value)
{
  deviceRegistrationId = value;
}

void ReceiptMessage::accept(IGcmMessageVisitor *visitor)
{
  return visitor->visit(*this);
}
