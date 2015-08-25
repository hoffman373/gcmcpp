#ifndef GCM_ACK_MESSAGE_HPP
#define GCM_ACK_MESSAGE_HPP

#include <string>
#include "IdMessageBase.hpp"

using namespace std;

namespace obr_basementcontrol
{
  class AckMessage : public IdMessageBase {
  public:
    virtual GcmStanzaMsgType getMessageType();

    virtual void parse(Json::Value& messageRoot);

    virtual string buildPayload();

    virtual MessageBase* clone();
      
    const string& getFrom();

    void setFrom(const string& value);

    const string& getTo();

    void setTo(const string& value);

    virtual void accept(IGcmMessageVisitor *visitor);

  private:
    string from;
    string to;
  };
}

#endif
