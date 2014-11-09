/*==============================================================================
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include <stdlib.h>
#include "prnPrint.h"

#define  _REMOTECLIENTTHREAD_CPP_
#include "risRemoteClientThread.h"

namespace Ris
{
namespace Remote
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************

RemoteClientThread::RemoteClientThread()
{
   mRemoteConnected=false;

   mRemoteTcpServerThread  = new Ris::Net::TcpClientThreadWithQCall;

   mRemoteMessageParser = new RemoteMsg::MessageParser;

   mCmdLineThread = 0;

   // QCall CallPointers
   mRemoteSessionQCall.bind  (this,&RemoteClientThread::executeRemoteSession);
   mRemoteRxMsgQCall.bind    (this,&RemoteClientThread::executeRemoteRxMsg);
}

//******************************************************************************

RemoteClientThread::~RemoteClientThread()
{
   delete mRemoteTcpServerThread;
   delete mRemoteMessageParser;

   if (mCmdLineThread)
   {
      delete mCmdLineThread;
   }
}

//******************************************************************************
// This sets configuration members

void RemoteClientThread::configure(char* aIpAddress,int aPortNumber,BaseCmdLineExec* aCmdLineExec)
{
   Prn::print(Prn::ThreadInit,0, "RemoteClientThread:configure");

   //--------------------------------------------------------------------------- 
   // Configure child thread, client

   mRemoteTcpServerThread->configure(
      aIpAddress,
      aPortNumber,
      mRemoteMessageParser,
      &mRemoteSessionQCall,
      &mRemoteRxMsgQCall);

   //--------------------------------------------------------------------------- 
   // Command line thread

   if (aCmdLineExec!=0)
   {
      mCmdLineThread = new RemoteClientCmdLineThread;
      mCmdLineThread->configure(aCmdLineExec);
      mCmdLineThread->launchThread();
   }
}

//******************************************************************************
// This sets base thread configuration members

void RemoteClientThread::configureThread()
{
   // Set base class configuration members to defaults
   BaseClass::configureThread();

   // Set base class thread priority
   BaseClass::setThreadPriorityHigh();

   BaseClass::mTimerPeriod = 1000;
}

//******************************************************************************

void RemoteClientThread::threadInitFunction()
{
   Prn::print(Prn::ThreadInit,0, "RemoteClientThread::threadInitFunction");
   BaseClass::threadInitFunction();

   // Launch child thread
   mRemoteTcpServerThread->launchThread();
}

//******************************************************************************
// Thread exit function, base class overload.

void  RemoteClientThread::threadExitFunction()
{
   Prn::print(Prn::ThreadInit,0, "RemoteClientThread::threadExitFunction");

   // Shutdown child thread
   mRemoteTcpServerThread->shutdownThread();

   // Shutdown command line thread
   if (mCmdLineThread)
   {
      mCmdLineThread->shutdownThread();
   }

   // Base class exit
   BaseClass::threadExitFunction();
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void RemoteClientThread::executeOnTimer(int aTimerCount)
{
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void RemoteClientThread::executeRemoteSession(bool aConnected)
{
   if (aConnected)
   {
      Prn::print(Prn::ProcRun,Prn::Run2, "RemoteClientThread SERVER        CONNECTED");
   }
   else
   {
      Prn::print(Prn::ProcRun,Prn::Run2, "RemoteClientThread SERVER        DISCONNECTED");
   }
   mRemoteConnected=aConnected;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void RemoteClientThread::executeRemoteRxMsg(Ris::ByteContent* aRxMsg)
{
   RemoteMsg::BaseMsg* tRxMsg = (RemoteMsg::BaseMsg*)aRxMsg;

   // Message jump table based on message type.
   // Calls corresponding specfic message handler method.
   switch (tRxMsg->mMessageType)
   {
      case RemoteMsg::MsgIdT::RunTestAck :
      {
         Prn::print(Prn::ProcRun,Prn::Run2, "RemoteClientThread::executeRemoteRxMsg RunTestAck");
         delete aRxMsg;
      }
      break;
      case RemoteMsg::MsgIdT::TestCompletion :
      {
         Prn::print(Prn::ProcRun,Prn::Run2, "RemoteClientThread::executeRemoteRxMsg TestCompletion %s",((RemoteMsg::TestCompletionMsg*)tRxMsg)->mTestName);
         delete aRxMsg;
      }
      break;
      case RemoteMsg::MsgIdT::CmdLineStr :
      {
         if (mCmdLineThread)
         {
            mCmdLineThread->mExecuteCmdLineQCall((RemoteMsg::CmdLineStrMsg*)tRxMsg);
         }
      }
      break;
      default :
         Prn::print(Prn::ProcRun,Prn::Run2, "RemoteClientThread::executeRemoteRxMsg ??? %d",tRxMsg->mMessageType);
         delete aRxMsg;
         break;
   }

}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// These send messages via the sockets

void RemoteClientThread::sendMsgToRemote(RemoteMsg::BaseMsg* aTxMsg)
{
   if (mRemoteConnected)
   {
      mRemoteTcpServerThread->sendMsg(aTxMsg);
   }
   else
   {
      DecreaseResource(aTxMsg);
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************

RemoteClientCmdLineThread::RemoteClientCmdLineThread()
{
   mCmdLineExec=0;

   // QCall CallPointers
   mExecuteCmdLineQCall.bind  (this,&RemoteClientCmdLineThread::executeRemoteRxMsg);
}

//******************************************************************************

RemoteClientCmdLineThread::~RemoteClientCmdLineThread()
{
}

//******************************************************************************
// This sets configuration members

void RemoteClientCmdLineThread::configure(BaseCmdLineExec* aCmdLineExec)
{
   mCmdLineExec=aCmdLineExec;
}

//******************************************************************************
// This sets base thread configuration members

void RemoteClientCmdLineThread::configureThread()
{
   // Set base class configuration members to defaults
   BaseClass::configureThread();
   // No timer
   mCreateThreadTimerFlag=false;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void RemoteClientCmdLineThread::executeRemoteRxMsg(Ris::RemoteMsg::CmdLineStrMsg* aRxMsg)
{
// Prn::print(0,0, "RemoteClientCmdLineThread::executeRemoteRxMsg %s",aRxMsg->mCmdLineStr);
   mCmdLineString.execute(mCmdLineExec,aRxMsg->mCmdLineStr);
   delete aRxMsg;
}

}//namespace
}//namespace
