#ifndef GCM_RECEIPT_MESSAGE_HPP
#define GCM_RECEIPT_MESSAGE_HPP

#include <string>
#include "GcmMessageBase.hpp"

using namespace std;

namespace obr_basementcontrol
{
  class ReceiptMessage : public MessageBase {
  public:
    virtual GcmStanzaMsgType getMessageType();

    virtual void parse(Json::Value& messageRoot);

    virtual string buildPayload();

    virtual MessageBase* clone();

    const string& getFrom();

    void setFrom(const string& value);

    const string& getMessageId();

    void setMessageId(const string& value);

    const string& getCategory();

    void setCategory(const string& value);

    const string& getMessageStatus();

    void setMessageStatus(const string& value);

    const string& getOriginalMessageId();

    void setOriginalMessageId(const string& value);

    const string& getDeviceRegistrationId();

    void setDeviceRegistrationId(const string& value);

    virtual void accept(IGcmMessageVisitor *visitor);

  private:
    string from;
    string messageId;
    string category;
    string messageStatus;
    string originalMessageId;
    string deviceRegistrationId;
  };
}

#endif
