#ifndef GCM_CONNECTION_DRAINING_MESSAGE_HPP
#define GCM_CONNECTION_DRAINING_MESSAGE_HPP

#include <string>
#include "GcmMessageBase.hpp"

using namespace std;

namespace obr_basementcontrol
{
  class ConnectionDrainingMessage : public MessageBase {
  public:
    virtual GcmStanzaMsgType getMessageType();

    virtual void parse(Json::Value& messageRoot);

    virtual string buildPayload();

    virtual void dumpToStream(ostream& stream);

    virtual MessageBase* clone();
      
    virtual void dumpToStream(ostream& stream) const;

    friend ostream& operator<<(ostream& stream, const ConnectionDrainingMessage& dt)
    {
      stream << "here";
      return stream;
    }
  };
}

#endif
