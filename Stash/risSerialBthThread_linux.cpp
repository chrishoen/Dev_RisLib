//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "stdafx.h"

#include "prnPrint.h"

#include "risSerialBthThread.h"

namespace Ris
{

//******************************************************************************
//******************************************************************************
//******************************************************************************

SerialBthThread::SerialBthThread(Ris::SerialPort* aSerialPort)
{
   // Set base class variables.
   BaseClass::setThreadName("Bluetooth");
   BaseClass::setThreadPriorityNormal();

   // Set member variables.
   mSerialPort = aSerialPort;
   mTI = mSerialPort->mSettings.mTraceIndex;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Thread init function. This is called by the base class immedidately 
// after the thread starts running.

void SerialBthThread::threadInitFunction()
{
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Thread run function. This is called by the base class immediately 
// after the thread init function. It runs a loop that waits for the
// timer or the termination event.

void SerialBthThread::threadRunFunction()
{
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Thread exit function. This is called by the base class immedidately
// before the thread is terminated.

void SerialBthThread::threadExitFunction()
{
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Thread shutdown function. Set the termination flag, post to the 
// waitable event and wait for the thread to terminate.

void SerialBthThread::shutdownThread()
{
   BaseClass::mTerminateFlag = true;
   BaseClass::waitForThreadTerminate();
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace

