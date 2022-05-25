/*==============================================================================
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************
#include "stdafx.h"

#include "procoMsgHelper.h"
#include "procoSerialParms.h"

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
   BaseClass::setThreadName("Proc");
   BaseClass::setThreadPriority(Ris::Threads::gPriorities.mProc);
   BaseClass::mTimerPeriod = 100;

   // Initialize qcalls.
   mSessionQCall.bind(this, &PeerThread::executeSession);
   mRxMsgQCall.bind   (this,&PeerThread::executeRxMsg);
   mAbortQCall.bind(this, &PeerThread::executeAbort);

   // Initialize member variables.
   mSerialMsgThread = 0;
   mMsgMonkey = new MsgMonkey;
   mConnectionFlag = false;
   mTPCode = 0;
   mRxCount = 0;
   mTxCount = 0;
   mShowCode = 0;
}

PeerThread::~PeerThread()
{
   if (mSerialMsgThread) delete mSerialMsgThread;
   delete mMsgMonkey;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Show thread info for this thread and for child threads.

void PeerThread::showThreadInfo()
{
   BaseClass::showThreadInfo();
   if (mSerialMsgThread)
   {
      mSerialMsgThread->showThreadInfo();
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Thread init function. This is called by the base class immedidately 
// after the thread starts running. It creates and launches the 
// child SerialMsgThread.

void PeerThread::threadInitFunction()
{
   Trc::write(11, 0, "PeerThread::threadInitFunction");

   // Instance of serial port settings.
   Ris::SerialSettings tSerialSettings;

   tSerialSettings.setPortDevice(gSerialParms.mSerialPortDevice);
   tSerialSettings.setPortSetup(gSerialParms.mSerialPortSetup);
   tSerialSettings.mThreadPriority = Ris::Threads::gPriorities.mSerial;
   tSerialSettings.mRxTimeout = gSerialParms.mSerialRxTimeout;
   tSerialSettings.mMsgMonkey = mMsgMonkey;
   tSerialSettings.mSessionQCall = mSessionQCall;
   tSerialSettings.mRxMsgQCall = mRxMsgQCall;
   tSerialSettings.mTraceIndex = 11;
   Trc::start(11);

   // Create child thread with the settings.
   mSerialMsgThread = new Ris::SerialMsgThread(tSerialSettings);

   // Launch child thread.
   mSerialMsgThread->launchThread(); 

   Trc::write(11, 0, "PeerThread::threadInitFunction done");
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Thread exit function. This is called by the base class immedidately
// before the thread is terminated. It shuts down the child SerialMsgThread.

void PeerThread::threadExitFunction()
{
   Trc::write(11, 0, "PeerThread::threadExitFunction");
   Prn::print(0, "PeerThread::threadExitFunction BEGIN");

   // Shutdown the child thread.
   mSerialMsgThread->shutdownThread();

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
      MsgHelper::initialize(tTxMsg);
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
// Abort function. This is bound to the qcall. It aborts the serial port.

void PeerThread::executeAbort()
{
   mSerialMsgThread->mSerialMsgPort.doAbort();
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// qcall registered to the mSerialMsgThread child thread. It is invoked
// when a session is established or disestablished (when the serial port
// is opened or it is closed because of an error or a disconnection). 

void PeerThread::executeSession(bool aConnected)
{
   if (aConnected)
   {
      Prn::print(Prn::Show1, "PeerThread CONNECTED");
   }
   else
   {
      Prn::print(Prn::Show1, "PeerThread DISCONNECTED");
   }

   mConnectionFlag = aConnected;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// qcall registered to the mSerialMsgThread child thread. It is invoked by
// the child thread when a message is received.
// Based on the receive message type, call one of the specific receive
// message handlers. This is bound to the qcall.

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
      MsgHelper::initialize(tTxMsg, 1000);
      tTxMsg->mCode1 = aRxMsg->mCode1;
      mSerialMsgThread->sendMsg(tTxMsg);
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
// Send a message via mSerialMsgThread:

void PeerThread::sendMsg(BaseMsg* aTxMsg)
{
   mSerialMsgThread->sendMsg(aTxMsg);
   mTxCount++;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Send a message via mSerialMsgThread:

void PeerThread::sendTestMsg()
{
   TestMsg* tMsg = new TestMsg;
   tMsg->mCode1 = 201;

   mSerialMsgThread->sendMsg(tMsg);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace