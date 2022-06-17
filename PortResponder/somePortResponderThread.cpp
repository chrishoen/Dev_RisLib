/*==============================================================================
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************
#include "stdafx.h"

#define  _SOMEPORTRESPONDERTHREAD_CPP_
#include "somePortResponderThread.h"

//******************************************************************************
//******************************************************************************
//******************************************************************************

namespace Some
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Constructor.

PortResponderThread::PortResponderThread()
{
   // Set base class thread services.
   BaseClass::setThreadName("PortResponder");
   BaseClass::setThreadPriorityHigh();

   // Set base class thread priority.
   BaseClass::setThreadPriorityHigh();

   // Initialize variables.
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Thread init function. This is called by the base class immediately
// after the thread starts running. It initializes the udp port.

void PortResponderThread::threadInitFunction()
{
   Trc::write(11, 0, "PortResponderThread::threadInitFunction");

   // Instance of network socket settings.
   Ris::Net::Settings tSettings;
   tSettings.setLocalPort(5012);
   tSettings.setUdpWrapFlag(true);
   tSettings.mTraceIndex = 11;

   // Configure the socket with the settings.
   mUdpStringSocket.initialize(tSettings);
   mUdpStringSocket.configure();

   Trc::write(11, 0, "PortResponderThread::threadInitFunction done");
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Thread run function. This is called by the base class immediately
// after the thread init function. It runs a loop that blocks on 
// udp port receives and then processes them. The loop terminates
// when the udp port receive is aborted.

void PortResponderThread::threadRunFunction()
{
   while (mUdpStringSocket.doRecvString())
   {
      Prn::print(0, "mUdpStringSocket.doRecvString %16s : %5d",
         mUdpStringSocket.mFromAddress.mString,
         mUdpStringSocket.mFromAddress.mPort);
      Prn::print(0, "mUdpStringSocket.doRecvString %16s", mUdpStringSocket.mRxString);
      mUdpStringSocket.doSendString("Mira in the neighborhood 1234");
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Thread exit function. This is called by the base class immediately
// before the thread is terminated. It is close the udp port.

void PortResponderThread::threadExitFunction()
{
   printf("somePortResponderThread::threadExitFunction\n");

   // Close the udp port.
   mUdpStringSocket.doClose();
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Thread shutdown function. This is called out of the context of
// this thread. It aborts the udp port receive and waits for the
// thread to terminate after execution of the thread exit function.

void PortResponderThread::shutdownThread()
{
   printf("somePortResponderThread::shutdownThread\n");

   // Abort pending udp port receives
   mUdpStringSocket.doClose();

   // Wait for thread to terminate.
   BaseClass::shutdownThread();
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Send bytes via the udp port. This executes in the context of
// the calling thread.

void PortResponderThread::sendString(char* aString)
{
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace