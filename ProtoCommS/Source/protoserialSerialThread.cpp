/*==============================================================================
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************
#include "stdafx.h"

#include "protoserialSettings.h"
#include "protoserialMsgHelper.h"

#define  _PROCOSERIALTHREAD_CPP_
#include "protoserialSerialThread.h"

namespace ProtoSerial
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Constructor.

SerialThread::SerialThread()
{
   BaseClass::mTimerPeriod = 1000;

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

void SerialThread::threadInitFunction()
{
   Prn::print(Prn::ThreadInit1, "SerialThread::threadInitFunction BEGIN");

   // Configure the message monkey.
   mMonkeyCreator.configure(gSettings.mMyAppNumber);

   // Configure the message thread.
   mSerialMsgThread->configure(
      &mMonkeyCreator,
      gSettings.mSerialPortNumber,
      gSettings.mSerialPortSetup,
      gSettings.mSerialRxTimeout,
      &mRxMsgQCall);

   // Launch child thread
   mSerialMsgThread->launchThread();
   
   Prn::print(Prn::ThreadInit1, "SerialThread::threadInitFunction END");
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

void SerialThread::executeOnTimer(int aTimerCount)
{
   if (gSettings.mMyAppNumber == 401)
   {
      Prn::print(Prn::ThreadRun1, "SerialThread::executeOnTimer");
   }

   return;

   ProtoSerial::TestMsg* tx = new ProtoSerial::TestMsg;
   tx->mCode1=101;

   mSerialMsgThread->sendMsg(tx);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// QCall

void SerialThread::executeRxMessage(Ris::ByteContent* aMsg)
{
   ProtoSerial::BaseMsg* tMsg = (ProtoSerial::BaseMsg*)aMsg;

   // Message jump table based on message type.
   // Calls corresponding specfic message handler method.
   switch (tMsg->mMessageType)
   {
      case ProtoSerial::MsgIdT::cTestMsg :
         processRxMsg((ProtoSerial::TestMsg*)tMsg);
         break;
      case ProtoSerial::MsgIdT::cStatusRequestMsg :
         processRxMsg((ProtoSerial::StatusRequestMsg*)tMsg);
         break;
      case ProtoSerial::MsgIdT::cStatusResponseMsg :
         processRxMsg((ProtoSerial::StatusResponseMsg*)tMsg);
         break;
      case ProtoSerial::MsgIdT::cDataMsg :
         processRxMsg((ProtoSerial::DataMsg*)tMsg);
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

void SerialThread::processRxMsg(ProtoSerial::TestMsg*  aMsg)
{
   MsgHelper::show(aMsg);
   delete aMsg;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Rx message handler - StatusRequestMsg

void SerialThread::processRxMsg(ProtoSerial::StatusRequestMsg* aMsg)
{
   if (true)
   {
      ProtoSerial::StatusResponseMsg* tMsg = new ProtoSerial::StatusResponseMsg;
      mSerialMsgThread->sendMsg(tMsg);
   }

   MsgHelper::show(aMsg);
   delete aMsg;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Rx message handler - StatusResponseMsg

void SerialThread::processRxMsg(ProtoSerial::StatusResponseMsg* aMsg)
{
   MsgHelper::show(aMsg);
   delete aMsg;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Rx message handler - DataMsg

void SerialThread::processRxMsg(ProtoSerial::DataMsg* aMsg)
{
   MsgHelper::show(aMsg);
   delete aMsg;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// This sends a message via the tcp client thread

void SerialThread::sendMsg (ProtoSerial::BaseMsg* aMsg)
{
   mSerialMsgThread->sendMsg(aMsg);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// This sends a test message via the tcp client thread

void SerialThread::sendTestMsg()
{
   ProtoSerial::TestMsg* tMsg = new ProtoSerial::TestMsg;
 
   mSerialMsgThread->sendMsg(tMsg);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace