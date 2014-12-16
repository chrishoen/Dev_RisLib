/*==============================================================================
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "prnPrint.h"

#define  _RISREMOTESERVERTHREAD_CPP_
#include "risRemoteServerThread.h"

namespace Ris
{
namespace Remote
{

//******************************************************************************
//******************************************************************************
//******************************************************************************

ServerThread::ServerThread()
{
   mRemoteConnected=false;

   // Members
   mRemoteTcpServerThread       = new Ris::Net::TcpServerThreadWithQCall;
   mRemoteMessageParser   = new RemoteMsg::MessageParser;

   // QCall CallPointers
   mRemoteSessionQCall.bind      (this,&ServerThread::executeRemoteSession);
   mRemoteMsgRxQCall.bind        (this,&ServerThread::executeRemoteMsgRx);
   mRemoteRunTestQCall.bind      (this,&ServerThread::executeOnRemoteRunTest);
}

//******************************************************************************

ServerThread::~ServerThread()
{
   delete mRemoteTcpServerThread;
   delete mRemoteMessageParser;
}

//******************************************************************************
// This sets configuration members

void ServerThread::configure(int aPortNumber)
{
   Prn::print(Prn::ThreadInit,0, "ServerThread:configure");

   //--------------------------------------------------------------------------- 
   // Configure message parsers
   mRemoteMessageParser->configure(1001);

   //--------------------------------------------------------------------------- 
   // Configure child thread, server

   mRemoteTcpServerThread->configure(
      "0.0.0.0",
      aPortNumber,
      MaxSessions,
      mRemoteMessageParser,
      &mRemoteSessionQCall,
      &mRemoteMsgRxQCall);
}

//******************************************************************************
// This sets base thread configuration members

void ServerThread::configureThread()
{
   // Set base class configuration members to defaults
   BaseThread::configureThread();

   // Set base class thread priority
   BaseThread::setThreadPriorityHigh();

   // Set members for this thread
   BaseThread::setThreadName("RemoteServerThread");

// BaseThread::mThreadStackSize = 10*1000*1000;
}

//******************************************************************************

void ServerThread::threadInitFunction()
{
   Prn::print(Prn::ThreadInit,0, "ServerThread::threadInitFunction");
   BaseClass::threadInitFunction();

   // Launch child thread
   mRemoteTcpServerThread->launchThread(); 
}

//******************************************************************************
// Thread exit function, base class overload.

void  ServerThread::threadExitFunction()
{
   Prn::print(Prn::ThreadInit,0, "ServerThread::threadExitFunction");

   // Shutdown child thread
   mRemoteTcpServerThread->shutdownThread();

   // Base class exit
   BaseClass::threadExitFunction();
}

//******************************************************************************

void ServerThread::executeRemoteSession(int aSessionIndex,bool aConnected)
{
   if (aConnected) Prn::print(Prn::ProcRun,Prn::Run2, "ServerThread REMCON         CONNECTED");
   else            Prn::print(Prn::ProcRun,Prn::Run2, "ServerThread REMCON         DISCONNECTED");

   if(aConnected)
   {
      // The connection was lost, so remove the session from the state list
      mRemoteSessionStateList.add(aSessionIndex,0);
   }
   else
   {
      // The connection was lost, so remove the session from the state list
      mRemoteSessionStateList.remove(aSessionIndex);
   }

   mRemoteConnected=aConnected;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Remote RxMsg

void ServerThread::executeRemoteMsgRx(int aSessionIndex,Ris::ByteContent* aRxMsg)
{
   RemoteMsg::BaseMsg* tRxMsg = (RemoteMsg::BaseMsg*)aRxMsg;

   // Message jump table based on message type.
   // Calls corresponding specfic message handler method.
   switch (tRxMsg->mMessageType)
   {
      case RemoteMsg::MsgIdT::PrintStr :
         processRemotePrintStrMsgRx((RemoteMsg::PrintStrMsg*)tRxMsg);
         break;
	  default :
		  Prn::print(Prn::ProcRun,Prn::Run2, "ServerThread::executeServerRxMsg ??? %d",tRxMsg->mMessageType);
		  delete tRxMsg;
		  break;
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
void ServerThread::processRemotePrintStrMsgRx   (RemoteMsg::PrintStrMsg*  aRxMsg)
{
   Prn::print(0,0,">>> %s",aRxMsg->mPrintStr);
   delete aRxMsg;
}
//******************************************************************************
//******************************************************************************
//******************************************************************************
// QCall

void ServerThread::executeOnRemoteRunTest(char* aTestName)
{
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// This is called by the base class at 2 hertz. It can be used to send a 
// periodic status message.

void ServerThread::executeOnTimer(int aTimerCount)
{
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void ServerThread::sendMsgToRemote(RemoteMsg::BaseMsg* aTxMsg)
{
   // Guard
   if (mRemoteSessionStateList.mNumSessions==0)
   {
      DecreaseResource(aTxMsg);
      return;
   }

   // Increase message resource count for multiple transmits
   aTxMsg->increaseResource(mRemoteSessionStateList.mNumSessions);

   // For each session index that refers to a valid session
   for (int tSessionIndex=0;tSessionIndex<Ris::Net::SessionStateList::MaxSessions;tSessionIndex++)
   {
      if (mRemoteSessionStateList.isValidIndex(tSessionIndex))
      {
         // Send a status message to the session client
         mRemoteTcpServerThread->sendMsg(tSessionIndex,aTxMsg);
      }
   }
}


}//namespace
}//namespace
