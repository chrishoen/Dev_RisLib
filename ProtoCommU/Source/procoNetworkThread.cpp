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
// Constructor.

NetworkThread::NetworkThread()
{
   // Set base class thread priority.
   BaseClass::setThreadPriorityHigh();

   // Set timer period.
   BaseClass::mTimerPeriod = 1000;

   // Initialize QCalls.
   mRxMsgQCall.bind(this, &NetworkThread::executeRxMsg);

   // Initialize variables.
   mMonkeyCreator.configure(gSettings.mMyAppNumber);
   mUdpMsgThread = 0;
   mTPFlag = false;
   mStatusCount1=0;
   mStatusCount2=0;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

NetworkThread::~NetworkThread()
{
   delete mUdpMsgThread;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Thread exit function, base class overload.

void NetworkThread::threadInitFunction()
{
   Prn::print(Prn::ThreadInit1, "SerialThread::threadInitFunction");

   // Instance of network socket settings.
   Ris::Net::Settings tSettings;

   tSettings.setLocalIpAddr(gSettings.mMyUdpIPAddress);
   tSettings.mLocalIpPort = gSettings.mMyUdpPort;
   tSettings.setRemoteIpAddr(gSettings.mOtherUdpIPAddress);
   tSettings.mRemoteIpPort = gSettings.mOtherUdpPort;
   tSettings.mMonkeyCreator = &mMonkeyCreator;
   tSettings.mRxMsgQCall = mRxMsgQCall;

   // Create child thread.
   mUdpMsgThread = new Ris::Net::UdpMsgThread(tSettings);

   // Launch child thread.
   mUdpMsgThread->launchThread();
}

//******************************************************************************
//******************************************************************************
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
//******************************************************************************
//******************************************************************************
// QCall registered to mUdpMsgThread. This is invoked by mUdpMsgThread
// when it receives a message. It process the received messages.

void NetworkThread::executeRxMsg(Ris::ByteContent* aMsg)
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
//******************************************************************************
//******************************************************************************
// Message handler for TestMsg.

void NetworkThread::processRxMsg(ProtoComm::TestMsg*  aMsg)
{
   MsgHelper::show(aMsg);
   delete aMsg;
}

//******************************************************************************
//******************************************************************************
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
//******************************************************************************
//******************************************************************************
// Rx message handler - StatusResponseMsg

void NetworkThread::processRxMsg(ProtoComm::StatusResponseMsg* aMsg)
{
   MsgHelper::show(aMsg);
   delete aMsg;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Rx message handler - DataMsg

void NetworkThread::processRxMsg(ProtoComm::DataMsg* aMsg)
{
   MsgHelper::show(aMsg);
   delete aMsg;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Periodic timer execution, base class overload.

void NetworkThread::executeOnTimer(int aTimerCount)
{
   if (!mTPFlag) return;

   // Send a test message.
   ProtoComm::TestMsg* tx = new ProtoComm::TestMsg;
   tx->mCode1 = aTimerCount;
   mUdpMsgThread->sendMsg(tx);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Send a message via the socket thread.

void NetworkThread::sendMsg (ProtoComm::BaseMsg* aMsg)
{
   mUdpMsgThread->sendMsg(aMsg);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Send a message via the socket thread.

void NetworkThread::sendTestMsg()
{
   ProtoComm::TestMsg* tMsg = new ProtoComm::TestMsg;
 
   mUdpMsgThread->sendMsg(tMsg);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace