#pragma once

/*==============================================================================
Prototype tcp server thread message thread.
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "risNetTcpMsgServerThread.h"
#include "risNetSessionStateList.h"
#include "risThreadsQCallThread.h"

#include "procoMsg.h"

namespace ProtoComm
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// This is a tcp server message thread that connects to multiple tcp client
// message threads via a tcp server message child thread which manages a tcp
// server hub socket and multiple tcp message sockets. It then communicates
// byte content messages with the multiple clients.
//
// It provides the capability to establish tcp connections with the tcp
// clients via the child thread. When the child thread connects or disconnects
// to a client it invokes a qcall that was registered by this thread to defer 
// execution of a session notification handler.
//
// It provides the capability to send messages via the child thread socket and
// it provides handlers for messages received via the child thread socket.
// When the child thread receives a message it invokes a qcall that was
// registered by this thread to defer execution of a message handler.
// 
// It inherits from BaseQCallThread to obtain a call queue based thread
// functionality.

class ServerThread : public Ris::Threads::BaseQCallThread
{
public:
   typedef Ris::Threads::BaseQCallThread BaseClass;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Members.

   // Tcp server thread, this manages session connections and message
   // transmission and reception via a tcp server hub socket and a tcp
   // message socket.
   Ris::Net::TcpMsgServerThread* mTcpMsgServerThread;

   // Message monkey creator used by mTcpServerThread.
   ProtoComm::MsgMonkeyCreator mMonkeyCreator;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Members.

   // Session state list. This contains state for each connected session.
   Ris::Net::SessionStateList mSessionStateList;

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
   ServerThread();
   ~ServerThread();

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
   // a session is established or disestablished (when a client connects or
   // disconnects). 
   Ris::Net::TcpMsgServerThread::SessionQCall mSessionQCall;

   // Associated QCall methods. It maintains session state variables. When
   // a connection is established it sends a FirstMessage to the server to
   // inform it of it's identity.
   void executeSession(int aSessionIndex, bool aConnected);

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods. Receive message qcall.

   // QCall registered to the mTcpMsgThread child thread. It is invoked when
   // a message is received. It process the received messages.
   Ris::Net::TcpMsgServerThread::RxMsgQCall mRxMsgQCall;

   // Associated QCall method. It calls one of the specific receive message
   // handlers.
   void executeRxMsg(int aSessionIndex,Ris::ByteContent* aMsg);

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods. Specific receive message handlers.

   void processRxMsg(int aSessionIndex, TestMsg* aMsg);
   void processRxMsg(int aSessionIndex, FirstMessageMsg* aMsg);
   void processRxMsg(int aSessionIndex, StatusRequestMsg* aMsg);
   void processRxMsg(int aSessionIndex, StatusResponseMsg* aMsg);

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // Send a message.
   void sendMsg(int aSessionIndex, ProtoComm::BaseMsg* aMsg);
   void sendTestMsg(int aAppNumber);
};

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Global singular instance.

#ifdef _PROCOSERVERTHREAD_CPP_
         ServerThread* gServerThread;
#else
extern   ServerThread* gServerThread;
#endif

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace

