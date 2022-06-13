/*==============================================================================
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************
#include "stdafx.h"

#define  _SOMEPORTRESPONDERTHREAD_CPP_
#include "somePortResponderThread.h"

namespace Some
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Constructor.
PortResponderThread::PortResponderThread()
{
   // Set base class variables.
   BaseClass::setThreadName("Serial");
   BaseClass::setThreadPriorityHigh();
   BaseClass::mTimerPeriod = 1000;

   // Initialize qcalls.
   mRxStringQCall.bind   (this,&PortResponderThread::executeRxString);

   // Initialize member variables.
   mUdpStringThread = 0;
   mConnectionFlag = false;
   mTPFlag = false;
}

PortResponderThread::~PortResponderThread()
{
   if (mUdpStringThread) delete mUdpStringThread;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Place holder.

void PortResponderThread::show()
{
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Thread init function. This is called by the base class immedidately 
// after the thread starts running. It creates and launches the 
// child UdpStringThread.

void PortResponderThread::threadInitFunction()
{
   // Instance of network socket settings.
   Ris::Net::Settings tSettings;
   int tPort = 5012;
   tSettings.setLocalPort(tPort);
// tSettings.setRemoteAddress("192.168.1.134", tPort);
   tSettings.setUdpWrapFlag(true);
   tSettings.mUdpWrapFlag = true;
   tSettings.mRxStringQCall = mRxStringQCall;
   tSettings.mTraceIndex = 11;
   Trc::start(11);

   // Create child thread with the settings.
   mUdpStringThread = new Ris::Net::UdpStringThread(tSettings);

   // Launch child thread.
   mUdpStringThread->launchThread(); 
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Thread exit function. This is called by the base class immedidately
// before the thread is terminated. It shuts down the child UdpStringThread.

void PortResponderThread::threadExitFunction()
{
   Prn::print(0, "PortResponderThread::threadExitFunction BEGIN");

   // Shutdown the child thread.
   mUdpStringThread->shutdownThread();

   Prn::print(0, "PortResponderThread::threadExitFunction END");
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Thread shutdown function. This calls the base class shutdownThread
// function to terminate the thread. This executes in the context of
// the calling thread.

void PortResponderThread::shutdownThread()
{
   Prn::print(0, "PortResponderThread::shutdownThread BEGIN");
   BaseClass::shutdownThread();
   Prn::print(0, "PortResponderThread::shutdownThread END");
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Execute periodically. This is called by the base class timer.

void PortResponderThread::executeOnTimer(int aTimerCount)
{
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// qcall registered to the mUdpStringThread child thread. It is invoked by
// the child thread when a string is received.
// Based on the receive string type, call one of the specific receive
// string handlers. This is bound to the qcall.

void PortResponderThread::executeRxString(std::string* aString)
{
   Prn::print(Prn::Show1, "PortResponderThread::executeRxString %s", aString->c_str());
   delete aString;

   sendString(new std::string("Mira Hello"));
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Send a string via mUdpStringThread:

void PortResponderThread::sendString(std::string* aString)
{
   mUdpStringThread->sendString(aString);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace