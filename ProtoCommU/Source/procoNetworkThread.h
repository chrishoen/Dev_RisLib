#pragma once

/*==============================================================================
Prototype udp message thread.
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "risNetUdpMsgThread.h"
#include "risThreadsQCallThread.h"

#include "procoMsg.h"

namespace ProtoComm
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// This is a udp message thread that transmits and receives byte content
// messages via a udp message child thread which manages a udp message socket.
//
// It provides the capability to send messages via the child thread socket and
// it provides handlers for messages received via the child thread socket.
// When the child thread receives a message it invokes a qcall that was
// registered by this thread to defer execution of a message handler that is 
// a member of this thread.
//   
// It inherits from BaseQCallThread to obtain a call queue based thread
// functionality.

class  NetworkThread : public Ris::Threads::BaseQCallThread
{
public:
   typedef Ris::Threads::BaseQCallThread BaseClass;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Members.

   // Udp message child thread, this manages message transmission and
   // reception via a udp message socket.
   Ris::Net::UdpMsgThread* mUdpMsgThread;

   // Message monkey creator used by mUdpMsgThread.
   ProtoComm::MsgMonkeyCreator mMonkeyCreator;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Members.

   // Control variables.
   bool mTPFlag;

   // Metrics.
   int  mStatusCount1;
   int  mStatusCount2;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // Constructor.
   NetworkThread();
  ~NetworkThread();

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods. Thread base class overloads:

   // threadInitFunction starts the child thread.
   // threadExitFunction shuts down the child thread.
   // executeOnTimer sends a periodic status message.
   void threadInitFunction()override;
   void threadExitFunction()override; 
   void executeOnTimer(int)override;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods. Receive message qcall.

   // qcall registered to the mUdpMsgThread child thread. It is invoked when
   // a message is received. It process the received messages.
   Ris::Net::UdpMsgThread::RxMsgQCall mRxMsgQCall;

   // Call one of the specific receive message handlers. This is bound to the
   // qcall.
   void executeRxMsg (Ris::ByteContent* aMsg);

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods. Specific receive message handlers.

   void processRxMsg (ProtoComm::TestMsg*  aMsg);
   void processRxMsg (ProtoComm::EchoRequestMsg* aMsg);
   void processRxMsg (ProtoComm::EchoResponseMsg* aMsg);
   void processRxMsg (ProtoComm::DataMsg* aMsg);

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // Send a message.
   void sendMsg (ProtoComm::BaseMsg* aMsg);
   void sendTestMsg();   
};

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Global singular instance.

#ifdef _PROCONETWORKTHREAD_CPP_
         NetworkThread* gNetworkThread;
#else
extern   NetworkThread* gNetworkThread;
#endif

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace
