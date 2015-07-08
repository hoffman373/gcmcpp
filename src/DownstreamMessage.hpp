#ifndef GCM_DOWNSTREAM_MESSAGE_HPP
#define GCM_DOWNSTREAM_MESSAGE_HPP

#include <string>
#include "GcmMessageBase.hpp"
#include "IdMessageBase.hpp"
#include "DownstreamMessageBase.hpp"

using namespace std;

namespace obr_basementcontrol
{
  class DownstreamMessage : public DownstreamMessageBase {
  public:

    DownstreamMessage();
    
    virtual GcmStanzaMsgType getMessageType();

    virtual void parse(Json::Value& messageRoot);

    virtual string buildPayload();

    virtual MessageBase* clone();
      
    const string& getData();

    void setData(const string& value);

  private:
    string data;
  };
}

#endif
