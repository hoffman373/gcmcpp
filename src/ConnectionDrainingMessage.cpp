#include "ConnectionDrainingMessage.hpp"
#include <iostream>

using namespace obr_basementcontrol;

GcmStanzaMsgType ConnectionDrainingMessage::getMessageType() {
  return GcmStanzaMsgType::CONNECTION_DRAINING;
}

void ConnectionDrainingMessage::parse(Json::Value& messageRoot) {
}

string ConnectionDrainingMessage::buildPayload() {
  Json::Value messageRoot;
  messageRoot["message_type"] = "control";  
  messageRoot["control_type"] = "CONNECTION_DRAINING";
  
  ostringstream output;
  output << messageRoot;
  output.flush();
  return output.str();
}

MessageBase* ConnectionDrainingMessage::clone()
{
  ConnectionDrainingMessage* returnValue = new ConnectionDrainingMessage();
  return returnValue;
}

void ConnectionDrainingMessage::dumpToStream(ostream& stream) const
{
  stream << "Here";
}

void ConnectionDrainingMessage::dumpToStream(ostream& stream)
{
  stream << "Here";
}

void ConnectionDrainingMessage::accept(IGcmMessageVisitor *visitor)
{
  return visitor->visit(*this);
}
