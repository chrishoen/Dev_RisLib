/*==============================================================================
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************
#include "stdafx.h"

#include "procoMsgHelper.h"
#include "procoUdpParms.h"

#define  _PROCOPEERTHREAD_CPP_
#include "procoPeerThread.h"

namespace ProtoComm
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Constructor.

PeerThread::PeerThread()
{
   // Set base class variables.
   BaseClass::setThreadName("Peer");
   BaseClass::setThreadPriority(Ris::Threads::gPriorities.mProc);
   BaseClass::mTimerPeriod = 100;

   // Initialize qcalls.
   mRxMsgQCall.bind   (this,&PeerThread::executeRxMsg);

   // Initialize member variables.
   mMsgThread = 0;
   mMsgMonkey = new MsgMonkey;
   mTPCode = 0;
   mRxCount = 0;
   mTxCount = 0;
   mShowCode = 0;
}

PeerThread::~PeerThread()
{
   if (mMsgThread)
   {
      delete mMsgThread;
      mMsgThread = 0;
   }
   delete mMsgMonkey;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Show thread info for this thread and for child threads.

void PeerThread::showThreadInfo()
{
   BaseClass::showThreadInfo();
   if (mMsgThread)
   {
      mMsgThread->showThreadInfo();
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Thread init function. This is called by the base class immedidately 
// after the thread starts running. It creates and launches the child
// message thread.

void PeerThread::threadInitFunction()
{
   Trc::start(11);
   Trc::write(11, 0, "PeerThread::threadInitFunction");

   // Instance of network socket settings.
   Ris::Net::Settings tSettings;

   tSettings.setLocalAddress(gUdpParms.mMyUdpAddress, gUdpParms.mMyUdpPort);
   tSettings.setRemoteAddress(gUdpParms.mOtherUdpAddress, gUdpParms.mOtherUdpPort);
   tSettings.setUdpWrapFlag(gUdpParms.mUdpWrapFlag);
   tSettings.mMsgMonkey = mMsgMonkey;
   tSettings.mRxMsgQCall = mRxMsgQCall;
   tSettings.mTraceIndex = 11;

   // Create the child thread according to the settings.
   mMsgThread = new Ris::Net::UdpMsgThread(tSettings);

   // Launch the child thread.
   mMsgThread->launchThread();

   Trc::write(11, 0, "PeerThread::threadInitFunction done");
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Thread exit function. This is called by the base class immedidately
// before the thread is terminated. It shuts down the child message
// thread.

void PeerThread::threadExitFunction()
{
   Trc::write(11, 0, "PeerThread::threadExitFunction");
   Prn::print(0, "PeerThread::threadExitFunction BEGIN");

   // Shutdown the child thread.
   mMsgThread->shutdownThread();

   Prn::print(0, "PeerThread::threadExitFunction END");
   Trc::write(11, 0, "PeerThread::threadExitFunction done");
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Thread shutdown function. This calls the base class shutdownThread
// function to terminate the thread. This executes in the context of
// the calling thread.

void PeerThread::shutdownThread()
{
   Trc::write(11, 0, "PeerThread::shutdownThread");
   Prn::print(0, "PeerThread::shutdownThread BEGIN");
   BaseClass::shutdownThread();
   Prn::print(0, "PeerThread::shutdownThread END");
   Trc::write(11, 0, "PeerThread::shutdownThread done");
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Execute periodically. This is called by the base class timer.

void PeerThread::executeOnTimer(int aTimerCount)
{
   if (mTPCode == 1)
   {
      EchoRequestMsg* tTxMsg = new EchoRequestMsg;
      tTxMsg->initialize(1000);
      sendMsg(tTxMsg);
   }
   else if (mTPCode == 2)
   {
      ByteBlobMsg* tTxMsg = new ByteBlobMsg;
      MsgHelper::initialize2(tTxMsg);
      sendMsg(tTxMsg);
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// qcall registered to the mMsgThread child thread. It is invoked by
// the child thread when a message is received. Based on the receive
// message type, call one of the following specific receive message
// handlers. This is bound to the qcall.

void PeerThread::executeRxMsg(Ris::ByteContent* aMsg)
{
   ProtoComm::BaseMsg* tMsg = (ProtoComm::BaseMsg*)aMsg;

   // Message jump table based on message type.
   // Call corresponding specfic message handler method.
   switch (tMsg->mMessageType)
   {
   case ProtoComm::MsgIdT::cTestMsg:
      processRxMsg((ProtoComm::TestMsg*)tMsg);
      break;
   case ProtoComm::MsgIdT::cEchoRequestMsg:
      processRxMsg((ProtoComm::EchoRequestMsg*)tMsg);
      break;
   case ProtoComm::MsgIdT::cEchoResponseMsg:
      processRxMsg((ProtoComm::EchoResponseMsg*)tMsg);
      break;
   case ProtoComm::MsgIdT::cDataMsg:
      processRxMsg((ProtoComm::DataMsg*)tMsg);
      break;
   case ProtoComm::MsgIdT::cByteBlobMsg:
      processRxMsg((ProtoComm::ByteBlobMsg*)tMsg);
      break;
   default:
      Prn::print(Prn::Show1, "PeerThread::executeServerRxMsg ??? %d", tMsg->mMessageType);
      delete tMsg;
      break;
   }
   mRxCount++;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Message handler - TestMsg.

void PeerThread::processRxMsg(ProtoComm::TestMsg*  aRxMsg)
{
   MsgHelper::show(Prn::Show1, aRxMsg);
   delete aRxMsg;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Rx message handler - EchoRequestMsg.

void PeerThread::processRxMsg(ProtoComm::EchoRequestMsg* aRxMsg)
{
   if (true)
   {
      ProtoComm::EchoResponseMsg* tTxMsg = new ProtoComm::EchoResponseMsg;
      tTxMsg->initialize(1000);
      tTxMsg->mCode1 = aRxMsg->mCode1;
      mMsgThread->sendMsg(tTxMsg);
   }
   if (mShowCode == 3)
   {
      MsgHelper::show(Prn::Show1, aRxMsg);
   }
   delete aRxMsg;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Rx message handler - EchoResponseMsg.

void PeerThread::processRxMsg(ProtoComm::EchoResponseMsg* aRxMsg)
{
   if (mShowCode == 3)
   {
      MsgHelper::show(Prn::Show1, aRxMsg);
   }
   delete aRxMsg;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Rx message handler - DataMsg.

void PeerThread::processRxMsg(ProtoComm::DataMsg* aRxMsg)
{
   if (mShowCode == 3)
   {
      MsgHelper::show(Prn::Show1, aRxMsg);
   }
   delete aRxMsg;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Rx message handler - ByteBlobMsg.

void PeerThread::processRxMsg(ProtoComm::ByteBlobMsg* aRxMsg)
{
   if (mShowCode == 3)
   {
      MsgHelper::show(Prn::Show1, aRxMsg);
   }
   delete aRxMsg;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Send a message via the child message thread:

void PeerThread::sendMsg(BaseMsg* aTxMsg)
{
   mMsgThread->sendMsg(aTxMsg);
   mTxCount++;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Send a message via the child message thread:

void PeerThread::sendTestMsg()
{
   TestMsg* tMsg = new TestMsg;
   tMsg->mCode1 = 201;

   mMsgThread->sendMsg(tMsg);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace