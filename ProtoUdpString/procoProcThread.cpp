/*==============================================================================
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************
#include "stdafx.h"

#include "procoUdpSettings.h"

#define  _PROCOPROCTHREAD_CPP_
#include "procoProcThread.h"

namespace ProtoComm
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Constructor.
ProcThread::ProcThread()
{
   // Set base class variables.
   BaseClass::setThreadName("Udp");
   BaseClass::setThreadPriorityHigh();
   BaseClass::mTimerPeriod = 1000;

   // Initialize qcalls.
   mRxStringQCall.bind   (this,&ProcThread::executeRxString);

   // Initialize member variables.
   mUdpStringThread = 0;
   mConnectionFlag = false;
   mTxCount = 0;
   mRxCount = 0;
   mTPFlag = false;
}

ProcThread::~ProcThread()
{
   if (mUdpStringThread) delete mUdpStringThread;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Place holder.

void ProcThread::show()
{
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Thread init function. This is called by the base class immedidately 
// after the thread starts running. It creates and launches the 
// child UdpStringThread.

void ProcThread::threadInitFunction()
{
   Trc::start(11);
   Trc::write(11, 0, "ProcThread::threadInitFunction");

   // Instance of network socket settings.
   Ris::Net::Settings tSettings;

   tSettings.setLocalAddress(gUdpSettings.mMyUdpAddress, gUdpSettings.mMyUdpPort);
   tSettings.setRemoteAddress(gUdpSettings.mOtherUdpAddress, gUdpSettings.mOtherUdpPort);
   tSettings.setUdpWrapFlag(gUdpSettings.mUdpWrapFlag);
   tSettings.setUdpBroadcast(gUdpSettings.mUdpBroadcast);
   tSettings.mRxStringQCall = mRxStringQCall;
   tSettings.mTraceIndex = 11;

   // Create the child thread with the settings.
   mUdpStringThread = new Ris::Net::UdpStringThread(tSettings);

   // Launch the child thread.
   mUdpStringThread->launchThread();

   Trc::write(11, 0, "ProcThread::threadInitFunction done");
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Thread exit function. This is called by the base class immedidately
// before the thread is terminated. It shuts down the child UdpStringThread.

void ProcThread::threadExitFunction()
{
   Prn::print(0, "ProcThread::threadExitFunction BEGIN");

   // Shutdown the child thread.
   mUdpStringThread->shutdownThread();

   Prn::print(0, "ProcThread::threadExitFunction END");
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Thread shutdown function. This calls the base class shutdownThread
// function to terminate the thread. This executes in the context of
// the calling thread.

void ProcThread::shutdownThread()
{
   Prn::print(0, "ProcThread::shutdownThread BEGIN");
   BaseClass::shutdownThread();
   Prn::print(0, "ProcThread::shutdownThread END");
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Execute periodically. This is called by the base class timer.

void ProcThread::executeOnTimer(int aTimerCount)
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
// qcall registered to the mUdpStringThread child thread. It is invoked by
// the child thread when a string is received.
// Based on the receive string type, call one of the specific receive
// string handlers. This is bound to the qcall.

void ProcThread::executeRxString(std::string* aString)
{
   Prn::print(Prn::Show1, "RxString %4d %s", mRxCount++, aString->c_str());
   delete aString;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Send a string via mUdpStringThread:

void ProcThread::sendString(std::string* aString)
{
   mUdpStringThread->sendString(aString);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace