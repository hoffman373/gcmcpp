#include "IdMessageBase.hpp"

using namespace obr_basementcontrol;

const string& IdMessageBase::getMessageId()
{
  return messageId;
}

void IdMessageBase::setMessageId(const string& value)
{
  messageId = value;
}
