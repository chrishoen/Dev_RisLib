/*==============================================================================
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************
#include "stdafx.h"

#include "fcomSettings.h"

#define  _FCOMSERIALTHREAD_CPP_
#include "fcomSerialThread.h"

namespace FCom
{

//******************************************************************************
//******************************************************************************
//******************************************************************************

SerialThread::SerialThread()
{
   // Set base class thread priority.
   BaseClass::setThreadPriorityHigh();

   // Set timer period.
   BaseClass::mTimerPeriod = 1000;
   if (gSettings.mThreadTimerPeriod)BaseClass::mTimerPeriod = gSettings.mThreadTimerPeriod;

   // Initialize QCalls.
   mRxMsgQCall.bind   (this,&SerialThread::executeRxMsg);
   mTest1QCall.bind   (this,&SerialThread::executeTest1);

   // Initialize variables.
   mSerialMsgThread = 0;
   mTPFlag = false;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

SerialThread::~SerialThread()
{
   if (mSerialMsgThread) delete mSerialMsgThread;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void SerialThread::show()
{
   Prn::print(0, "SerialMsgPort %d %d",
      mSerialMsgThread->mSerialMsgPort.mHeaderAllCount,
      mSerialMsgThread->mSerialMsgPort.mHeaderOneCount);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void SerialThread::threadInitFunction()
{
   Prn::print(Prn::ThreadInit1, "SerialThread::launch");

   // Instance of serial port settings.
   Ris::SerialSettings tSerialSettings;

   tSerialSettings.setPortDevice(gSettings.mPortDevice);
   tSerialSettings.setPortSetup(gSettings.mPortSetup);
   tSerialSettings.mRxTimeout     = gSettings.mPortTimeout;
   tSerialSettings.mMonkeyCreator = &mMonkeyCreator;
   tSerialSettings.mRxMsgQCall    = mRxMsgQCall;

   // Create child thread.
   mSerialMsgThread = new Ris::SerialMsgThread(tSerialSettings);

   // Launch child thread.
   mSerialMsgThread->launchThread(); 
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
// Execute the call in the context of the long thread.

void SerialThread::executeTest1(int  aCode)
{
   FCom::TestMsg* msg = new FCom::TestMsg;
   msg->mCode1 = 201;

   mSerialMsgThread->sendMsg(msg);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// executeOnTimer

void SerialThread::executeOnTimer(int aTimerCount)
{
   if (!mTPFlag) return;

   EchoRequestMsg* tMsg = (EchoRequestMsg*)FCom::createMsg(cEchoRequestMsg);
   tMsg->mCode1 = aTimerCount;
   sendMsg(tMsg);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// QCall

void SerialThread::executeRxMsg(Ris::ByteContent* aMsg)
{
   if(!aMsg) return;

   // Put the message to the message processor
   FCom::BaseMsg* tMsg = (FCom::BaseMsg*)aMsg;

   // Message jump table based on message type.
   // Calls corresponding specfic message handler method.
   switch (tMsg->mMessageType)
   {
      case FCom::cTestMsg :
         processRxMsg((FCom::TestMsg*)tMsg);
         break;
      case FCom::cEchoResponseMsg:
         processRxMsg((FCom::EchoResponseMsg*)tMsg);
         break;
      case FCom::cSampleMsg:
         processRxMsg((FCom::SampleMsg*)tMsg);
         break;
      default :
         Prn::print(Prn::ThreadRun1, "SerialThread::processRxMsg %d",tMsg->mMessageType);
         destroyMsg(tMsg);
         break;
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Rx message handler - TestMsg

void SerialThread::processRxMsg(FCom::TestMsg* aMsg)
{
   Prn::print(Prn::ThreadRun1, "SerialThread::processRxMsg_TestMsg %d",aMsg->mCode1);
   destroyMsg(aMsg);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Rx message handler - SampleMsg

void SerialThread::processRxMsg(FCom::SampleMsg* aMsg)
{
   Prn::print(Prn::ViewRun1, "SampleMsg %d", aMsg->mTimerCount);
   destroyMsg(aMsg);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Rx message handler - EchoResponseMsg

void SerialThread::processRxMsg(FCom::EchoResponseMsg* aMsg)
{
   Prn::print(Prn::ThreadRun1, "EchoResponseMsg %d", aMsg->mCode1);
   destroyMsg(aMsg);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Sends a message via the serial thread.

void SerialThread::sendMsg (FCom::BaseMsg* aTxMsg)
{
   mSerialMsgThread->sendMsg(aTxMsg);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Send a message via the serial thread.

void SerialThread::sendTestMsg()
{
   TestMsg* tMsg = (TestMsg*)FCom::createMsg(FCom::cTestMsg);
   tMsg->mCode1=201;
 
   mSerialMsgThread->sendMsg(tMsg);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Send a message via the serial thread.

void SerialThread::sendSettingsMsg()
{
   // Create settings message.
   SettingsMsg* tMsg = (SettingsMsg*)FCom::createMsg(cSettingsMsg);

   // Copy settings variables.
   tMsg->mEnable = gSettings.mCSenEnable;
   tMsg->mTimerModulo = gSettings.mCSenTimerModulo;

   // Send settings message.
   mSerialMsgThread->sendMsg(tMsg);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace