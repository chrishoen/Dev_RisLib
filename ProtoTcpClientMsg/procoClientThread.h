#pragma once

/*==============================================================================
Tcp client message thread prototype class.
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "risThreadsQCallThread.h"
#include "risNetTcpClientMsgThread.h"

#include "procoMsg.h"

//******************************************************************************
//******************************************************************************
//******************************************************************************

namespace ProtoComm
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Tcp client message thread class. It processes messages that are
// communicated via tcp sockets. The messages follow the byte content binary
// message scheme.
//
// It inherits from BaseQCallThread to obtain a call queue based thread
// functionality.
//
// The thread creates a child tcp client message thread that manages a tcp
// client connection, receives messages and passes them to this thread via
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

class  ClientThread : public Ris::Threads::BaseQCallThread
{
public:
   typedef Ris::Threads::BaseQCallThread BaseClass;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Members.

   // Tcp message thread, this manages tcp message connections and
   // message transmission and reception.
   Ris::Net::TcpClientMsgThread* mMsgThread;

   // Message monkey used by the message thread.
   MsgMonkey* mMsgMonkey;

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

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // Constructor.
   ClientThread();
  ~ClientThread();

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods. Thread base class overloads.

   // Thread init function. This is called by the base class immedidately 
   // after the thread starts running. It creates and launches the 
   // child message thread.
   void threadInitFunction() override;

   // Thread exit function. This is called by the base class immedidately
   // before the thread is terminated. It shuts down the child message 
   // thread.
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

   // qcall registered to the child message thread. It is invoked when a
   // session is established or disestablished (when this client connects or
   // disconnects to the server). 
   Ris::Net::TcpClientMsgThread::SessionQCall mSessionQCall;

   // Maintain session state variables. When a connection is established it
   // sends a FirstMessage to the server to inform it of its identity.This is
   // bound to the qcall.
   void executeSession(bool aConnected);

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods. Receive message qcall.

   // qcall registered to the child message thread. It is invoked when
   // a message is received. It process the received messages.
   Ris::Net::TcpClientMsgThread::RxMsgQCall mRxMsgQCall;

   // Call one of the following specific receive message handlers. This is
   // bound to the qcall.
   void executeRxMsg(Ris::ByteContent* aMsg);

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // Receive message handlers. There is one for each message that can 
   // be received.
   void processRxMsg(ProtoComm::TestMsg* aMsg);
   void processRxMsg(ProtoComm::EchoRequestMsg* aMsg);
   void processRxMsg(ProtoComm::EchoResponseMsg* aMsg);
   void processRxMsg(ProtoComm::DataMsg* aMsg);
   void processRxMsg(ProtoComm::ByteBlobMsg* aMsg);

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // Send a message via the child message thread:
   void sendMsg (BaseMsg* aTxMsg);
   void sendTestMsg();
};

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Global singular instance.

#ifdef _PROCOCLIENTTHREAD_CPP_
         ClientThread* gClientThread = 0;
#else
extern   ClientThread* gClientThread;
#endif

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace
