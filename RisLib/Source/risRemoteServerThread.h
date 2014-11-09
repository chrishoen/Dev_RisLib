#ifndef _RISREMOTESERVERTHREAD_H_
#define _RISREMOTESERVERTHREAD_H_

/*==============================================================================
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************
#include "risContainers.h"
#include "risNetSessionStateList.h"
#include "risNetTcpServerThread.h"
#include "risNetTcpClientThread.h"
#include "risThreadsQCall.h"

#include "risRemoteMsg.h"

namespace Ris
{
namespace Remote
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Message Thread.
//
// This is the network thread class. It provides a central thread that manages 
// test sequencer network communications.

// The test sequencer is a tcp server that accepts connections from clients. 
// It is also a tcp client to the hardware controller, the pipeline broker, and
// the bit server. And, it is also a tcp client to the Rfeg, either the Ceesim
// or the Ames.
//
// It has five member socket threads that are used to communicate:
//
//   Ris::Net::TcpServerThreadWithQCall*  mConConTcpServerThread;
//   Ris::Net::TcpClientThreadWithQCall*  mHwControlTcpClientThread;
//   Ris::Net::TcpClientThreadWithQCall*  mPipeBrokerTcpClientThread;
//   Ris::Net::TcpClientThreadWithQCall*  mBitServerTcpClientThread;
//
// The server thread accepts multiple connections from clients and the client
// threads connect to corresponding servers. They are child threads that manage
// connection session changes and receive messages as Tcp participants. So,
// there are multiple threads structured as two layers: The network thread and
// its member child threads. The child threads provide the execution context 
// for actually managing session changes and receiving messages. The network
// thread provides the execution context for processing the session changes
// and received messages.
//
// The network thread acts in conjunction with the network message processor
// object, which are both visible to each other. The network thread receives
// messages from its member tcp socket threads and passes them to the
// processor. The processor processes the messages and sends other messages
// back out via the network thread. The network thread provides the execution
// context for the message processor to process the messages.
// 
//******************************************************************************

class ServerThread : public Ris::Threads::BaseQCallThread
{
public:
   typedef Ris::Threads::BaseQCallThread BaseClass;

   //--------------------------------------------------------------
   // Constructor

   ServerThread();
  ~ServerThread();

   // COnfigure 
   void configure(int aPortNumber);

   //--------------------------------------------------------------
   // Thread base class overloads:

   // configure sets configuration members
   // init starts an event timer
   // launch starts the child threads + this thread
   // threadExitFunction shuts down the child thread
   // executeOnTimer sends out periodic status

   void configureThread();
   void threadInitFunction(); 
   void threadExitFunction(); 
   void executeOnTimer(int);

   //--------------------------------------------------------------
   // Tcp threads, these manages session connections and 
   // message transmission and reception

   Ris::Net::TcpServerThreadWithQCall*  mRemoteTcpServerThread;

   // Maximum number of sessions for mTcpServerThread
   enum {MaxSessions=4};

   // Message parsers used by Tcp threads
   RemoteMsg::MessageParser*     mRemoteMessageParser;

   //--------------------------------------------------------------
   // QCalls registered to mTcpThreads

   Ris::Net::TcpServerThreadWithQCall::SessionQCall  mRemoteSessionQCall;
   Ris::Net::TcpServerThreadWithQCall::RxMsgQCall    mRemoteMsgRxQCall;


   // Associated QCall methods
   void executeRemoteSession      (int aSessionIndex,bool aConnected);
   void executeRemoteMsgRx        (int aSessionIndex,Ris::ByteContent* aRxMsg);

   //**************************************************************************
   // Message processors
   //**************************************************************************
   void processRemotePrintStrMsgRx   (RemoteMsg::PrintStrMsg*  aRxMsg);

   //--------------------------------------------------------------
   // QCall, RemoteRunTest:

   typedef Ris::Threads::QCall1<char*> RemoteRunTestQCall;
   RemoteRunTestQCall mRemoteRunTestQCall;
   void executeOnRemoteRunTest (char* aTestName);

   //--------------------------------------------------------------
   // Send messages

   void sendMsgToRemote      (RemoteMsg::BaseMsg* aTxMsg);

   //--------------------------------------------------------------
   // State:

   Ris::Net::SessionStateList mRemoteSessionStateList;
   bool                       mRemoteConnected;
   int                        mRemoteSessionIndex;

   // Message counters
   int mHwControlEntityStatusCount;
   int mRfegControlEntityStatusCount;

};

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Global instance

#ifdef _RISREMOTESERVERTHREAD_CPP_
        ServerThread* gServerThread=0;
#else
extern  ServerThread* gServerThread;
#endif

//******************************************************************************
}//namespace
}//namespace


#endif

