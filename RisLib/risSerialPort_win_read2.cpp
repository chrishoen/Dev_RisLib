//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "stdafx.h"

#include <windows.h>

#include "my_functions.h"
#include "risPortableCalls.h"
#include "prnPrint.h"
#include "trcTrace.h"

#include "risSerialPort_win.h"

namespace Ris
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Get the comm modem status. Return true if the modem is valid.

bool SerialPort::doGetModemStatus()
{
   DWORD tModemStat = 0xff;
   GetCommModemStatus(mPortHandle, &tModemStat);

   Trc::write(mTI, 0, "ModemStat %x", tModemStat);
   Prn::print(Prn::Show1, "ModemStat %x", tModemStat);

   mModemValid = tModemStat != 0;
   return mModemValid;
}
   
//******************************************************************************
//******************************************************************************
//******************************************************************************
// Receive a requested number of bytes. Block until all of the bytes
// have been received. Copy the bytes into the pointer argument.
// Return the number of bytes received or a negative error code.

int SerialPort::doReceiveBytes2(char* aData, int aNumBytes)
{
   Trc::write(mTI, 0, "SerialPort::doReceiveBytes2");

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Locals.

   // Wait.
   DWORD tRet = 0;
   DWORD tWaitTimeout = mSettings.mRxTimeout == 0 ? INFINITE : mSettings.mRxTimeout;
   HANDLE tWaitHandles[2];
   memset(&tWaitHandles, 0, 2 * sizeof(HANDLE));
   DWORD tWaitCount = 0;

   // Read.
   DWORD tNumRead = 0;
   DWORD tNumToRead = aNumBytes;
   OVERLAPPED tReadOverlapped;
   memset(&tReadOverlapped, 0, sizeof(tReadOverlapped));
   tReadOverlapped.hEvent = mReadCompletion;
   bool tReadPending = false;

   // Comm.
   DWORD tEvtMask = 0;
   DWORD tNumDummy = 0;
   OVERLAPPED tCommOverlapped;
   memset(&tCommOverlapped, 0, sizeof(tCommOverlapped));
   tCommOverlapped.hEvent = mCommCompletion;
   bool tCommPending = false;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Read.
   
   // Issue read operation, overlapped i/o.
   tRet = ReadFile(mPortHandle, aData, tNumToRead, &tNumRead, &tReadOverlapped);

   // If the read completes immediately.
   if (tRet)
   {
      // Check number of bytes.
      if (tNumRead != tNumToRead)
      {
         mPortErrorCount++;
         Trc::write(mTI, 0, "SerialPort::doReceiveBytes FAIL1");
         return cSerialRetError;
      }

      // Done. Success.
      mRxByteCount += tNumRead;
      Trc::write(mTI, 0, "SerialPort::doReceiveBytes done1");
      return tNumRead;
   }

   // Check for read abort.
   if (GetLastError() == ERROR_OPERATION_ABORTED)
   {
      ClearCommError(mPortHandle, 0, 0);
      Trc::write(mTI, 0, "SerialPort::doReceiveBytes ABORTED1");
      return cSerialRetAbort;
   }
   // Check for read errors.
   else if (GetLastError() != ERROR_IO_PENDING)
   {
      mPortErrorCount++;
      Trc::write(mTI, 0, "SerialPort::doReceiveBytes FAIL1");
      return cSerialRetError;
   }
   // The read is pending.
   tReadPending = true;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Comm.

   // Issue wait comm event operation, overlapped i/o.
   tRet = WaitCommEvent(mPortHandle, &tEvtMask, &tCommOverlapped);

   // If the wait comm event completes immediately.
   if (tRet)
   {
      // Check the comm modem status.
      if (!doGetModemStatus())
      {
         Trc::write(mTI, 0, "SerialPort::doReceiveBytes FAIL2 modem");
         return cSerialRetError;
      }
   }
   else
   {
      // Check for abort.
      if (GetLastError() == ERROR_OPERATION_ABORTED)
      {
         ClearCommError(mPortHandle, 0, 0);
         Trc::write(mTI, 0, "SerialPort::doReceiveBytes ABORTED2");
         return cSerialRetAbort;
      }
      // Check for errors.
      else if (GetLastError() != ERROR_IO_PENDING)
      {
         Trc::write(mTI, 0, "SerialPort::doReceiveBytes FAIL3 %d", GetLastError());
         return cSerialRetError;
      }
      // The comm event is pending.
      else
      {
         tCommPending = true;
      }
   }

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Wait. At this point a read is pending and a comm event might
   // be pending.

   // Setup the wait parameters.
   memset(&tWaitHandles, 0, 2 * sizeof(HANDLE));
   tWaitHandles[0] = mReadCompletion;
   tWaitCount = 1;
   if (tCommPending)
   {
      tWaitHandles[1] = mCommCompletion;
      tWaitCount = 2;
   }

   // Wait for overlapped i/o completion.
   tRet = WaitForMultipleObjects(tWaitCount, tWaitHandles, FALSE, tWaitTimeout);

   // Select on the returned status code for timeout or errors.
   switch (tRet)
   {
   case WAIT_FAILED:
   {
      mPortErrorCount++;
      Trc::write(mTI, 0, "SerialPort::doReceiveBytes FAIL2");
      return cSerialRetError;
   }
   break;
   case WAIT_TIMEOUT:
   {
      Trc::write(mTI, 0, "SerialPort::doReceiveBytes TIMEOUT");
      printf("serial_poll_error_2 timeout\n");
      return cSerialRetTimeout;
   }
   break;
   case WAIT_OBJECT_0:
   case WAIT_OBJECT_0 + 1:
   {
      // read or comm event completion.
   }
   break;
   default:
   {
      // Should never get here.
      Trc::write(mTI, 0, "SerialPort::doReceiveBytes FAIL991");
      return cSerialRetError;
   }
   }
   
   // At this point the wait call returned with no timeout or errors.
   // Check the overlapped results for read or comm event completion. 

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Check read results.

   if (tReadPending)
   {
       // Check overlapped result for read abort or read errors.
      if (!GetOverlappedResult(mPortHandle, &tReadOverlapped, &tNumRead, FALSE))
      {
         if (GetLastError() == ERROR_OPERATION_ABORTED)
         {
            ClearCommError(mPortHandle, 0, 0);
            Trc::write(mTI, 0, "SerialPort::doReceiveBytes ABORTED2");
            return cSerialRetAbort;
         }
         else
         {
            mPortErrorCount++;
            Trc::write(mTI, 0, "SerialPort::doReceiveBytes FAIL4");
            return cSerialRetError;
         }
      }

      // Check overlapped result for read empty.
      if (tNumRead == 0)
      {
         mPortErrorCount++;
         Trc::write(mTI, 0, "SerialPort::doReceiveBytes FAIL read empty");
         return cSerialRetEmpty;
      }
      // Check number of bytes.
      if (tNumRead != tNumToRead)
      {
         mPortErrorCount++;
         Trc::write(mTI, 0, "SerialPort::doReceiveBytes FAIL5");
         return cSerialRetError;
      }

      // Success.
      mRxByteCount += tNumRead;
      Trc::write(mTI, 0, "SerialPort::doReceiveBytes done2");
      return tNumRead;
   }

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Check comm event results.

   if (tCommPending)
   {
      // Check overlapped result for comm event abort or read errors.
      if (!GetOverlappedResult(mPortHandle, &tCommOverlapped, &tNumRead, FALSE))
      {
         if (GetLastError() == ERROR_OPERATION_ABORTED)
         {
            ClearCommError(mPortHandle, 0, 0);
            Trc::write(mTI, 0, "SerialPort::doReceiveBytes ABORTED2");
            return cSerialRetAbort;
         }
         else
         {
            mPortErrorCount++;
            Trc::write(mTI, 0, "SerialPort::doReceiveBytes FAIL4");
            return cSerialRetError;
         }
      }
   }


   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Done.

   // Should never get here.
   Trc::write(mTI, 0, "SerialPort::doReceiveBytes FAIL992");
   return cSerialRetError;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace

