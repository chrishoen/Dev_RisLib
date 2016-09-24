/*==============================================================================
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************
#include "prnPrint.h"
#include "procoSettings.h"

#define  _PROCOCLIENTTHREAD_CPP_
#include "procoClientThread.h"

namespace ProtoComm
{

//******************************************************************************
//******************************************************************************
//******************************************************************************

ClientThread::ClientThread()
{
   mTcpClientThread = new Ris::Net::TcpMsgClientThread;
   mConnectionFlag=false;
   mPeriodicEnable=false;
   mPeriodicCount=0;
   mStatusCount1=0;
   mStatusCount2=0;

   // Initialize QCalls
   mSessionQCall.bind (this,&ClientThread::executeSession);
   mRxMsgQCall.bind   (this,&ClientThread::executeRxMsg);
}

//******************************************************************************

ClientThread::~ClientThread()
{
   delete mTcpClientThread;
}

//******************************************************************************
// This sets configuration members

void ClientThread::configure()
{
   Prn::print(Prn::ThreadInit1, "ClientThread::configure");

   //--------------------------------------------------------------------------- 
   // Configure message monkey
   mMonkeyCreator.configure(gSettings.mMyAppNumber);

   //---------------------------------------------------------------------------
   // Configure child thread, client

   mTcpClientThread->configure(
      &mMonkeyCreator,
      "127.0.0.1",
      gSettings.mTcpServerPort,
      &mSessionQCall,
      &mRxMsgQCall);
}

//******************************************************************************
// This sets base thread configuration members

void ClientThread::configureThread()
{
   // Set base class configuration members to defaults
   BaseThread::configureThread();

   BaseClass::mTimerPeriod=1000;
}

//******************************************************************************

void ClientThread::launchThread()
{
   Prn::print(Prn::ThreadInit1, "ClientThread::launch");

   // Launch child thread
   mTcpClientThread->launchThread(); 
   
   // Launch this thread
   BaseClass::launchThread();
}

//******************************************************************************
// Thread exit function, base class overload.
void  ClientThread::threadExitFunction()
{
   Prn::print(Prn::ThreadInit1, "ClientThread::threadExitFunction");

   // Shutdown the tcp client thread
   mTcpClientThread->shutdownThread();

   // Base class exit
   BaseClass::threadExitFunction();
}

//******************************************************************************
// QCall
void ClientThread::executeSession (bool aConnected)
{
   if (aConnected)
   {
      Prn::print(Prn::ThreadRun1, "ClientThread::notifyForSession CONNECTED");

      // Transmit a FirstMessage to the server to inform it of who this 
      // client is.
      ProtoComm::FirstMessageMsg* msg = new ProtoComm::FirstMessageMsg;
      sendMsg(msg);
   }
   else
   {
      Prn::print(Prn::ThreadRun1, "ClientThread::notifyForSession DISCONNECTED");
   }

   mConnectionFlag = aConnected;
}

//******************************************************************************
// QCall

void ClientThread::executeRxMsg(Ris::ByteContent* aRxMsg)
{
   if(!aRxMsg) return;

   // Put the message to the message processor
   ProtoComm::BaseMsg* tRxMsg = (ProtoComm::BaseMsg*)aRxMsg;

   // Message jump table based on message type.
   // Calls corresponding specfic message handler method.
   switch (tRxMsg->mMessageType)
   {
      case ProtoComm::MsgIdT::cTestMsg :
         processRxMsg((ProtoComm::TestMsg*)tRxMsg);
         break;
      case ProtoComm::MsgIdT::cStatusRequestMsg :
         processRxMsg((ProtoComm::StatusRequestMsg*)tRxMsg);
         break;
      case ProtoComm::MsgIdT::cStatusResponseMsg :
         processRxMsg((ProtoComm::StatusResponseMsg*)tRxMsg);
         break;
      default :
         Prn::print(Prn::ThreadRun1, "ClientThread::processRxMsg %d",tRxMsg->mMessageType);
         delete aRxMsg;
         break;
   }
}

//******************************************************************************
// Rx message handler - TestMsg

void ClientThread::processRxMsg(ProtoComm::TestMsg* aRxMsg)
{
   Prn::print(Prn::ThreadRun1, "ClientThread::processRxMsg_TestMsg");
   delete aRxMsg;
}

//******************************************************************************
// executeOnTimer

void ClientThread::executeOnTimer(int aTimerCount)
{
   if (!mPeriodicEnable) return;
   Prn::print(Prn::ThreadRun1, "ClientThread::executeOnTimer %d",mPeriodicCount++);
}

//******************************************************************************
// Rx message handler - StatusRequestMsg

void ClientThread::processRxMsg(ProtoComm::StatusRequestMsg* aRxMsg)
{

   if (true)
   {
      ProtoComm::StatusResponseMsg* tTxMsg = new ProtoComm::StatusResponseMsg;
      sendMsg(tTxMsg);
   }

   Prn::print(Prn::ThreadRun1, "ClientThread::processRxMsg_StatusRequestMsg %d",mStatusCount1++);

   delete aRxMsg;
}

//******************************************************************************
// Rx message handler - StatusResponseMsg

void ClientThread::processRxMsg(ProtoComm::StatusResponseMsg* aRxMsg)
{
   Prn::print(Prn::ThreadRun1, "ClientThread::processRxMsg_StatusResponseMsg");
   delete aRxMsg;
}

//******************************************************************************
// This sends a message via the tcp client thread

void ClientThread::sendMsg (ProtoComm::BaseMsg* aTxMsg)
{
   mTcpClientThread->sendMsg(aTxMsg);
}

//******************************************************************************
// This sends a test message via the tcp client thread

void ClientThread::sendTestMsg()
{
   ProtoComm::TestMsg* msg = new ProtoComm::TestMsg;
   msg->mCode1=201;
 
   mTcpClientThread->sendMsg(msg);
}

}//namespace