/*==============================================================================
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************
#include "stdafx.h"

#include "risSockets.h"
#include "procoSettings.h"
#include "procoMsgHelper.h"

#define  _PROCONETWORKTHREAD_CPP_
#include "procoNetworkThread.h"


namespace ProtoComm
{

//******************************************************************************
//******************************************************************************
//******************************************************************************

NetworkThread::NetworkThread()
{
   mStatusCount1=0;
   mStatusCount2=0;

   mUdpMsgThread = new Ris::Net::UdpMsgThread;

   // Initialize QCalls
   mRxMsgQCall.bind   (this,&NetworkThread::executeRxMessage);
}

//******************************************************************************

NetworkThread::~NetworkThread()
{
   delete mUdpMsgThread;
}

//******************************************************************************
// This configures members

void NetworkThread::configure()
{
   Prn::print(Prn::ThreadInit1, "NetworkThread::configure");

   //--------------------------------------------------------------------------- 
   // Configure message monkey

   mMonkeyCreator.configure(gSettings.mMyAppNumber);

   //---------------------------------------------------------------------------
   // Configure message thread

   mUdpMsgThread->configure(
      &mMonkeyCreator,
      gSettings.mMyUdpIPAddress,
      gSettings.mMyUdpPort,
      gSettings.mOtherUdpIPAddress,
      gSettings.mOtherUdpPort,
      &mRxMsgQCall);
}

//******************************************************************************

void NetworkThread::launchThread()
{
   Prn::print(Prn::ThreadInit1, "NetworkThread::launch");

   // Launch child thread
   mUdpMsgThread->launchThread(); 
   
   // Launch this thread
   BaseClass::launchThread();
}

//******************************************************************************
// Thread exit function, base class overload.
void  NetworkThread::threadExitFunction()
{
   Prn::print(Prn::ThreadInit1, "NetworkThread::threadExitFunction");

   // Shutdown the tcp client thread
   mUdpMsgThread->shutdownThread();

   // Base class exit
   BaseClass::threadExitFunction();
}

//******************************************************************************
// QCall

void NetworkThread::executeRxMessage(Ris::ByteContent* aMsg)
{
   ProtoComm::BaseMsg* tMsg = (ProtoComm::BaseMsg*)aMsg;

   // Message jump table based on message type.
   // Calls corresponding specfic message handler method.
   switch (tMsg->mMessageType)
   {
      case ProtoComm::MsgIdT::cTestMsg :
         processRxMsg((ProtoComm::TestMsg*)tMsg);
         break;
      case ProtoComm::MsgIdT::cStatusRequestMsg :
         processRxMsg((ProtoComm::StatusRequestMsg*)tMsg);
         break;
      case ProtoComm::MsgIdT::cStatusResponseMsg :
         processRxMsg((ProtoComm::StatusResponseMsg*)tMsg);
         break;
      case ProtoComm::MsgIdT::cDataMsg :
         processRxMsg((ProtoComm::DataMsg*)tMsg);
         break;
      default :
         Prn::print(Prn::ThreadRun1, "NetworkThread::executeServerRxMsg ??? %d",tMsg->mMessageType);
         delete tMsg;
         break;
   }
}

//******************************************************************************
// Message handler for TestMsg.

void NetworkThread::processRxMsg(ProtoComm::TestMsg*  aMsg)
{
   MsgHelper::show(aMsg);
   delete aMsg;
}

//******************************************************************************
// Rx message handler - StatusRequestMsg

void NetworkThread::processRxMsg(ProtoComm::StatusRequestMsg* aMsg)
{
   if (true)
   {
      ProtoComm::StatusResponseMsg* tMsg = new ProtoComm::StatusResponseMsg;
      mUdpMsgThread->sendMsg(tMsg);
   }

   MsgHelper::show(aMsg);
   delete aMsg;
}

//******************************************************************************
// Rx message handler - StatusResponseMsg

void NetworkThread::processRxMsg(ProtoComm::StatusResponseMsg* aMsg)
{
   MsgHelper::show(aMsg);
   delete aMsg;
}

//******************************************************************************
// Rx message handler - DataMsg

void NetworkThread::processRxMsg(ProtoComm::DataMsg* aMsg)
{
   MsgHelper::show(aMsg);
   delete aMsg;
}

//******************************************************************************
// QCall

void NetworkThread::executeOnTimer(int aTimerCount)
{
   Prn::print(Prn::ThreadRun2, "NetworkThread::executeRxMessage");

   return;

   ProtoComm::TestMsg* tx = new ProtoComm::TestMsg;
   tx->mCode1=101;

   mUdpMsgThread->sendMsg(tx);
}

//******************************************************************************
// This sends a message via the tcp client thread

void NetworkThread::sendMsg (ProtoComm::BaseMsg* aMsg)
{
   mUdpMsgThread->sendMsg(aMsg);
}

//******************************************************************************
// This sends a test message via the tcp client thread

void NetworkThread::sendTestMsg()
{
   ProtoComm::TestMsg* tMsg = new ProtoComm::TestMsg;
 
   mUdpMsgThread->sendMsg(tMsg);
}

}//namespace