#pragma once

/*==============================================================================
Udp message prototype thread class.
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "risThreadsQCallThread.h"
#include "risNetUdpMsgThread.h"

#include "procoMsg.h"

//******************************************************************************
//******************************************************************************
//******************************************************************************

namespace ProtoComm
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Udp message thread class. It processes messages that are communicated
// via udp datagrams. The messages follow the byte content binary message
// scheme.
//
// It inherits from BaseQCallThread to obtain a call queue based thread
// functionality.
//
// The thread creates a child udp message thread that creates and manages
// a udp socket, receives messages and passes them to this thread via a qcall
// callback, and provides for the transmission of messages.
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

class  PeerThread : public Ris::Threads::BaseQCallThread
{
public:
   typedef Ris::Threads::BaseQCallThread BaseClass;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Members.

   // Udp message thread, this manages udp message transmission and reception.
   Ris::Net::UdpMsgThread* mMsgThread;

   // Message monkey used by the message thread.
   MsgMonkey* mMsgMonkey;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Members.

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
   PeerThread();
  ~PeerThread();

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods. Thread base class overloads.

   // Thread init function. This is called by the base class immedidately 
   // after the thread starts running. It creates and launches the child
   // message thread.
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
   // Methods. Receive message qcall.

   // qcall registered to the child message thread. It is invoked by
   // the child thread when a message is received.
   Ris::Net::UdpMsgThread::RxMsgQCall mRxMsgQCall;

   // Based on the receive message type, call one of the following specific
   // receive message handlers. This is bound to the qcall.
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

#ifdef _PROCOPEERTHREAD_CPP_
         PeerThread* gPeerThread = 0;
#else
extern   PeerThread* gPeerThread;
#endif

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace
