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
   HANDLE mCommCompletion;
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
   mSpecific->mCommCompletion = 0;
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

   // Create.
   mSpecific->mCommCompletion = CreateEvent(NULL, TRUE, FALSE, NULL);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Thread run function. This is called by the base class immediately 
// after the thread init function. It runs a loop that monitors the
// bluetooth. If it detects an error then it resets the serial port.

void SerialBthThread::threadRunFunction()
{
   while (true)
   {
      if (BaseClass::mTerminateFlag) return;
      if (int tRet = doBthTest())
      {
         Prn::print(Prn::Show1, "doBthTest ERROR %d", tRet);
         BaseClass::threadSleep(1000);
      }
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Thread exit function. This is called by the base class immedidately
// before the thread is terminated.

void SerialBthThread::threadExitFunction()
{
   Prn::print(0, "SerialBthThread::threadExitFunction");
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
// Do bluetooth test with WaitCommEvent.

int SerialBthThread::doBthTest()
{
   Trc::write(mTI, 0, "SerialPort::doBthTest");
   Prn::print(Prn::Show1, "doBthTest");

   // Locals.
   DWORD tRet = 0;
   DWORD tEvtMask = 0;
   DWORD tNumDummy = 0;
   bool tWaitForCompletion = false;
   OVERLAPPED tOverlapped;
   memset(&tOverlapped, 0, sizeof(tOverlapped));
   tOverlapped.hEvent = mSpecific->mCommCompletion;
   DWORD tWaitTimeout = INFINITE;

   // Issue wait comm event operation, overlapped i/o.
   tRet = WaitCommEvent(mSpecific->mPortHandle, &tEvtMask, &tOverlapped);

   // If the wait completes immediately.
   if (tRet)
   {
      // Do not wait for completion.
      tWaitForCompletion = false;
   }
   else
   {
      // Check for abort.
      if (GetLastError() == ERROR_OPERATION_ABORTED)
      {
         ClearCommError(mSpecific->mPortHandle, 0, 0);
         Trc::write(mTI, 0, "SerialPort::doBthTest ABORTED1");
         return cSerialRetAbort;
      }
      // Check for errors.
      else if (GetLastError() != ERROR_IO_PENDING)
      {
         Trc::write(mTI, 0, "SerialPort::doBthTest FAIL1 %d", GetLastError());
         return cSerialRetError;
      }
      // The read is pending.
      else
      {
         // Wait for completion.
         tWaitForCompletion = true;
      }
   }

   // If waiting for completion.
   if (tWaitForCompletion)
   {
      // Wait for overlapped i/o completion.
      tRet = WaitForSingleObject(tOverlapped.hEvent, tWaitTimeout);

      // Select on the returned status code.
      switch (tRet)
      {
      case WAIT_FAILED:
      {
         Trc::write(mTI, 0, "SerialPort::doBthTest FAIL2");
         return cSerialRetError;
      }
      break;
      case WAIT_ABANDONED:
      {
         Trc::write(mTI, 0, "SerialPort::doBthTest FAIL3");
         return cSerialRetError;
      }
      break;
      case WAIT_TIMEOUT:
      {
         Trc::write(mTI, 0, "SerialPort::doBthTest TIMEOUT");
         printf("serial_poll_error_22 timeout\n");
         return cSerialRetTimeout;
      }
      break;
      case WAIT_OBJECT_0:
      {
         // Check overlapped result for abort or for errors.
         if (!GetOverlappedResult(mSpecific->mPortHandle, &tOverlapped, &tNumDummy, FALSE))
         {
            if (GetLastError() == ERROR_OPERATION_ABORTED)
            {
               ClearCommError(mSpecific->mPortHandle, 0, 0);
               Trc::write(mTI, 0, "SerialPort::doBthTest ABORTED2");
               return cSerialRetAbort;
            }
            else
            {
               Trc::write(mTI, 0, "SerialPort::doBthTest FAIL4");
               return cSerialRetError;
            }
         }
      }
      break;
      }
   }

   // Success.
   Trc::write(mTI, 0, "SerialPort::doBthTest done %x", tEvtMask);
   Prn::print(0, "doBthTest EvtMask %x", tEvtMask);

   DWORD tModemStat = 0xff;
   GetCommModemStatus(mSpecific->mPortHandle, &tModemStat);

   Trc::write(mTI, 0, "ModemStat %x", tModemStat);
   Prn::print(Prn::Show1, "ModemStat %x", tModemStat);
   return 0;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace

