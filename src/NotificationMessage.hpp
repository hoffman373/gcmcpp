#ifndef GCM_NOTIFIATION_MESSAGE_HPP
#define GCM_NOTIFIATION_MESSAGE_HPP

#include <string>
#include "GcmMessageBase.hpp"
#include "IdMessageBase.hpp"
#include "DownstreamMessageBase.hpp"

using namespace std;

namespace obr_basementcontrol
{
  class NotificationMessage : public DownstreamMessageBase {
  public:
    NotificationMessage();
    
    virtual GcmStanzaMsgType getMessageType();

    virtual void parse(Json::Value& messageRoot);

    virtual string buildPayload();

    virtual MessageBase* clone();
    
    const string& getTitle();

    void setTitle(const string& value);

    const string& getBody();

    void setBody(const string& value);

    const string& getIcon();

    void setIcon(const string& value);

    const string& getSound();

    void setSound(const string& value);

    const string& getBadge();

    void setBadge(const string& value);

    const string& getTag();

    void setTag(const string& value);

    const string& getColor();

    void setColor(const string& value);

    const string& getClickAction();

    void setClickAction(const string& value);

    const string& getBodyLocKey();

    void setBodyLocKey(const string& value);

    const string& getBodyLocArgs();

    void setBodyLocArgs(const string& value);

    const string& getTitleLocArgs();

    void setTitleLocArgs(const string& value);

    const string& getTitleLocKey();

    void setTitleLocKey(const string& value);

  private:
    string title;
    string body;
    string icon;
    string sound;
    string badge;
    string tag;
    string color;
    string clickAction;
    string bodyLocKey;
    string bodyLocArgs;
    string titleLocArgs;
    string titleLocKey;
  };
}

#endif
