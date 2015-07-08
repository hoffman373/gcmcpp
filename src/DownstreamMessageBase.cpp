#include "DownstreamMessageBase.hpp"
#include <iostream>

using namespace obr_basementcontrol;

DownstreamMessageBase::DownstreamMessageBase()
  : priority(-1), timeToLive(-1), contentAvailable(false),
    delayWhileIdle(false), deliveryReceiptRequested(false), dryRun(false)
{
}

void DownstreamMessageBase::parse(Json::Value& messageRoot) {
  to = messageRoot.get("to", "").asString();
  setMessageId(messageRoot.get("message_id", "").asString());
  collapseKey = messageRoot.get("collapse_key", "").asString();
  priority = messageRoot.get("priority", -1).asInt();
  contentAvailable = messageRoot.get("content_available", false).asBool();
  delayWhileIdle = messageRoot.get("delay_while_idle", false).asBool();
  timeToLive = messageRoot.get("time_to_live", -1).asInt();
  deliveryReceiptRequested = messageRoot.get("delivery_receipt_requested", false).asBool();
  dryRun = messageRoot.get("dry_run", false).asBool();
}

void DownstreamMessageBase::buildPayload(Json::Value& messageRoot)
{
  messageRoot["to"] = to;
  messageRoot["message_id"] = getMessageId();
  if(!collapseKey.empty()) {
    messageRoot["collapse_key"] = collapseKey;
  }

  if(priority > -1) {
    messageRoot["priority"] = priority;
  }
      
  if(contentAvailable) {    
    messageRoot["content_available"] = true;
  }

  if(delayWhileIdle) {
    messageRoot["delay_while_idle"] = true;
  }

  if(timeToLive > -1) {
    messageRoot["time_to_live"] = timeToLive;
  }

  if(deliveryReceiptRequested) {
    messageRoot["delivery_receipt_requested"] = true;
  }

  if(dryRun) {
    messageRoot["dry_run"] = true;
  }
}

void DownstreamMessageBase::clone(DownstreamMessageBase* dest)
{
  dest->setTo(getTo());
  dest->setMessageId(getMessageId());
  dest->setCollapseKey(getCollapseKey());
  dest->setPriority(getPriority());
  dest->setContentAvailable(getContentAvailable());
  dest->setDelayWhileIdle(getDelayWhileIdle());
  dest->setTimeToLive(getTimeToLive());
  dest->setDeliveryReceiptRequested(getDeliveryReceiptRequested());
  dest->setDryRun(getDryRun());
}

const string& DownstreamMessageBase::getTo()
{
  return to;
}

void DownstreamMessageBase::setTo(const string& value)
{
  to = value;
}

const string& DownstreamMessageBase::getCollapseKey()
{
  return collapseKey;
}

void DownstreamMessageBase::setCollapseKey(const string& value)
{
  collapseKey = value;
}

int DownstreamMessageBase::getPriority()
{
  return priority;
}

void DownstreamMessageBase::setPriority(const int value)
{
  priority = value;
}

bool DownstreamMessageBase::getContentAvailable()
{
  return contentAvailable;
}

void DownstreamMessageBase::setContentAvailable(bool value)
{
  contentAvailable = value;
}

bool DownstreamMessageBase::getDelayWhileIdle()
{
  return delayWhileIdle;
}

void DownstreamMessageBase::setDelayWhileIdle(bool value)
{
  delayWhileIdle = value;
}

int DownstreamMessageBase::getTimeToLive()
{
  return timeToLive;
}

void DownstreamMessageBase::setTimeToLive(const int value)
{
  timeToLive = value;
}

bool DownstreamMessageBase::getDeliveryReceiptRequested()
{
  return deliveryReceiptRequested;
}

void DownstreamMessageBase::setDeliveryReceiptRequested(bool value)
{
  deliveryReceiptRequested = value;
}

bool DownstreamMessageBase::getDryRun()
{
  return dryRun;
}

void DownstreamMessageBase::setDryRun(bool value)
{
  dryRun = value;
}

