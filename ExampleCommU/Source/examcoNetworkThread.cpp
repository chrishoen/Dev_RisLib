/*==============================================================================
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************
#include "risSockets.h"
#include "prnPrint.h"
#include "examcoSettings.h"

#define  _EXAMCONETWORKTHREAD_CPP_
#include "examcoNetworkThread.h"


namespace ProtoComm
{

//******************************************************************************
//******************************************************************************
//******************************************************************************

NetworkThread::NetworkThread()
{
   mStatusCount1=0;
   mStatusCount2=0;

   mUdpRxThread = new Ris::Net::UdpRxThread;

   // Initialize QCalls
   mRxMsgQCall.bind   (this,&NetworkThread::executeRxMsg);
}

//******************************************************************************

NetworkThread::~NetworkThread()
{
   delete mUdpRxThread;
}

//******************************************************************************
// This configures members

void NetworkThread::configure()
{
   Prn::print(Prn::ThreadInit1, "NetworkThread::configure");

   //--------------------------------------------------------------------------- 
   // Configure message parser

   mMessageParserCreator.configure(gSettings.mMyAppNumber);

   //---------------------------------------------------------------------------
   // Configure receive socket thread

   mUdpRxThread->configure(
      gSettings.mMyUdpIPAddress,
      gSettings.mMyUdpPort,
      &mMessageParserCreator,
      &mRxMsgQCall);

   //---------------------------------------------------------------------------
   // Configure transmit socket

   mTxSocket.configure(
      gSettings.mOtherUdpIPAddress,
      gSettings.mOtherUdpPort,
      &mMessageParserCreator);
}

//******************************************************************************

void NetworkThread::launchThread()
{
   Prn::print(Prn::ThreadInit1, "NetworkThread::launch");

   // Launch child thread
   mUdpRxThread->launchThread(); 
   
   // Launch this thread
   BaseClass::launchThread();
}

//******************************************************************************
// Thread exit function, base class overload.
void  NetworkThread::threadExitFunction()
{
   Prn::print(Prn::ThreadInit1, "NetworkThread::threadExitFunction");

   // Shutdown the tcp client thread
   mUdpRxThread->shutdownThread();

   // Base class exit
   BaseClass::threadExitFunction();
}

//******************************************************************************
// QCall

void NetworkThread::executeRxMsg(Ris::ByteContent* aRxMsg)
{
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
      case ProtoComm::MsgIdT::cDataMsg :
         processRxMsg((ProtoComm::DataMsg*)tRxMsg);
         break;
      default :
         Prn::print(Prn::ThreadRun1, "NetworkThread::executeServerRxMsg ??? %d",tRxMsg->mMessageType);
         delete tRxMsg;
         break;
   }
}

//******************************************************************************
// Message handler for TestMsg.

void NetworkThread::processRxMsg(ProtoComm::TestMsg*  aRxMsg)
{
   Prn::print(Prn::ThreadRun1, "NetworkThread::processRxMsg_TestMsg %d %d", aRxMsg->mCode1, aRxMsg->mHeader.mSourceId);
}

//******************************************************************************
// Rx message handler - StatusRequestMsg

void NetworkThread::processRxMsg(ProtoComm::StatusRequestMsg* aRxMsg)
{
   if (true)
   {
      ProtoComm::StatusResponseMsg* tTxMsg = new ProtoComm::StatusResponseMsg;
      sendMsg(tTxMsg);
   }

   Prn::print(Prn::ThreadRun1, "NetworkThread::processRxMsg_StatusRequestMsg %d",mStatusCount1++);
   Prn::print(Prn::ThreadRun1, "Code1      %d", aRxMsg->mCode1);
   Prn::print(Prn::ThreadRun1, "Code2      %d", aRxMsg->mCode2);
   Prn::print(Prn::ThreadRun1, "Code3      %d", aRxMsg->mCode3);
   Prn::print(Prn::ThreadRun1, "Code4      %d", aRxMsg->mCode4);

   delete aRxMsg;
}

//******************************************************************************
// Rx message handler - StatusResponseMsg

void NetworkThread::processRxMsg(ProtoComm::StatusResponseMsg* aRxMsg)
{
   Prn::print(Prn::ThreadRun1, "NetworkThread::processRxMsg_StatusResponseMsg");
   delete aRxMsg;
}

//******************************************************************************
// Rx message handler - DataMsg

void NetworkThread::processRxMsg(ProtoComm::DataMsg* aRxMsg)
{
   Prn::print(Prn::ThreadRun1, "NetworkThread::processRxMsg_DataMsg");

   aRxMsg->show();

   delete aRxMsg;
}

//******************************************************************************
// QCall

void NetworkThread::executeOnTimer(int aTimerCount)
{
   Prn::print(Prn::ThreadRun2, "NetworkThread::executeRxMsg");

   return;

   ProtoComm::TestMsg* tx = new ProtoComm::TestMsg;
   tx->mCode1=101;

   sendMsg(tx);
}

//******************************************************************************
// This sends a message via the tcp client thread

void NetworkThread::sendMsg (ProtoComm::BaseMsg* aTxMsg)
{
   mTxSocket.doSendMsg(aTxMsg);
}

//******************************************************************************
// This sends a test message via the tcp client thread

void NetworkThread::sendTestMsg()
{
   ProtoComm::TestMsg* msg = new ProtoComm::TestMsg;
   msg->mCode1=201;
 
   mTxSocket.doSendMsg(msg);
}

}//namespace