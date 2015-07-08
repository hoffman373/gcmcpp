#ifndef I_GCM_MANAGER_HPP
#define I_GCM_MANAGER_HPP

#include "GcmMessageBase.hpp"
#include "IdMessageBase.hpp"

namespace obr_basementcontrol
{
  enum class GcmManagerState{Disconnected, Connected, Connecting, Waiting, Draining};
  
  class IGcmManager
  {
  public:
    IGcmManager(const string& hostname, int port, const string& usename, const string& password);
    // IGcmManager instance is responsible for destroying message.
    virtual void enqueToSend(IdMessageBase* toSend) = 0;    
    virtual void start() = 0;
    virtual void stop() = 0;
    virtual GcmManagerState getState() = 0;
  protected:
    string hostname;
    int port;
    string username;
    string password;
  };
  
}

#endif
