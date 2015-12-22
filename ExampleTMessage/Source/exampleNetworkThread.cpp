/*==============================================================================
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************
#include "risSockets.h"
#include "prnPrint.h"
#include "exampleSettings.h"
#include "exampleTMessageHelper.h"

#define  _EXAMPLENETWORKTHREAD_CPP_
#include "exampleNetworkThread.h"


namespace Example
{

//******************************************************************************
//******************************************************************************
//******************************************************************************

NetworkThread::NetworkThread()
{
   mStatusCount1=0;
   mStatusCount2=0;

   mUdpTMessageThread = new Ris::Net::UdpTMessageThread;

   // Initialize QCalls
   mRxMessageQCall.bind   (this,&NetworkThread::executeRxMessage);
}

//******************************************************************************

NetworkThread::~NetworkThread()
{
   delete mUdpTMessageThread;
}

//******************************************************************************
// This configures members

void NetworkThread::configure()
{
   Prn::print(Prn::ThreadInit1, "NetworkThread::configure");

   //---------------------------------------------------------------------------
   // Configure socket thread

   mUdpTMessageThread->configure(
      gSettings.mMyUdpIPAddress,
      gSettings.mMyUdpPort,
      gSettings.mOtherUdpIPAddress,
      gSettings.mOtherUdpPort,
      &mMsgCopier,
      &mRxMessageQCall);

}

//******************************************************************************

void NetworkThread::launchThread()
{
   Prn::print(Prn::ThreadInit1, "NetworkThread::launch");

   // Launch child thread
   mUdpTMessageThread->launchThread(); 
   
   // Launch this thread
   BaseClass::launchThread();
}

//******************************************************************************
// Thread exit function, base class overload.
void  NetworkThread::threadExitFunction()
{
   Prn::print(Prn::ThreadInit1, "NetworkThread::threadExitFunction");

   // Shutdown the tcp client thread
   mUdpTMessageThread->shutdownThread();

   // Base class exit
   BaseClass::threadExitFunction();
}

//******************************************************************************
// QCall

void NetworkThread::executeRxMessage(Ris::ByteTMessage* aMsg)
{
   // Message jump table based on message type.
   // Calls corresponding specfic message handler method.
   switch (aMsg->mTMessageType)
   {
      case TypeIdT::cTestMsg :
         processRxMessage((TestMsg*)aMsg);
         break;
      case TypeIdT::cStatusMsg :
         processRxMessage((StatusMsg*)aMsg);
         break;
      case TypeIdT::cData1Msg :
         processRxMessage((Data1Msg*)aMsg);
         break;
      case TypeIdT::cData2Msg :
         processRxMessage((Data2Msg*)aMsg);
         break;
      case TypeIdT::cData3Msg :
         processRxMessage((Data3Msg*)aMsg);
         break;
      case TypeIdT::cData4Msg :
         processRxMessage((Data4Msg*)aMsg);
         break;
      default :
         Prn::print(Prn::ThreadRun1, "NetworkThread::executeServerRxTMessage ??? %d",aMsg->mTMessageType);
         delete aMsg;
         break;
   }
}

//******************************************************************************
// Message handler

void NetworkThread::processRxMessage(TestMsg* aMsg)
{
   Prn::print(Prn::ThreadRun1, "NetworkThread::processRxMessage_TestMsg" );

   TMessageHelper::show(aMsg);

   delete aMsg;
}

//******************************************************************************
// Message handler

void NetworkThread::processRxMessage(StatusMsg* aMsg)
{
   Prn::print(Prn::ThreadRun1, "NetworkThread::processRxMessage_StatusMsg %d",mStatusCount1++);

   TMessageHelper::show(aMsg);

   delete aMsg;
}

//******************************************************************************
// Message handler

void NetworkThread::processRxMessage(Data1Msg* aMsg)
{
   Prn::print(Prn::ThreadRun1, "NetworkThread::processRxMessage_Data1Msg");

   TMessageHelper::show(aMsg);

   delete aMsg;
}

//******************************************************************************
// Message handler

void NetworkThread::processRxMessage(Data2Msg* aMsg)
{
   Prn::print(Prn::ThreadRun1, "NetworkThread::processRxMessage_Data2Msg");

   TMessageHelper::show(aMsg);

   delete aMsg;
}

//******************************************************************************
// Message handler

void NetworkThread::processRxMessage(Data3Msg* aMsg)
{
   Prn::print(Prn::ThreadRun1, "NetworkThread::processRxMessage_Data3Msg");

   TMessageHelper::show(aMsg);

   delete aMsg;
}

//******************************************************************************
// Message handler

void NetworkThread::processRxMessage(Data4Msg* aMsg)
{
   Prn::print(Prn::ThreadRun1, "NetworkThread::processRxMessage_Data4Msg");

   TMessageHelper::show(aMsg);

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

   sendMessage(tx);
}

//******************************************************************************
// This sends a message via the tcp client thread

void NetworkThread::sendMessage (Ris::ByteTMessage* aMsg)
{
   mUdpTMessageThread->sendMessage(aMsg);
}

//******************************************************************************
// This sends a test message via the tcp client thread

void NetworkThread::sendTestMsg()
{
   TestMsg* tTMessage = new TestMsg;
   tTMessage->mCode1=201;
 
   mUdpTMessageThread->sendMessage(tTMessage);
}

}//namespace