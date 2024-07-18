#pragma once

/*==============================================================================
Tcp server message thread prototype class.
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
// Tcp server message thread class. It processes messages that are
// communicated via tcp sockets. The messages follow the byte content binary
// message scheme.
//
// It inherits from BaseQCallThread to obtain a call queue based thread
// functionality.
//
// The thread creates a child tcp server message thread that manages a tcp
// server connection, receives messages and passes them to this thread via
// a qcall callback, and provides for the transmission of messages.
// 
// This thread is based on a call queue and it uses qcalls to interface to
// the child thread. When the child thread receives a message it invokes this
// threads receive message qcall, which defers execution of its receive
// message member function, which then processes the message. 
//
// The child thread provides the execution context for managing the socket
// and receiving messages on it. This thread provides the execution
// context for processing the received messages and for sending messags.
//
//

class  ServerThread : public Ris::Threads::BaseQCallThread
{
public:
   typedef Ris::Threads::BaseQCallThread BaseClass;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Members.

   // Tcp message thread, this manages tcp message connections and
   // message transmission and reception.
   Ris::Net::TcpServerMsgThread* mMsgThread;

   // Message monkey used by the child message thread.
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
   // child message thread.
   void threadInitFunction() override;

   // Thread exit function. This is called by the base class immedidately
   // before the thread is terminated. It shuts down the child message thread.
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

   // qcall registered to the child message thread. It is invoked when
   // a session is established or disestablished (when a client connects or
   // disconnects to this server). 
   Ris::Net::TcpServerMsgThread::SessionQCall mSessionQCall;

   // This is bound to the qcall. Maintain the session state list.
   void executeSession(int aSessionIndex, bool aConnected);

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods. Receive message qcall.

   // qcall registered to the child message thread. It is invoked when
   // a message is received. It process the received messages.
   Ris::Net::TcpServerMsgThread::RxMsgQCall mRxMsgQCall;

   // This is bound to the qcall. Call one of the following specific receive
   // message handlers.
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

   // Send a message via the child message thread:
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
