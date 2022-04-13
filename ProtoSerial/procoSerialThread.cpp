/*==============================================================================
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************
#include "stdafx.h"

#include "procoMsgHelper.h"
#include "procoSerialParms.h"

#define  _PROCOSERIALTHREAD_CPP_
#include "procoSerialThread.h"

namespace ProtoComm
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Constructor.

SerialThread::SerialThread()
{
   // Set base class variables.
   BaseClass::setThreadName("Serial");
   BaseClass::setThreadPriority(Ris::Threads::gPriorities.mHigh);
   BaseClass::mTimerPeriod = 1000;

   // Initialize qcalls.
   mSessionQCall.bind(this, &SerialThread::executeSession);
   mRxMsgQCall.bind   (this,&SerialThread::executeRxMsg);
   mAbortQCall.bind(this, &SerialThread::executeAbort);

   // Initialize member variables.
   mSerialMsgThread = 0;
   mConnectionFlag = false;
   mTPFlag = false;
}

SerialThread::~SerialThread()
{
   if (mSerialMsgThread) delete mSerialMsgThread;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Place holder.

void SerialThread::show()
{
   Prn::print(0, "SerialMsgPort %d %d",
      mSerialMsgThread->mSerialMsgPort.mHeaderAllCount,
      mSerialMsgThread->mSerialMsgPort.mHeaderOneCount);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Thread init function. This is called by the base class immedidately 
// after the thread starts running. It creates and launches the 
// child SerialMsgThread.

void SerialThread::threadInitFunction()
{
   // Instance of serial port settings.
   Ris::SerialSettings tSerialSettings;

   tSerialSettings.setPortDevice(gSerialParms.mSerialPortDevice);
   tSerialSettings.setPortSetup(gSerialParms.mSerialPortSetup);
   tSerialSettings.mThreadPriority = Ris::Threads::gPriorities.mSerial;
   tSerialSettings.mRxTimeout = gSerialParms.mSerialRxTimeout;
   tSerialSettings.mMonkeyCreator = &mMonkeyCreator;
   tSerialSettings.mSessionQCall = mSessionQCall;
   tSerialSettings.mRxMsgQCall = mRxMsgQCall;
   tSerialSettings.mTraceIndex = 11;
   Trc::start(11);
   // Create child thread with the settings.
   mSerialMsgThread = new Ris::SerialMsgThread(tSerialSettings);

   // Launch child thread.
   mSerialMsgThread->launchThread(); 
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Thread exit function. This is called by the base class immedidately
// before the thread is terminated. It shuts down the child SerialMsgThread.

void SerialThread::threadExitFunction()
{
   Prn::print(0, "SerialThread::threadExitFunction BEGIN");

   // Shutdown the child thread.
   mSerialMsgThread->shutdownThread();

   Prn::print(0, "SerialThread::threadExitFunction END");
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Thread shutdown function. This calls the base class shutdownThread
// function to terminate the thread. This executes in the context of
// the calling thread.

void SerialThread::shutdownThread()
{
   Prn::print(0, "SerialThread::shutdownThread BEGIN");
   BaseClass::shutdownThread();
   Prn::print(0, "SerialThread::shutdownThread END");
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Execute periodically. This is called by the base class timer.

void SerialThread::executeOnTimer(int aTimerCount)
{
   if (!mTPFlag) return;

   EchoRequestMsg* tMsg = new EchoRequestMsg;
   MsgHelper::initialize(tMsg, 1000);
   tMsg->mCode1 = aTimerCount;
   sendMsg(tMsg);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Abort function. This is bound to the qcall. It aborts the serial port.

void SerialThread::executeAbort()
{
   mSerialMsgThread->mSerialMsgPort.doAbort();
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// qcall registered to the mSerialMsgThread child thread. It is invoked
// when a session is established or disestablished (when the serial port
// is opened or it is closed because of an error or a disconnection). 

void SerialThread::executeSession(bool aConnected)
{
   if (aConnected)
   {
      Prn::print(Prn::Show1, "SerialThread CONNECTED");
   }
   else
   {
      Prn::print(Prn::Show1, "SerialThread DISCONNECTED");
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

void SerialThread::executeRxMsg(Ris::ByteContent* aMsg)
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
   default:
      Prn::print(Prn::Show1, "SerialThread::executeServerRxMsg ??? %d", tMsg->mMessageType);
      delete tMsg;
      break;
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Message handler - TestMsg.

void SerialThread::processRxMsg(ProtoComm::TestMsg*  aMsg)
{
   MsgHelper::show(Prn::Show1, aMsg);
   delete aMsg;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Rx message handler - EchoRequestMsg.

void SerialThread::processRxMsg(ProtoComm::EchoRequestMsg* aRxMsg)
{
   if (true)
   {
      ProtoComm::EchoResponseMsg* tTxMsg = new ProtoComm::EchoResponseMsg;
      MsgHelper::initialize(tTxMsg, 1000);
      tTxMsg->mCode1 = aRxMsg->mCode1;
      mSerialMsgThread->sendMsg(tTxMsg);
   }

   MsgHelper::show(Prn::Show1, aRxMsg);
   delete aRxMsg;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Rx message handler - EchoResponseMsg.

void SerialThread::processRxMsg(ProtoComm::EchoResponseMsg* aMsg)
{
   MsgHelper::show(Prn::Show1, aMsg);
   delete aMsg;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Rx message handler - DataMsg.

void SerialThread::processRxMsg(ProtoComm::DataMsg* aMsg)
{
   MsgHelper::show(Prn::Show1, aMsg);
   delete aMsg;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Send a message via mSerialMsgThread:

void SerialThread::sendMsg(BaseMsg* aTxMsg)
{
   mSerialMsgThread->sendMsg(aTxMsg);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Send a message via mSerialMsgThread:

void SerialThread::sendTestMsg()
{
   TestMsg* tMsg = new TestMsg;
   tMsg->mCode1 = 201;

   mSerialMsgThread->sendMsg(tMsg);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace