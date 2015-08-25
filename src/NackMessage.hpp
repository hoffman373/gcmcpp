#ifndef GCM_NACK_MESSAGE_HPP
#define GCM_NACK_MESSAGE_HPP

#include <string>
#include "IdMessageBase.hpp"

using namespace std;

namespace obr_basementcontrol
{
  class NackMessage : public IdMessageBase {
  public:
    virtual GcmStanzaMsgType getMessageType();

    virtual void parse(Json::Value& messageRoot);

    virtual string buildPayload();

    virtual MessageBase* clone();

    const string& getFrom();

    void setFrom(const string& value);

    void setError(GcmStanzaErrorType value);

    void setErrorDescription(string value);

    const string& getErrorDescription();

    const GcmStanzaErrorType getError();

    virtual void accept(IGcmMessageVisitor *visitor);

  private:
    string from;
    GcmStanzaErrorType error;
    string errorDescription;
  };
}

#endif
