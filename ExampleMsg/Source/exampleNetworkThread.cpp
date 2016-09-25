/*==============================================================================
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************
#include "risSockets.h"
#include "prnPrint.h"
#include "exampleSettings.h"
#include "exampleMsgHelper.h"

#define  _EXAMPLENETWORKTHREAD_CPP_
#include "exampleNetworkThread.h"


namespace ExampleMsg
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
   ExampleMsg::BaseMsg* tMsg = (ExampleMsg::BaseMsg*)aMsg;

   // Message jump table based on message type.
   // Calls corresponding specfic message handler method.
   switch (tMsg->mMessageType)
   {
      case ExampleMsg::MsgIdT::cTestMsg :
         processRxMsg((ExampleMsg::TestMsg*)tMsg);
         break;
      case ExampleMsg::MsgIdT::cStatusRequestMsg :
         processRxMsg((ExampleMsg::StatusRequestMsg*)tMsg);
         break;
      case ExampleMsg::MsgIdT::cStatusResponseMsg :
         processRxMsg((ExampleMsg::StatusResponseMsg*)tMsg);
         break;
      case ExampleMsg::MsgIdT::cDataMsg :
         processRxMsg((ExampleMsg::DataMsg*)tMsg);
         break;
      default :
         Prn::print(Prn::ThreadRun1, "NetworkThread::executeServerRxMsg ??? %d",tMsg->mMessageType);
         delete tMsg;
         break;
   }
}

//******************************************************************************
// Message handler for TestMsg.

void NetworkThread::processRxMsg(ExampleMsg::TestMsg*  aMsg)
{
   MsgHelper::show(aMsg);
   delete aMsg;
}

//******************************************************************************
// Rx message handler - StatusRequestMsg

void NetworkThread::processRxMsg(ExampleMsg::StatusRequestMsg* aMsg)
{
   if (true)
   {
      ExampleMsg::StatusResponseMsg* tMsg = new ExampleMsg::StatusResponseMsg;
      mUdpMsgThread->sendMsg(tMsg);
   }

   MsgHelper::show(aMsg);
   delete aMsg;
}

//******************************************************************************
// Rx message handler - StatusResponseMsg

void NetworkThread::processRxMsg(ExampleMsg::StatusResponseMsg* aMsg)
{
   MsgHelper::show(aMsg);
   delete aMsg;
}

//******************************************************************************
// Rx message handler - DataMsg

void NetworkThread::processRxMsg(ExampleMsg::DataMsg* aMsg)
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

   ExampleMsg::TestMsg* tx = new ExampleMsg::TestMsg;
   tx->mCode1=101;

   mUdpMsgThread->sendMsg(tx);
}

//******************************************************************************
// This sends a message via the tcp client thread

void NetworkThread::sendMsg (ExampleMsg::BaseMsg* aMsg)
{
   mUdpMsgThread->sendMsg(aMsg);
}

//******************************************************************************
// This sends a test message via the tcp client thread

void NetworkThread::sendTestMsg()
{
   ExampleMsg::TestMsg* tMsg = new ExampleMsg::TestMsg;
 
   mUdpMsgThread->sendMsg(tMsg);
}

}//namespace