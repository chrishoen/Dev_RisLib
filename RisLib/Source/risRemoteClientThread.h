#ifndef _REMOTECLIENTTHREAD_H_
#define _REMOTECLIENTTHREAD_H_

/*==============================================================================
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************
#include "risNetUdpMsgSocket.h"
#include "risNetTcpClientThread.h"
#include "risThreadsQCall.h"
#include "risCmdLineCmd.h"
#include "risCmdLineString.h"
#include "risRemoteMsg.h"

namespace Ris
{
namespace Remote
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
class RemoteClientCmdLineThread;
   
class RemoteClientThread : public Ris::Threads::BaseQCallThread
{
public:
   typedef Ris::Threads::BaseQCallThread BaseClass;

   //--------------------------------------------------------------
   // Constructor
   RemoteClientThread();
  ~RemoteClientThread();

   // Configure
   void configure(char* aIpAddress,int aPortNumber,BaseCmdLineExec* aCmdLineExec=0);

   // Thread that executes remote command line message
   RemoteClientCmdLineThread* mCmdLineThread;

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

   Ris::Net::TcpClientThreadWithQCall*  mRemoteTcpServerThread;

   // Message parser used by transmit socket
   RemoteMsg::MessageParser* mRemoteMessageParser;

   //--------------------------------------------------------------
   // Command line executive thread

   RemoteClientCmdLineThread* mRemoteClientCmdLineThread;

   //--------------------------------------------------------------
   // QCalls registered to mTcpThreads

   Ris::Net::TcpClientThreadWithQCall::SessionQCall  mRemoteSessionQCall;
   Ris::Net::TcpClientThreadWithQCall::RxMsgQCall    mRemoteRxMsgQCall;

   // Associated QCall methods
   void executeRemoteSession (bool aConnected);
   void executeRemoteRxMsg   (Ris::ByteContent* aRxMsg);

   bool  mRemoteConnected;

   //--------------------------------------------------------------
   // Send messages

   void sendMsgToRemote(RemoteMsg::BaseMsg* aTxMsg);
};
//******************************************************************************
//******************************************************************************
//******************************************************************************
class RemoteClientCmdLineThread : public Ris::Threads::BaseQCallThread
{
public:
   typedef Ris::Threads::BaseQCallThread BaseClass;

   // Constructor
   RemoteClientCmdLineThread();
  ~RemoteClientCmdLineThread();

   // Configure
   void configure(BaseCmdLineExec* aCmdLineExec);

   //--------------------------------------------------------------
   // Thread base class overloads:

   // configure sets configuration members

   void configureThread();

   //--------------------------------------------------------------
   // Command line executive that executes the command lines

   // Command line executive
   BaseCmdLineExec* mCmdLineExec;

   // Command line string processor
   Ris::CmdLineString mCmdLineString;

   //--------------------------------------------------------------
   // QCall, ExecuteCmdLine:

   typedef Ris::Threads::QCall1<Ris::RemoteMsg::CmdLineStrMsg*> ExecuteCmdLineQCall;
   ExecuteCmdLineQCall mExecuteCmdLineQCall;
   void executeOnExecuteCmdLine (Ris::RemoteMsg::CmdLineStrMsg*);

   //--------------------------------------------------------------
   // Execute receive message

   void executeRemoteRxMsg   (Ris::RemoteMsg::CmdLineStrMsg* aRxMsg);
};

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Global instance

#ifdef _REMOTECLIENTTHREAD_CPP_
        RemoteClientThread* gClientThread=0;
#else
extern  RemoteClientThread* gClientThread;
#endif

//******************************************************************************
}//namespace
}//namespace


#endif

