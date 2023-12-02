#pragma once

/*==============================================================================
Udp message prototype thread class.
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "risThreadsQCallThread.h"
#include "risNetTcpServerMsgThread.h"
#include "risNetSessionStateList.h"

#include "procoMsg.h"

//******************************************************************************
//******************************************************************************
//******************************************************************************

namespace ProtoComm
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Udp message prototype thread class. It processes messages that are
// communicated via a tcp channel. The messages follow the byte content
// binary message scheme.
//
// This is a prototype tcp thread class. It inherits from BaseQCallThread to
// obtain a call queue based thread functionality.
//
// The prototype thread creates a child tcp message thread that establishes
// and manages a tcp connection, receives messages and passes them to the
// parent via a qcall callback, and allows for the transmission of messages.
// the child thread also notifies the parent thread of tcp connection
// establishment/disestablishment via a qcall callback.
// 
// The prototype thread is based on a call queue and it uses qcalls to
// interface to the child thread. When the child thread detects a session
// change it invokes the prototypes thread's mSessionQCall, which defers
// execution of its executeSession member function. Likewise, when the child
// thread receives a message it invokes the tcp thread's mRxMsgQCall, which
// defers  execution of its executeRxMsg member function. 
//
// The child thread provides the execution context for actually managing
// session changes and receiving messages. The parent thread provides the
// execution context for processing the session changes and the received 
// messages.
//

class  ServerThread : public Ris::Threads::BaseQCallThread
{
public:
   typedef Ris::Threads::BaseQCallThread BaseClass;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Members.

   // Udp message thread, this manages tcp message connections and
   // message transmission and reception.
   Ris::Net::TcpServerMsgThread* mMsgThread;

   // Message monkey used by mMsgThread.
   MsgMonkey* mMsgMonkey;

   // Session state list. This contains state for each connected session.
   Ris::Net::SessionStateList mSessionStateList;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Members.

   // True if the tcp connection is valid.
   bool mConnectionFlag;

   // State variables.
   int mTPCode;
   int mRxCount;
   int mTxCount;
   int mShowCode;

   // App number.
   int mClientAppNumber;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // Constructor.
   ServerThread();
  ~ServerThread();

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods. Thread base class overloads.

   // Thread init function. This is called by the base class immedidately 
   // after the thread starts running. It creates and launches the 
   // child TcpServerMsgThread.
   void threadInitFunction() override;

   // Thread exit function. This is called by the base class immedidately
   // before the thread is terminated. It shuts down the child TcpServerMsgThread.
   void threadExitFunction() override;

   // Thread shutdown function. This calls the base class shutdownThread
   // function to terminate the thread. This executes in the context of
   // the calling thread.
   void shutdownThread() override;

   // Execute periodically. This is called by the base class timer.
   void executeOnTimer(int aTimerCount) override;

   // Show thread info for this thread and for child threads.
   void showThreadInfo() override;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods. Session qcall.

   // qcall registered to the mTcpMsgThread child thread. It is invoked when
   // a session is established or disestablished (when this client connects or
   // disconnects to the server). 
   Ris::Net::TcpServerMsgThread::SessionQCall mSessionQCall;

   // This is bound to the qcall. Maintain the session state list.
   void executeSession(int aSessionIndex, bool aConnected);

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods. Receive message qcall.

   // qcall registered to the mTcpMsgThread child thread. It is invoked when
   // a message is received. It process the received messages.
   Ris::Net::TcpServerMsgThread::RxMsgQCall mRxMsgQCall;

   // This is bound to the qcall. Call one of the specific receive message
   // handlers.
   void executeRxMsg(int aSessionIndex, Ris::ByteContent* aMsg);

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // Receive message handlers. There is one for each message that can 
   // be received.
   void processRxMsg(int aSessionIndex, ProtoComm::TestMsg* aMsg);
   void processRxMsg(int aSessionIndex, ProtoComm::FirstMessageMsg* aMsg);
   void processRxMsg(int aSessionIndex, ProtoComm::EchoRequestMsg* aMsg);
   void processRxMsg(int aSessionIndex, ProtoComm::EchoResponseMsg* aMsg);
   void processRxMsg(int aSessionIndex, ProtoComm::DataMsg* aMsg);
   void processRxMsg(int aSessionIndex, ProtoComm::ByteBlobMsg* aMsg);

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // Send a message via mMsgThread:
   void sendMsg (int aSessionIndex, BaseMsg* aTxMsg);
   void sendTestMsg(int aAppNumber);
};

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Global singular instance.

#ifdef _PROCOSERVERTHREAD_CPP_
         ServerThread* gServerThread = 0;
#else
extern   ServerThread* gServerThread;
#endif

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace
