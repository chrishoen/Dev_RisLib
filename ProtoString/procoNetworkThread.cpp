/*==============================================================================
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************
#include "stdafx.h"

#include "risSockets.h"
#include "procoUdpSettings.h"

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
   // Set base class thread services.
   BaseClass::setThreadName("Network");
   BaseClass::setThreadPriorityHigh();
   BaseClass::setThreadPrintLevel(TS::PrintLevel(3, 3));

   // Set base class thread priority.
   BaseClass::setThreadPriorityHigh();

   // Set base class timer period.
   BaseClass::mTimerPeriod = gUdpSettings.mThreadTimerPeriod;

   // Initialize qcalls.
   mRxStringQCall.bind(this, &NetworkThread::executeRxString);

   // Initialize variables.
   mUdpStringThread = 0;
   mTPFlag = false;
   mStatusCount1=0;
   mStatusCount2=0;
}

NetworkThread::~NetworkThread()
{
   delete mUdpStringThread;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Thread init function. This is called by the base class immediately 
// after the thread starts running. It starts the child thread.

void NetworkThread::threadInitFunction()
{
   // Instance of network socket settings.
   Ris::Net::Settings tSettings;

   tSettings.setLocalPort(gUdpSettings.mMyUdpPort);
   tSettings.setRemoteAddress(gUdpSettings.mOtherUdpIPAddress, gUdpSettings.mOtherUdpPort);
   tSettings.mRxStringQCall = mRxStringQCall;
   tSettings.mPrintLevel = gUdpSettings.mPrintLevel;

   // Create the child thread with the settings.
   mUdpStringThread = new Ris::Net::UdpStringThread(tSettings);

   // Launch the child thread.
   mUdpStringThread->launchThread();
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Thread exit function, base class overload.

void NetworkThread::threadExitFunction()
{
   // Shutdown the child thread.
   mUdpStringThread->shutdownThread();
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Show thread state info, base class overload.

void NetworkThread::showThreadInfo()
{
   BaseClass::showThreadInfo();
   if (mUdpStringThread) mUdpStringThread->showThreadInfo();
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// QCall registered to the mUdpStringThread child thread. It is invoked when
// a string is received. It process the received strings.

void NetworkThread::executeRxString(std::string* aString)
{
   Prn::print(0, "NetworkThread::executeRxString %s",aString->c_str());
   delete aString;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Send a string via the child socket thread.

void NetworkThread::sendString (std::string* aString)
{
   mUdpStringThread->sendString(aString);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Execute periodically. This is called by the base class timer. It
// sends an echo request string.

void NetworkThread::executeOnTimer(int aTimerCount)
{
   if (!mTPFlag) return;
   Prn::print(Prn::ThreadRun3, "NetworkThread::executeOnTimer %d", aTimerCount);

   // Send a string.
   std::string* tString = new std::string("timer_string");
   sendString(tString);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace