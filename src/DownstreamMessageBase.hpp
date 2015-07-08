#ifndef GCM_DOWNSTREAM_MESSAGE_BASE_HPP
#define GCM_DOWNSTREAM_MESSAGE_BASE_HPP

#include <string>
#include "GcmMessageBase.hpp"
#include "IdMessageBase.hpp"

using namespace std;

namespace obr_basementcontrol
{
  class DownstreamMessageBase : public IdMessageBase {
  public:

    DownstreamMessageBase();
    
    virtual void parse(Json::Value& messageRoot);

    virtual void clone(DownstreamMessageBase* dest);      

    const string& getTo();

    void setTo(const string& value);

    const string& getCollapseKey();

    void setCollapseKey(const string& value);

    int getPriority();

    void setPriority(const int value);

    bool getContentAvailable();

    void setContentAvailable(bool value);

    bool getDelayWhileIdle();

    void setDelayWhileIdle(bool value);

    int getTimeToLive();

    void setTimeToLive(const int value);

    bool getDeliveryReceiptRequested();

    void setDeliveryReceiptRequested(bool value);

    bool getDryRun();

    void setDryRun(bool value);

  protected:
    virtual void buildPayload(Json::Value& messageRoot);
    
  private:
    string to;
    string collapseKey;
    int priority;
    bool contentAvailable;
    bool delayWhileIdle;
    int timeToLive;
    bool deliveryReceiptRequested;
    bool dryRun;
  };
}

#endif
