/*==============================================================================
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************
#include "stdafx.h"

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
   BaseClass::setThreadPriorityHigh();
   BaseClass::mTimerPeriod = 1000;

   // Initialize qcalls.
   mSessionQCall.bind(this, &SerialThread::executeSession);
   mRxStringQCall.bind   (this,&SerialThread::executeRxString);

   // Initialize member variables.
   mSerialStringThread = 0;
   mConnectionFlag = false;
   mTPFlag = false;
}

SerialThread::~SerialThread()
{
   if (mSerialStringThread) delete mSerialStringThread;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Place holder.

void SerialThread::show()
{
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Thread init function. This is called by the base class immedidately 
// after the thread starts running. It creates and launches the 
// child SerialStringThread.

void SerialThread::threadInitFunction()
{
   // Instance of serial port settings.
   Ris::SerialSettings tSerialSettings;

   tSerialSettings.setPortDevice(gSerialParms.mSerialPortDevice);
   tSerialSettings.setPortSetup(gSerialParms.mSerialPortSetup);
   tSerialSettings.mTxTermMode = gSerialParms.mTxTermMode;
   tSerialSettings.mRxTermMode = gSerialParms.mRxTermMode;
   tSerialSettings.mSessionQCall = mSessionQCall;
   tSerialSettings.mRxStringQCall = mRxStringQCall;

   // Create child thread with the settings.
   mSerialStringThread = new Ris::SerialStringThread(tSerialSettings);

   // Launch child thread.
   mSerialStringThread->launchThread(); 
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Thread exit function. This is called by the base class immedidately
// before the thread is terminated. It shuts down the child SerialStringThread.

void SerialThread::threadExitFunction()
{
   Prn::print(0, "SerialThread::threadExitFunction BEGIN");

   // Shutdown the child thread.
   mSerialStringThread->shutdownThread();

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

   char tString[100];
   sprintf(tString, "timer_string %d", aTimerCount);

   // Send a string.
   sendString(new std::string(tString));
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// qcall registered to the mSerialStringThread child thread. It is invoked
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
// qcall registered to the mSerialStringThread child thread. It is invoked by
// the child thread when a string is received.
// Based on the receive string type, call one of the specific receive
// string handlers. This is bound to the qcall.

void SerialThread::executeRxString(std::string* aString)
{
   Prn::print(0, "SerialThread::executeRxString %s", aString->c_str());
   delete aString;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Send a string via mSerialStringThread:

void SerialThread::sendString(std::string* aString)
{
   mSerialStringThread->sendString(aString);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace