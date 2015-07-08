#include "GlooxGcmManager.hpp"
#include <algorithm>
#include <iostream>
#include "GcmStanza.hpp"
#include "AckMessage.hpp"
#include "NackMessage.hpp"
#include "UpstreamMessage.hpp"
#include "NotificationMessage.hpp"
#include "ConnectionDrainingMessage.hpp"
#include "ReceiptMessage.hpp"
#include <gloox/gloox.h>
#include <gloox/connectiontls.h>
#include <gloox/connectiontcpclient.h>
#include <gloox/message.h>
#include <thread>
#include <chrono>
#include <set>

using namespace obr_basementcontrol;
using namespace gloox;
using namespace std;

GlooxGcmManager::GlooxGcmManager(const string& hostname, int port, const string& usename, const string& password)
  : IGcmManager(hostname, port, usename, password)
{
  sendWorkBuffer = new IdMessageBase*[WORK_BUFFER_SIZE];
  sendWorkerKeepRunning = false;
  currentState = GcmManagerState::Disconnected;
  current = nullptr;
}

GlooxGcmManager::~GlooxGcmManager()
{
  delete[] sendWorkBuffer;
  delete current;
}

void GlooxGcmManager::enqueToSend(IdMessageBase* toSendMsg)
{
  unique_lock<mutex> mqLock(msgQueueMutex);
  toSend.push(toSendMsg);
  mqLock.unlock();

  // Notify the worker that something is available to work.
  workQueueCv.notify_one();
}

void GlooxGcmManager::start()
{
  // Connect to Service.
  retryCount = 0;
  currentState = GcmManagerState::Connecting;
  sendWorkerKeepRunning = true;
  receiveWorkerThread = thread(&GlooxGcmManager::receiveWorker, this);
  sendWorkerThread = thread(&GlooxGcmManager::sendWorker, this);
  resendWorkerThread = thread(&GlooxGcmManager::resendWorker, this);
}

void GlooxGcmManager::receiveWorker()
{
  // Continue to connect while the state is correct.
  while(currentState == GcmManagerState::Connecting || currentState == GcmManagerState::Draining) {
    Client* localClient = new Client(hostname);
    current = localClient;
    localClient->setPassword(password);
    localClient->setUsername(username);
    localClient->setPort(port);
    localClient->setSasl(true);
    localClient->logInstance().registerLogHandler(gloox::LogLevelDebug, gloox::LogAreaAll, this);
    ConnectionTLS* connection = new ConnectionTLS(localClient,
						     new ConnectionTCPClient( localClient->logInstance(), hostname,port),
						  localClient->logInstance());
    localClient->setConnectionImpl(connection);
    localClient->registerConnectionListener(this);
    localClient->registerMessageHandler(this);
    localClient->registerStanzaExtension(new GcmStanza((Tag*)nullptr));       
    // Block while we use this connection.
    localClient->connect(true);
    handleLog(LogLevelDebug, LogAreaUser, "Connection client terminated.");
    delete localClient;
  }

  handleLog(LogLevelDebug, LogAreaUser, "Receive worker finished.");
}

void GlooxGcmManager::stop()
{
  handleLog(LogLevelDebug, LogAreaUser, "Stopping manager.");
  currentState = GcmManagerState::Disconnected;
  sendWorkerKeepRunning = false;
  workQueueCv.notify_one();
  sendWorkerThread.join();
  resendWorkerThread.join();
  handleSystemStopped();
  handleLog(LogLevelDebug, LogAreaUser, "Manager stopped.");
}

void GlooxGcmManager::resendWorker()
{
  while (sendWorkerKeepRunning)
    {
      unique_lock<mutex> wkLock(workQueueMutex, defer_lock);
      handleLog(LogLevelDebug, LogAreaUser, "Resend worker waking up.");
      wkLock.lock();
      bool doWork = messagesWaitingForAck.size() > 0;
      wkLock.unlock();
      while(doWork) {
	wkLock.lock();
	set<string> toRemove;
	for(pair<const string, pair<IdMessageBase*, int>> entry : messagesWaitingForAck)
	  {
	    int newVal = ++(entry.second.second);
	    switch(newVal)
	      {
	      case 4:
	      case 8:
	      case 16:
	      case 32:
	      case 64:
	      case 128:
	      case 256:
	      case 512:
		{		  
		  // Resend the message.		  
		  IdMessageBase* toStore = entry.second.first;
		  handleLog(LogLevelDebug, LogAreaUser, "Resending " + toStore->getMessageId());
		  IdMessageBase* msgToSend = (IdMessageBase*)toStore->clone();
		  
		  GcmStanza* message = new GcmStanza(msgToSend);	 
		  Message toSend(Message::MessageType::Chat, JID());
		  toSend.addExtension(message);
		  current->send(toSend);
		  break;
		}
	      case 1024:
		{
		  // Kill the message.
		  handleLog(LogLevelDebug, LogAreaUser, "Killing " + entry.first);
		  toRemove.insert(entry.first);
		  break;
		}
	      }
	  }

	// Removed killed messages.
	for(string toRem : toRemove) {
	  IdMessageBase* msg = messagesWaitingForAck[toRem].first;
	  handleMessageDropped(*msg);
	  messagesWaitingForAck.erase(toRem);
	  delete msg;
	}
	wkLock.unlock();
	
	this_thread::sleep_for (chrono::seconds(2));
	wkLock.lock();
	doWork = messagesWaitingForAck.size() > 0;
	wkLock.unlock();
      }

      // Sleep until we have things in messagesWaitingForAck.
      wkLock.lock();
      resendWorkerCv.wait(wkLock);
      wkLock.unlock();
    }
}

GcmManagerState GlooxGcmManager::getState()
{
  return currentState;
}

void GlooxGcmManager::sendWorker()
{  
  while(sendWorkerKeepRunning)
    {
      unique_lock<mutex> wkLock(workQueueMutex, defer_lock);

      handleLog(LogLevelDebug, LogAreaUser, "Send working waking up.");
      if(currentState == GcmManagerState::Connected) {
	handleLog(LogLevelDebug, LogAreaUser, "Send working waking up while in the connected state.");
	// Only do work if we are connected.
	
	int workBufferCount = 0;
	unique_lock<mutex> mqLock(msgQueueMutex);
	for(int i=0; i < WORK_BUFFER_SIZE && toSend.size() > 0; i++)
	  {
	    // Fill up the work buffer.
	    sendWorkBuffer[i] = toSend.front();
	    toSend.pop();
	    workBufferCount++;
	  }
	mqLock.unlock();
	
	// Perform send operations.
	bool startResendWorker = false;
	for(int i=0; i < workBufferCount; i++)
	  {
	    handleLog(LogLevelDebug, LogAreaUser, "Sending a message out the door.");
	    IdMessageBase* msgToSend = sendWorkBuffer[i];
	    if(msgToSend->getMessageType() != GcmStanzaMsgType::ACK ||
	       msgToSend->getMessageType() != GcmStanzaMsgType::NACK) {
	      IdMessageBase* toStore = (IdMessageBase*)msgToSend->clone();
	      wkLock.lock();
	      // Move to the wait queue.
	      messagesWaitingForAck[msgToSend->getMessageId()] = pair<IdMessageBase*, int>(toStore, 0);
	      wkLock.unlock();
	    }
	    
	    // Send.
	    // send operation takes ownship of the message pointer.
	    GcmStanza* message = new GcmStanza(msgToSend);	 
	    Message toSend(Message::MessageType::Chat, JID());
	    toSend.addExtension(message);
	    current->send(toSend);
	    startResendWorker = true;
	    handleLog(LogLevelDebug, LogAreaUser, "Message sent");
	  }

	if(startResendWorker) {
	  resendWorkerCv.notify_all();
	}
      }

      // Wait until we get signaled.
      wkLock.lock();
      workQueueCv.wait(wkLock);
      wkLock.unlock();
    }
}

void GlooxGcmManager::onConnect()
{
  retryCount = 0;
  handleLog(LogLevelDebug, LogAreaUser, "Connected to the XMPP server.");
  currentState = GcmManagerState::Connected;
  // Notify the worker that the connection state changed.
  workQueueCv.notify_one();
}

string GlooxGcmManager::getErrorString(ConnectionError e)
{
  switch(e)
    {
    case ConnNoError:
      return "ConnNoError";      
    case ConnStreamError:
      return "ConnStreamError";      
    case ConnStreamVersionError:
      return "ConnStreamVersionError";      
    case ConnStreamClosed:
      return "ConnStreamClosed";      
    case ConnProxyAuthFailed:
      return "ConnProxyAuthFailed";      
    case ConnProxyNoSupportedAuth:
      return "ConnProxyNoSupportedAuth";      
    case ConnConnectionRefused:
      return "Refused";      
    case ConnIoError:
      return "ConnIoError";
    case ConnParseError:
      return "ConnParseError";     
    case ConnProxyAuthRequired:
      return "ConnProxyAuthRequired";      
    case ConnDnsError:
      return "ConnDnsError";      
    case ConnOutOfMemory:
      return "ConnOutOfMemory";      
    case ConnNoSupportedAuth:
      return "ConnNoSupportedAuth";      
    case ConnTlsFailed:
      return "ConnTlsFailed";      
    case ConnTlsNotAvailable:
      return "ConnTlsNotAvailable";      
    case ConnCompressionFailed:
      return "ConnCompressionFailed";      
    case ConnAuthenticationFailed:
      return "ConnAuthenticationFailed";      
    case ConnUserDisconnected:
      return "ConnUserDisconnected";      
    case ConnNotConnected:
      return "ConnNotConnected";      
    default:
      return "Unknkown error";     
    }
}

void GlooxGcmManager::onDisconnect(ConnectionError e)
{
  if(currentState == GcmManagerState::Connected) {
    // Stop has not been calld, so try to connected again.
    currentState == GcmManagerState::Connecting;
  } else if(currentState == GcmManagerState::Connecting) {
    // We are having an issue connecting, use exponential backoff and reconnect.
    if(retryCount < MAX_RETRY_COUNT) {
      int pauseInSeconds = 2^retryCount;
      this_thread::sleep_for (chrono::seconds(pauseInSeconds));
      retryCount++;
    } else {
      // Start the retry process again.
      retryCount = 0;
    }
  }
  
  handleLog(LogLevelDebug, LogAreaUser, "Disconnected with an error: " + getErrorString(e));
}

bool GlooxGcmManager::onTLSConnect(const CertInfo& info)
{
  handleLog(LogLevelDebug, LogAreaUser, "onTLSConnect");
  return true;
}

void GlooxGcmManager::handleLog(LogLevel level, LogArea area, const string& message)
{
  cout << message << endl;
}

void GlooxGcmManager::handleMessage (const Message &msg, MessageSession *session)
{
  GcmStanza* stanza = (GcmStanza*)msg.findExtension(ExtUser + 100);
  if(stanza != nullptr) {
    MessageBase* msg = stanza->pilferMesage();
    switch(msg->getMessageType()) {
    case GcmStanzaMsgType::ACK: {
      AckMessage* ack = (AckMessage*)msg;
      handleLog(LogLevelDebug, LogAreaUser, "Received an ack.");
      unique_lock<mutex> wkLock(workQueueMutex);
      if(messagesWaitingForAck.count(ack->getMessageId())==1) {
	handleLog(LogLevelDebug, LogAreaUser, "Ack was found in the waiting table.");
	IdMessageBase* toRemove = messagesWaitingForAck[ack->getMessageId()].first;
	messagesWaitingForAck.erase(ack->getMessageId());
	handleMessageSuccess(*toRemove);
	delete toRemove;
      }

      handleAckMessage(*ack);
      break;
    }
    case GcmStanzaMsgType::NACK: {
      NackMessage* nack = (NackMessage*)msg;
      unique_lock<mutex> wkLock(workQueueMutex);
      if(messagesWaitingForAck.count(nack->getMessageId())==1) {
	// Attempt to send again depending upon the error.
	switch(nack->getError()) {
	case GcmStanzaErrorType::INVALID_JSON:
	case GcmStanzaErrorType::BAD_REGISTRATION:
	case GcmStanzaErrorType::DEVICE_UNREGISTERED:
	case GcmStanzaErrorType::BAD_ACK:
	case GcmStanzaErrorType::SERVICE_UNAVAILABLE:
	case GcmStanzaErrorType::INTERNAL_SERVER_ERROR:
	  {
	    // Drop the message.
	    unique_lock<mutex> wkLock(workQueueMutex);
	    if(messagesWaitingForAck.count(nack->getMessageId())==1) {
	      handleLog(LogLevelDebug, LogAreaUser, "Nack was found in the waiting table.");
	      IdMessageBase* toRemove = messagesWaitingForAck[nack->getMessageId()].first;
	      messagesWaitingForAck.erase(nack->getMessageId());
	      handleMessageDropped(*toRemove);
	      delete toRemove;
	    }
	    
	    break;
	  }
	  // In these cases, we let the timeout mechanism attempt to resend messages later.
	  // DEVICE_MESSAGE_RATE_EXCEEDED,
	  // TOPICS_MESSAGE_RATE_EXCEEDED,
	  // CONNECTION_DRAINING
	    }
      }

      handleNackMessage(*nack);
      break;
    }
    case GcmStanzaMsgType::UPSTREAM: {
      handleLog(LogLevelDebug, LogAreaUser, "Sending ack for upstream message.");
      UpstreamMessage* upstream = (UpstreamMessage*)msg;
      ackMessage(upstream->getMessageId(), upstream->getFrom());
      handleUpstreamMessage(*upstream);
      break;
    }
    case GcmStanzaMsgType::CONNECTION_DRAINING: {
      handleLog(LogLevelDebug, LogAreaUser, "Connection Draining");
      currentState = GcmManagerState::Draining;
      break;
    }
    case GcmStanzaMsgType::RECEIPT: {
      ReceiptMessage* message = (ReceiptMessage*)msg;      
      hanleReceipt(*message);
      break;
    }
    }
    delete msg;
  }
}

void GlooxGcmManager::ackMessage(const string& messageId, const string& to)
{
  AckMessage* toAck = new AckMessage();
  toAck->setMessageId(messageId);
  toAck->setTo(to);
  enqueToSend(toAck);
}

void GlooxGcmManager::handleUpstreamMessage(UpstreamMessage& msg)
{
  NotificationMessage* pingBack = new NotificationMessage();
  pingBack->setTo(msg.getFrom());
  pingBack->setTitle("Hello There!");
  pingBack->setBody("How are you doing?");
  pingBack->setMessageId("PingBack" + msg.getMessageId());
  pingBack->setIcon("@drawable/myicon");
  enqueToSend(pingBack);
}

void GlooxGcmManager::handleNackMessage(NackMessage& msg)
{
}

void GlooxGcmManager::handleAckMessage(AckMessage& msg)
{
}

void GlooxGcmManager::hanleReceipt(ReceiptMessage& message)
{
}

void GlooxGcmManager::handleSystemStopped()
{
}

void GlooxGcmManager::handleMessageDropped(IdMessageBase& toDrop)
{
}

void GlooxGcmManager::handleMessageSuccess(IdMessageBase& toDrop)
{
}
