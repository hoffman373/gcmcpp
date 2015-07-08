#include "NotificationMessage.hpp"
#include <iostream>
#include <string>

using namespace std;

using namespace obr_basementcontrol;

NotificationMessage::NotificationMessage()
  : DownstreamMessageBase()
{
}

GcmStanzaMsgType NotificationMessage::getMessageType() {
  return GcmStanzaMsgType::NOTIFICATION;
}

void NotificationMessage::parse(Json::Value& messageRoot) {
  DownstreamMessageBase::parse(messageRoot);

  Json::Value& notification = messageRoot["notification"];  
  title = notification.get("title", "").asString();
  body = notification.get("body", "").asString();
  icon = notification.get("icon", "").asString();
  sound = notification.get("sound", "").asString();
  badge = notification.get("badge", "").asString();
  tag = notification.get("tag", "").asString();
  color = notification.get("color", "").asString();
  clickAction = notification.get("click_action", "").asString();
  bodyLocKey = notification.get("body_loc_key", "").asString();
  bodyLocArgs = notification.get("body_loc_args", "").asString();
  titleLocArgs = notification.get("title_loc_args", "").asString();
  titleLocKey = notification.get("title_loc_key", "").asString();
}

string NotificationMessage::buildPayload() {
  Json::Value messageRoot;
  
  DownstreamMessageBase::buildPayload(messageRoot);
  
  Json::Value& notification = messageRoot["notification"];  
  
  if(!title.empty()) {    
    notification["title"] = title;
  }
  
  if(!body.empty()) {
    notification["body"] = body;
  }
  
  if(!icon.empty()) {
    notification["icon"] = icon;
  }
 
  if(!sound.empty()) {
    notification["sound"] = sound;
  }
 
  if(!badge.empty()) {
    notification["badge"] = badge;
  }
 
  if(!tag.empty()) {
    notification["tag"] = tag;
  }
 
  if(!color.empty()) {
    notification["color"] = color;
  }
 
  if(!clickAction.empty()) {
    notification["click_action"] = clickAction;
  }
 
  if(!bodyLocKey.empty()) {
    notification["body_loc_key"] = bodyLocKey;
  }
 
  if(!bodyLocArgs.empty()) {
    notification["body_loc_args"] = bodyLocArgs;
  }
 
  if(!titleLocArgs.empty()) {
    notification["title_loc_args"] = titleLocArgs;
  }
 
  if(!titleLocKey.empty()) {
    notification["title_loc_key"] = titleLocKey;
  }
  
  ostringstream output;
  output << messageRoot;
  output.flush();
  return output.str();
}

MessageBase* NotificationMessage::clone()
{
  NotificationMessage* returnValue = new NotificationMessage();
  DownstreamMessageBase::clone((DownstreamMessageBase*)returnValue);
  returnValue->setTitle(getTitle());
  returnValue->setBody(getBody());
  returnValue->setIcon(getIcon());
  returnValue->setSound(getSound());
  returnValue->setBadge(getBadge());
  returnValue->setTag(getTag());
  returnValue->setColor(getColor());
  returnValue->setClickAction(getClickAction());
  returnValue->setBodyLocKey(getBodyLocKey());
  returnValue->setBodyLocArgs(getBodyLocArgs());
  returnValue->setTitleLocArgs(getTitleLocArgs());
  returnValue->setTitleLocKey(getTitleLocKey());
  return returnValue;
}

const string& NotificationMessage::getTitle()
{
  return title;
}

void NotificationMessage::setTitle(const string& value)
{
  title = value;
}

const string& NotificationMessage::getBody()
{
  return body;
}

void NotificationMessage::setBody(const string& value)
{
  body = value;
}

const string& NotificationMessage::getIcon()
{
  return icon;
}

void NotificationMessage::setIcon(const string& value)
{
  icon = value;
}

const string& NotificationMessage::getSound()
{
  return sound;
}

void NotificationMessage::setSound(const string& value)
{
  sound = value;
}

const string& NotificationMessage::getBadge()
{
  return badge;
}

void NotificationMessage::setBadge(const string& value)
{
  badge = value;
}

const string& NotificationMessage::getTag()
{
  return tag;
}

void NotificationMessage::setTag(const string& value)
{
  tag = value;
}

const string& NotificationMessage::getColor()
{
  return color;
}

void NotificationMessage::setColor(const string& value)
{
  color = value;
}

const string& NotificationMessage::getClickAction()
{
  return clickAction;
}

void NotificationMessage::setClickAction(const string& value)
{
  clickAction = value;
}

const string& NotificationMessage::getBodyLocKey()
{
  return bodyLocKey;
}

void NotificationMessage::setBodyLocKey(const string& value)
{
  bodyLocKey = value;
}

const string& NotificationMessage::getBodyLocArgs()
{
  return bodyLocArgs;
}

void NotificationMessage::setBodyLocArgs(const string& value)
{
  bodyLocArgs = value;
}

const string& NotificationMessage::getTitleLocArgs()
{
  return titleLocArgs;
}

void NotificationMessage::setTitleLocArgs(const string& value)
{
  titleLocArgs = value;
}

const string& NotificationMessage::getTitleLocKey()
{
  return titleLocKey;
}

void NotificationMessage::setTitleLocKey(const string& value)
{
  titleLocKey = value;
}
