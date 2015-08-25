#ifndef GCM_UPSTREAM_MESSAGE_MESSAGE_HPP
#define GCM_UPSTREAM_MESSAGE_MESSAGE_HPP

#include <string>
#include "GcmMessageBase.hpp"
#include "IdMessageBase.hpp"

using namespace std;

namespace obr_basementcontrol
{
  class UpstreamMessage : public IdMessageBase {
  public:
    virtual GcmStanzaMsgType getMessageType();

    virtual void parse(Json::Value& messageRoot);

    virtual string buildPayload();

    virtual MessageBase* clone();
      
    const string& getFrom();

    void setFrom(const string& value);

    const string& getCategory();

    void setCategory(const string& value);

    const Json::Value& getData();

    void setData(const Json::Value& value);

    virtual void accept(IGcmMessageVisitor *visitor);

  private:
    string from;
    string category;
    Json::Value data;
  };
}

#endif
