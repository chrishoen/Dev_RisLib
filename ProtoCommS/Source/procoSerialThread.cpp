/*==============================================================================
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************
#include "stdafx.h"

#include "risSockets.h"
#include "procoSettings.h"
#include "procoMsgHelper.h"

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
   mStatusCount1=0;
   mStatusCount2=0;

   mSerialMsgThread = new Ris::SerialMsgThread;

   // Initialize QCalls
   mRxMsgQCall.bind   (this,&SerialThread::executeRxMessage);
}

SerialThread::~SerialThread()
{
   delete mSerialMsgThread;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// This configures the serial port.

void SerialThread::configure()
{
   Prn::print(Prn::ThreadInit1, "SerialThread::configure");

   // Configure the message monkey.
   mMonkeyCreator.configure(gSettings.mMyAppNumber);

   // Configure the message thread.
   mSerialMsgThread->configure(
      &mMonkeyCreator,
      gSettings.mSerialPortNumber,
      gSettings.mSerialPortSetup,
      gSettings.mSerialRxTimeout,
      &mRxMsgQCall);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void SerialThread::launchThread()
{
   Prn::print(Prn::ThreadInit1, "SerialThread::launch");

   // Launch child thread
   mSerialMsgThread->launchThread(); 
   
   // Launch this thread
   BaseClass::launchThread();
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Thread exit function, base class overload.

void  SerialThread::threadExitFunction()
{
   Prn::print(Prn::ThreadInit1, "SerialThread::threadExitFunction");

   // Shutdown the tcp client thread
   mSerialMsgThread->shutdownThread();

   // Base class exit
   BaseClass::threadExitFunction();
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// QCall

void SerialThread::executeRxMessage(Ris::ByteContent* aMsg)
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
         Prn::print(Prn::ThreadRun1, "SerialThread::executeServerRxMsg ??? %d",tMsg->mMessageType);
         delete tMsg;
         break;
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Message handler for TestMsg.

void SerialThread::processRxMsg(ProtoComm::TestMsg*  aMsg)
{
   MsgHelper::show(aMsg);
   delete aMsg;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Rx message handler - StatusRequestMsg

void SerialThread::processRxMsg(ProtoComm::StatusRequestMsg* aMsg)
{
   if (true)
   {
      ProtoComm::StatusResponseMsg* tMsg = new ProtoComm::StatusResponseMsg;
      mSerialMsgThread->sendMsg(tMsg);
   }

   MsgHelper::show(aMsg);
   delete aMsg;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Rx message handler - StatusResponseMsg

void SerialThread::processRxMsg(ProtoComm::StatusResponseMsg* aMsg)
{
   MsgHelper::show(aMsg);
   delete aMsg;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Rx message handler - DataMsg

void SerialThread::processRxMsg(ProtoComm::DataMsg* aMsg)
{
   MsgHelper::show(aMsg);
   delete aMsg;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// QCall

void SerialThread::executeOnTimer(int aTimerCount)
{
   Prn::print(Prn::ThreadRun2, "SerialThread::executeRxMessage");

   return;

   ProtoComm::TestMsg* tx = new ProtoComm::TestMsg;
   tx->mCode1=101;

   mSerialMsgThread->sendMsg(tx);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// This sends a message via the tcp client thread

void SerialThread::sendMsg (ProtoComm::BaseMsg* aMsg)
{
   mSerialMsgThread->sendMsg(aMsg);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// This sends a test message via the tcp client thread

void SerialThread::sendTestMsg()
{
   ProtoComm::TestMsg* tMsg = new ProtoComm::TestMsg;
 
   mSerialMsgThread->sendMsg(tMsg);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace