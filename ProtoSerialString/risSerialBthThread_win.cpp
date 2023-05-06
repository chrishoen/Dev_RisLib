//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "stdafx.h"

#include <windows.h>

#include "prnPrint.h"

#include "risSerialBthThread.h"

namespace Ris
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Portable specifics.

class SerialPort::Specific
{
public:
   HANDLE mPortHandle;
   HANDLE mReadCompletion;
   HANDLE mWriteCompletion;
};

class SerialBthThread::Specific
{
public:
   HANDLE mPortHandle;
   HANDLE mCommEvent;
   DWORD  mCommMask;
};

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

   // Set specific variables.
   mSpecific = new Specific;
   mSpecific->mPortHandle = (Specific*)mSerialPort->mSpecific->mPortHandle;
   mSpecific->mCommEvent = 0;
   mSpecific->mCommMask = 0;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Thread init function. This is called by the base class immedidately 
// after the thread starts running.

void SerialBthThread::threadInitFunction()
{
   Prn::print(0, "SerialBthThread::threadInitFunction");

   // Set the comm event mask.
   mSpecific->mCommMask =
      EV_BREAK |
      EV_CTS |
      EV_DSR |
      EV_ERR |
      EV_RING |
      EV_RLSD;

   SetCommMask(mSpecific->mPortHandle, mSpecific->mCommMask);
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

