#pragma once

/*==============================================================================
Prototype tcp client thread message thread.
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "risNetTcpMsgClientThread.h"
#include "risThreadsQCallThread.h"

#include "procoMsg.h"

namespace ProtoComm
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// This is a tcp client message thread that connects to a tcp server message
// thread via a tcp client message child thread which manages a tcp message
// socket. It then communicates byte content messages with the server.
//
// It provides the capability to establish a tcp connection with the tcp
// server via the child thread. When the child thread connects or disconnects
// to the server it invokes a qcall that was registered by this thread to defer 
// execution of a session notification handler.
//
// It provides the capability to send messages via the child thread socket and
// it provides handlers for messages received via the child thread socket.
// When the child thread receives a message it invokes a qcall that was
// registered by this thread to defer execution of a message handler.
// 
// It inherits from BaseQCallThread to obtain a call queue based thread
// functionality.

class ClientThread : public Ris::Threads::BaseQCallThread
{
public:
   typedef Ris::Threads::BaseQCallThread BaseClass;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Members.

   // Tcp client thread, this manages session connections and message
   // transmission and reception via a tcp message socket.
   Ris::Net::TcpMsgClientThread* mTcpMsgClientThread;

   // Message monkey creator used by mTcpClientThread.
   ProtoComm::MsgMonkeyCreator mMonkeyCreator;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Members.

   // This indicates if a server connection is valid.
   bool mConnectionFlag;

   // Control variables.
   bool mTPFlag;

   // Metrics.
   int  mPeriodicCount;
   int  mStatusCount1;
   int  mStatusCount2;

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
   // Methods. Session qcall.

   // QCall registered to the mTcpMsgThread child thread. It is invoked when
   // a session is established or disestablished (when this client connects or
   // disconnects to the server). 
   Ris::Net::TcpMsgClientThread::SessionQCall mSessionQCall;

   // Associated QCall methods. It maintains session state variables.
   void executeSession(bool aConnected);

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods. Receive message qcall.

   // QCall registered to the mTcpMsgThread child thread. It is invoked when
   // a message is received. It process the received messages.
   Ris::Net::TcpMsgClientThread::RxMsgQCall mRxMsgQCall;

   // Associated QCall method. It calls one of the specific receive message
   // handlers.
   void executeRxMsg(Ris::ByteContent* aMsg);

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods. Specific receive message handlers.

   void processRxMsg(TestMsg* aRxMsg);
   void processRxMsg(StatusRequestMsg* aRxMsg);
   void processRxMsg(StatusResponseMsg* aRxMsg);

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // Send a message.
   void sendMsg(ProtoComm::BaseMsg* aTxMsg);
   void sendTestMsg();
};

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Global singular instance.

#ifdef _PROCOCLIENTTHREAD_CPP_
         ClientThread* gClientThread;
#else
extern   ClientThread* gClientThread;
#endif

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace
