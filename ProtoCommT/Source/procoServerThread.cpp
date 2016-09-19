/*==============================================================================
ServerThread.cpp
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "prnPrint.h"
#include "procoSettings.h"

#define  _PROCOSERVERTHREAD_CPP_
#include "procoServerThread.h"

namespace ProtoComm
{

//******************************************************************************
//******************************************************************************
//******************************************************************************

ServerThread::ServerThread()
{
   BaseClass::mTimerPeriod=1000;

   mPeriodicEnable=false;
   mPeriodicCount=0;
   mStatusCount1=0;
   mStatusCount2=0;

   mTcpServerThread  = new Ris::Net::TcpMsgAServerThread;

   // Initialize QCalls
   mSessionQCall.bind (this,&ServerThread::executeSession);
   mRxMsgQCall.bind   (this,&ServerThread::executeRxMsg);
}

//******************************************************************************

ServerThread::~ServerThread()
{
   delete mTcpServerThread;
}

//******************************************************************************
// This sets configuration members

void ServerThread::configure()
{
   Prn::print(Prn::ThreadInit1, "ServerThread::configure");

   //--------------------------------------------------------------------------- 
   // Configure message parser
   mMsgParserCreator.configure(gSettings.mMyAppNumber);

   //--------------------------------------------------------------------------- 
   // Configure child thread, server

   mTcpServerThread->configure(
      "0.0.0.0",
      gSettings.mTcpServerPort,
      MaxSessions,
      &mMsgParserCreator,
      &mSessionQCall,
      &mRxMsgQCall);
}

//******************************************************************************

void ServerThread::launchThread()
{
   Prn::print(Prn::ThreadInit1, "ServerThread::launch");

   // Launch child thread
   mTcpServerThread->launchThread(); 
   
   // Launch this thread
   BaseClass::launchThread();
}

//******************************************************************************
// Thread exit function, base class overload.

void  ServerThread::threadExitFunction()
{
   Prn::print(Prn::ThreadInit1, "ServerThread::threadExitFunction");

   // Shutdown child thread
   mTcpServerThread->shutdownThread(); 

   // Base class exit
   BaseClass::threadExitFunction();
}

//******************************************************************************
// QCall

void ServerThread::executeSession (int aSessionIndex,bool aConnected)
{
   if (aConnected) Prn::print(Prn::ThreadRun1, "ServerThread CLIENT  CONNECTED     %d",aSessionIndex);
   else            Prn::print(Prn::ThreadRun1, "ServerThread CLIENT  DISCONNECTED  %d",aSessionIndex);

   if(!aConnected)
   {
      // The connection was lost, so remove the session from the state list
      mSessionStateList.remove(aSessionIndex);
   }
}

//******************************************************************************
// QCall
void ServerThread::executeRxMsg(int aSessionIndex,Ris::ByteContent* aRxMsg)
{
   if(!aRxMsg) return;

   // Put the message to the message processor
   ProtoComm::BaseMsg* tRxMsg = (ProtoComm::BaseMsg*)aRxMsg;

   // Message jump table based on message type.
   // Calls corresponding specfic message handler method.
   switch (tRxMsg->mMessageType)
   {
      case ProtoComm::MsgIdT::cTestMsg :
         processRxMsg(aSessionIndex,(ProtoComm::TestMsg*)tRxMsg);
         break;
      case ProtoComm::MsgIdT::cFirstMessageMsg :
         processRxMsg(aSessionIndex,(ProtoComm::FirstMessageMsg*)tRxMsg);
         break;
      case ProtoComm::MsgIdT::cStatusRequestMsg :
         processRxMsg(aSessionIndex,(ProtoComm::StatusRequestMsg*)tRxMsg);
         break;
      case ProtoComm::MsgIdT::cStatusResponseMsg :
         processRxMsg(aSessionIndex,(ProtoComm::StatusResponseMsg*)tRxMsg);
         break;
      default :
         Prn::print(Prn::ThreadRun1, "ServerThread::processRxMsg %d",tRxMsg->mMessageType);
         delete aRxMsg;
         break;
   }
}

//******************************************************************************
// Rx message handler - TestMsg

void ServerThread::processRxMsg(int aSessionIndex,ProtoComm::TestMsg* aRxMsg)
{
   Prn::print(Prn::ThreadRun1, "ServerThread::processRxMsg_TestMsg");
   delete aRxMsg;
}

//******************************************************************************
// Rx message handler - FirstMessageMsg
//
// Specfic message handler for a FirstMessage. It adds the session to the 
// session state list. This message is sent by the client when a connection
// is established.

void ServerThread::processRxMsg(int aSessionIndex,ProtoComm::FirstMessageMsg* aRxMsg)
{
   Prn::print(Prn::ThreadRun1, "ServerThread::processRxMsg_FirstMessageMsg %d %d",aSessionIndex,aRxMsg->mHeader.mSourceId);

   //--------------------------------------------------------------
   // Process message:

   // Add session to state list
   mSessionStateList.add(aSessionIndex,aRxMsg->mHeader.mSourceId);

   // Process message:
   //--------------------------------------------------------------

   // Delete the message
   delete aRxMsg;
}

//******************************************************************************
// executeOnTimer

void ServerThread::executeOnTimer(int aTimerCount)
{
   Prn::print(Prn::ThreadRun2, "ServerThread::executeOnTimer %d",mPeriodicCount++);
   while(mPeriodicEnable) BaseClass::threadSleep(100);
}

//******************************************************************************
// Rx message handler - StatusRequestMsg

void ServerThread::processRxMsg(int aSessionIndex,ProtoComm::StatusRequestMsg* aRxMsg)
{
   if (false)
   {
      ProtoComm::StatusResponseMsg* tTxMsg = new ProtoComm::StatusResponseMsg;
      sendMsg(aSessionIndex,tTxMsg);
   }

   Prn::print(Prn::ThreadRun1, "ServerThread::processRxMsg_StatusRequestMsg %d",mStatusCount1++);

   delete aRxMsg;
}

//******************************************************************************
// Rx message handler - StatusResponseMsg

void ServerThread::processRxMsg(int aSessionIndex,ProtoComm::StatusResponseMsg* aRxMsg)
{
   Prn::print(Prn::ThreadRun1, "ServerThread::processRxMsg_StatusResponseMsg");
   delete aRxMsg;
}

//******************************************************************************
// This sends a message via the tcp server thread
void ServerThread::sendMsg(int aSessionIndex,ProtoComm::BaseMsg* aTxMsg)
{
   mTcpServerThread->sendMsg(aSessionIndex,aTxMsg);
}

//******************************************************************************
// This sends a test message via the tcp client thread
void ServerThread::sendTestMsg(int aAppNumber)
{
   // Get session index
   int tSessionIndex = mSessionStateList.getIndex(aAppNumber);
   if (tSessionIndex == Ris::Net::SessionStateList::InvalidValue) return;

   // Send message on socket at the session index
   ProtoComm::TestMsg* msg = new ProtoComm::TestMsg;
   msg->mCode1=201;

   mTcpServerThread->sendMsg(tSessionIndex,msg);
}

}//namespace
