#ifndef GLOOX_GCM_MANAGER_HPP
#define GLOOX_GCM_MANAGER_HPP

#include "IdMessageBase.hpp"
#include "IGcmManager.hpp"
#include "UpstreamMessage.hpp"
#include "NackMessage.hpp"
#include "AckMessage.hpp"
#include "ReceiptMessage.hpp"

#include <string>
#include <queue>
#include <unordered_map>
#include <utility>
#include <mutex>
#include <condition_variable>
#include <thread>

#include <gloox/client.h>
#include <gloox/presencehandler.h>
#include <gloox/connectionlistener.h>
#include <gloox/loghandler.h>
#include <gloox/messagehandler.h>


using namespace std;
using namespace gloox;

namespace obr_basementcontrol
{
  class GlooxGcmManager : public IGcmManager, public ConnectionListener, LogHandler, MessageHandler
  {
  public:
    GlooxGcmManager(const string& hostname, int port, const string& usename, const string& password);
    ~GlooxGcmManager();
    virtual void enqueToSend(IdMessageBase* toSend);
    virtual void start();
    virtual void stop();
    virtual GcmManagerState getState();

    virtual void onConnect();
    virtual void onDisconnect(ConnectionError e);
    virtual bool onTLSConnect(const CertInfo& info);
    virtual void handleLog(LogLevel level, LogArea area, const string& message);
    virtual void handleMessage (const Message &msg, MessageSession *session=0);    

  protected:
    virtual void handleUpstreamMessage(UpstreamMessage& msg);
    virtual void handleNackMessage(NackMessage& msg);
    virtual void handleAckMessage(AckMessage& msg);
    virtual void hanleReceipt(ReceiptMessage& message);
    virtual void handleSystemStopped();
    virtual void handleMessageDropped(IdMessageBase& toDrop);
    virtual void handleMessageSuccess(IdMessageBase& toDrop);

  private:
    void ackMessage(const string& messageId, const string& to);
    
  private:
    unordered_map<string, pair<IdMessageBase*, int>> messagesWaitingForAck;
    queue<IdMessageBase*> toSend;
    IdMessageBase** sendWorkBuffer;
    static const int WORK_BUFFER_SIZE = 5;
    mutex msgQueueMutex;
    mutex workQueueMutex;
    condition_variable workQueueCv;
    condition_variable resendWorkerCv;
    bool sendWorkerKeepRunning;    
    void sendWorker();
    void receiveWorker();
    void resendWorker();
    thread sendWorkerThread;
    thread receiveWorkerThread;
    thread resendWorkerThread;
    GcmManagerState currentState;
    Client* current;
    static string getErrorString(ConnectionError e);
    static const int MAX_RETRY_COUNT = 9;
    int retryCount;
  };
  
}

#endif
