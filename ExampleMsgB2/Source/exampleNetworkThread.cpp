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
   // Configure socket thread

   mUdpMsgThread->configure(
      gSettings.mMyUdpIPAddress,
      gSettings.mMyUdpPort,
      gSettings.mOtherUdpIPAddress,
      gSettings.mOtherUdpPort,
      &mMsgMonkey,
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

void NetworkThread::executeRxMessage(Ris::ByteMsg* aMsg)
{
   // Message jump table based on message type.
   // Calls corresponding specfic message handler method.
   switch (aMsg->mMessageType)
   {
      case TypeIdT::cTestMsg :
         processRxMsg((TestMsg*)aMsg);
         break;
      case TypeIdT::cStatusMsg :
         processRxMsg((StatusMsg*)aMsg);
         break;
      case TypeIdT::cData1Msg :
         processRxMsg((Data1Msg*)aMsg);
         break;
      case TypeIdT::cData2Msg :
         processRxMsg((Data2Msg*)aMsg);
         break;
      case TypeIdT::cData3Msg :
         processRxMsg((Data3Msg*)aMsg);
         break;
      case TypeIdT::cData4Msg :
         processRxMsg((Data4Msg*)aMsg);
         break;
      default :
         Prn::print(Prn::ThreadRun1, "NetworkThread::executeServerRxMsg ??? %d",aMsg->mMessageType);
         delete aMsg;
         break;
   }
}

//******************************************************************************
// Message handler

void NetworkThread::processRxMsg(TestMsg* aMsg)
{
   Prn::print(Prn::ThreadRun1, "NetworkThread::processRxMsg_TestMsg" );

   MsgHelper::show(aMsg);

   delete aMsg;
}

//******************************************************************************
// Message handler

void NetworkThread::processRxMsg(StatusMsg* aMsg)
{
   Prn::print(Prn::ThreadRun1, "NetworkThread::processRxMsg_StatusMsg %d",mStatusCount1++);

   MsgHelper::show(aMsg);

   delete aMsg;
}

//******************************************************************************
// Message handler

void NetworkThread::processRxMsg(Data1Msg* aMsg)
{
   Prn::print(Prn::ThreadRun1, "NetworkThread::processRxMsg_Data1Msg");

   MsgHelper::show(aMsg);

   delete aMsg;
}

//******************************************************************************
// Message handler

void NetworkThread::processRxMsg(Data2Msg* aMsg)
{
   Prn::print(Prn::ThreadRun1, "NetworkThread::processRxMsg_Data2Msg");

   MsgHelper::show(aMsg);

   delete aMsg;
}

//******************************************************************************
// Message handler

void NetworkThread::processRxMsg(Data3Msg* aMsg)
{
   Prn::print(Prn::ThreadRun1, "NetworkThread::processRxMsg_Data3Msg");

   MsgHelper::show(aMsg);

   delete aMsg;
}

//******************************************************************************
// Message handler

void NetworkThread::processRxMsg(Data4Msg* aMsg)
{
   Prn::print(Prn::ThreadRun1, "NetworkThread::processRxMsg_Data4Msg");

   MsgHelper::show(aMsg);

   delete aMsg;
}

//******************************************************************************
// QCall

void NetworkThread::executeOnTimer(int aTimerCount)
{
   Prn::print(Prn::ThreadRun2, "NetworkThread::executeRxMessage");

   return;

   TestMsg* tx = new TestMsg;
   tx->mCode1=101;

   sendMsg(tx);
}

//******************************************************************************
// This sends a message via the tcp client thread

void NetworkThread::sendMsg (Ris::ByteMsg* aMsg)
{
   mUdpMsgThread->sendMsg(aMsg);
}

//******************************************************************************
// This sends a test message via the tcp client thread

void NetworkThread::sendTestMsg()
{
   TestMsg* tMsg = new TestMsg;
   tMsg->mCode1=201;
 
   mUdpMsgThread->sendMsg(tMsg);
}

}//namespace