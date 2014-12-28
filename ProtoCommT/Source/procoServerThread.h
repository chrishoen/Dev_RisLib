#ifndef _PROCOSERVERTHREAD_H_
#define _PROCOSERVERTHREAD_H_

/*==============================================================================
ProtoComm server thread class.
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************
#include "prnPrint.h"

#include "risContainers.h"
#include "risNetTcpServerThread.h"
#include "risNetSessionStateList.h"
#include "risThreadsQCall.h"

#include "procoMsg.h"

namespace ProtoComm
{

//******************************************************************************
// Server Thread.
//
// This is the server thread class. It inherits from BaseQCallThread to
// obtain a call queue based thread functionality.
//
// The server thread acts in conjunction with the server message processor
// object. It passes received messages to the processor and gets back messages
// to transmit from the processor. The server thread provides the execution
// context for the message processor to process the messages.
// 
// The server thread has a member, mTcpServerThread that is an instance of
// Ris::Net::TcpServerThreadWithQCall. It is a child thread that manages
// connection session changes and receives messages as a Tcp server. So, there
// are two threads structured as two layers: The server thread and its member
// child thread mTcpServerThread.
//
// The server thread is based on a call queue and it uses QCalls to interface
// its mTcpServerThread. When mTcpServerThread detects a session change it
// invokes the server thread's mSessionQCall, which defers execution of its
// executeSession member function. Likewise, when mTcpServerThread receives
// a message it invokes the server thread's mRxMessageQCall, which defers 
// execution of its executeRxMsg member function. 
//
// mTcpServerThread provides the execution context for actually managing
// session changes and receiving messages. The session thread the provides
// the execution context for processing the session changes and the received 
// messages. The processing is done by the message processor object.
//
//******************************************************************************

class  ServerThread : public Ris::Threads::BaseQCallThread
{
public:
   typedef Ris::Threads::BaseQCallThread BaseClass;

   ServerThread();
  ~ServerThread();

   //--------------------------------------------------------------
   // Configure:

   void configure(int aMyApplicationId, int aPortNumber);

   //--------------------------------------------------------------
   // Thread base class overloads:

   // configure sets configuration members.
   // launch starts the child threads + this thread
   // threadInitFunction sets up the base class multiple objects.
   // threadExitFunction shuts down the child thread
   // shutdown shuts down child threads + this thread
   void configureThread();
   void launchThread();
   void threadExitFunction(); 
   void executeOnTimer(int);

   //--------------------------------------------------------------
   // Tcp server thread, this manages session connections and 
   // message transmission and reception

   Ris::Net::TcpServerThreadWithQCall* mTcpServerThread;

   // Maximum number of sessions for mTcpServerThread
   enum {MaxSessions=10};

   // Message parser used by mTcpServerThread
   ProtoComm::MessageParser* mMessageParser;

   //--------------------------------------------------------------
   // QCall:

   // QCalls registered to mTcpServerThread
   Ris::Net::TcpServerThreadWithQCall::SessionQCall  mSessionQCall;
   Ris::Net::TcpServerThreadWithQCall::RxMsgQCall    mRxMsgQCall;

   // Associated QCall methods, these are called by the
   // threadRunFunction to process conditions sent from 
   // mTcpServerThread.
   void executeSession (int aSessionIndex,bool aConnected);
   void executeRxMsg   (int aSessionIndex,Ris::ByteContent* aRxMsg);

   //--------------------------------------------------------------
   // Receive message handlers:

   void processRxMsg (int aSessionIndex,ProtoComm::TestMsg* aRxMsg);
   void processRxMsg (int aSessionIndex,ProtoComm::FirstMessageMsg* aRxMsg);
   void processRxMsg (int aSessionIndex,ProtoComm::StatusRequestMsg* aRxMsg);
   void processRxMsg (int aSessionIndex,ProtoComm::StatusResponseMsg* aRxMsg);

   //--------------------------------------------------------------
   // Session state lists:
   // These contain state about each session

   Ris::Net::SessionStateList mSessionStateList;
   bool mPeriodicEnable;
   int  mPeriodicCount;
   int  mStatusCount1;
   int  mStatusCount2;

   //--------------------------------------------------------------
   // Send a message via mTcpServerThread:

   void sendMsg (int aSessionIndex,ProtoComm::BaseMsg* aTxMsg);
   void sendTestMsg ();
};
//******************************************************************************
// Global instance

#ifdef _PROCOSERVERTHREAD_CPP_
         ServerThread* gServerThread;
#else
extern   ServerThread* gServerThread;
#endif


//******************************************************************************
}//namespace


#endif

