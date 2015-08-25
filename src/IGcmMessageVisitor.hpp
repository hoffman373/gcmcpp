#ifndef I_GCM_MESSAGE_VISITOR_HPP
#define I_GCM_MESSAGE_VISITOR_HPP
//#include "AckMessage.hpp"
//#include "NackMessage.hpp"
//#include "ReceiptMessage.hpp"
//#include "ConnectionDrainingMessage.hpp"
//#include "UpstreamMessage.hpp"
//#include "NotificationMessage.hpp"
//#include "DownstreamMessage.hpp"

using namespace std;

namespace obr_basementcontrol
{
  class AckMessage;
  class NackMessage;
  class ReceiptMessage;
  class ConnectionDrainingMessage;
  class UpstreamMessage;
  class NotificationMessage;
  class DownstreamMessage;

  class IGcmMessageVisitor
  {
  public:
    virtual void visit(AckMessage &toVisit)=0;
    virtual void visit(NackMessage &toVisit)=0;
    virtual void visit(ReceiptMessage &toVisit)=0;
    virtual void visit(ConnectionDrainingMessage &toVisit)=0;
    virtual void visit(UpstreamMessage &toVisit)=0;
    virtual void visit(NotificationMessage &toVisit)=0;
    virtual void visit(DownstreamMessage &toVisit)=0;
  };
}


#endif
