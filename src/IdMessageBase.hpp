#ifndef ID_MESSAGE_BASE_HPP
#define ID_MESSAGE_BASE_HPP

#include <string>
#include "GcmMessageBase.hpp"

using namespace std;

namespace obr_basementcontrol
{
  class IdMessageBase : public MessageBase {
  public:

    const string& getMessageId();

    void setMessageId(const string& value);

  private:
    string messageId;
  };
}

#endif
