/*==============================================================================
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************
#include "stdafx.h"

#include "someSerialParms.h"

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
   mRxBuffer[0] = 0;
   mTxBuffer[0] = 0;
   mErrorCount = 0;
   mRestartCount = 0;
   mRxCount = 0;
   mTxCount = 0;
   mRxReqNumBytes = gSerialParms.mRxReqNumBytes;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Thread init function. This is called by the base class immediately
// after the thread starts running. It initializes the serial port.

void PortResponderThread::threadInitFunction()
{
   Trc::write(11, 0, "PortResponderThread::threadInitFunction");

   // Instance of network socket settings.
   Ris::Net::Settings tSettings;

   mRxSocket.configureLocal(5012);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Thread run function. This is called by the base class immediately
// after the thread init function. It runs a loop that blocks on 
// serial port receives and then processes them. The loop terminates
// when the serial port receive is aborted.

void PortResponderThread::threadRunFunction()
{
   while (mRxSocket.doRecvString())
   {
      puts(mRxSocket.mRxString);
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Thread exit function. This is called by the base class immediately
// before the thread is terminated. It is close the serial port.

void PortResponderThread::threadExitFunction()
{
   printf("somePortResponderThread::threadExitFunction\n");

   // Close the serial port.
   mRxSocket.doClose();
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Thread shutdown function. This is called out of the context of
// this thread. It aborts the serial port receive and waits for the
// thread to terminate after execution of the thread exit function.

void PortResponderThread::shutdownThread()
{
   printf("somePortResponderThread::shutdownThread\n");

   // Abort pending serial port receives
   mRxSocket.doClose();

   // Wait for thread to terminate.
   BaseClass::shutdownThread();
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Send bytes via the serial port. This executes in the context of
// the calling thread.

void PortResponderThread::sendString(char* aString)
{
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace