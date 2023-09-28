//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "stdafx.h"

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
// Send a requested number of bytes. Return the actual number of bytes
// sent or a negative error code.

int SerialPort::doSendBytes(const char* aData, int aNumBytes)
{
   Trc::write(mTI, 0, "SerialPort::doSendBytes");

   // Guard.
   if (!mValidFlag)
   {
      Trc::write(mTI, 0, "SerialPort::doSendBytes INVALID");
      return cSerialRetError;
   }

   // Check the comm modem status.
   if (!doGetModemStatus())
   {
      Trc::write(mTI, 0, "SerialPort::doSendBytes FAIL modem1");
      return cSerialRetError;
   }

   // Clear errors.
   ClearCommError(mPortHandle, 0, 0);

   // Locals.
   DWORD tRet = 0;
   DWORD tNumWritten = 0;
   DWORD tNumToWrite = aNumBytes;
   bool tWaitForCompletion = false;
   memset(&mWriteOverlapped, 0, sizeof(mWriteOverlapped));
   mWriteOverlapped.hEvent = mWriteCompletion;
   DWORD tWaitTimeout = mSettings.mTxTimeout == 0 ? INFINITE : mSettings.mTxTimeout;

   // Issue write operation, overlapped i/o.
   tRet = WriteFile(mPortHandle, aData, tNumToWrite, &tNumWritten, &mWriteOverlapped);

   // If the write completes immediately.
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
         ClearCommError(mPortHandle, 0, 0);
         Trc::write(mTI, 0, "SerialPort::doSendBytes ABORTED1");
         return cSerialRetAbort;
      }
      // Check for errors.
      else if (GetLastError() != ERROR_IO_PENDING)
      {
         mPortErrorCount++;
         Trc::write(mTI, 0, "SerialPort::doSendBytes FAIL1 %d", GetLastError());
         return cSerialRetError;
      }
      // The write is pending.
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
      tRet = WaitForSingleObject(mWriteOverlapped.hEvent, tWaitTimeout);

      // Select on the returned status code.
      switch (tRet)
      {
      case WAIT_FAILED:
      {
         mPortErrorCount++;
         Trc::write(mTI, 0, "SerialPort::doSendBytes FAIL2");
         return cSerialRetError;
      }
      break;
      case WAIT_ABANDONED:
      {
         mPortErrorCount++;
         Trc::write(mTI, 0, "SerialPort::doSendBytes FAIL3");
         return cSerialRetError;
      }
      break;
      case WAIT_TIMEOUT:
      {
         Trc::write(mTI, 0, "SerialPort::doSendBytes TIMEOUT");
         //Prn::print(0,      "SerialPort::doSendBytes TIMEOUT");
         return cSerialRetTimeout;
      }
      break;
      case WAIT_OBJECT_0:
      {
         // Check overlapped result for abort or for errors.
         if (!GetOverlappedResult(mPortHandle, &mWriteOverlapped, &tNumWritten, FALSE))
         {
            if (GetLastError() == ERROR_OPERATION_ABORTED)
            {
               ClearCommError(mPortHandle, 0, 0);
               Trc::write(mTI, 0, "SerialPort::doSendBytes ABORTED2");
               return cSerialRetAbort;
            }
            else
            {
               mPortErrorCount++;
               Trc::write(mTI, 0, "SerialPort::doSendBytes FAIL4");
               return cSerialRetError;
            }
         }
         // Check overlapped result for write empty.
         if (tNumWritten == 0)
         {
            mPortErrorCount++;
            Trc::write(mTI, 0, "SerialPort::doSendBytes FAIL write empty");
            return cSerialRetEmpty;
         }
      }
      break;
      }
   }

   // Check number of bytes.
   if (tNumWritten != tNumToWrite)
   {
      mPortErrorCount++;
      Trc::write(mTI, 0, "SerialPort::doSendBytes FAIL5");
      return cSerialRetError;
   }

   // If bluetooth then flush the transmit.
   if (mSettings.mBthFlag)
   {
      Trc::write(mTI, 6, "SerialPort::doSendBytes flush");

      ClearCommError(mPortHandle, 0, 0);

      DWORD lFlags =
         PURGE_TXABORT | PURGE_TXCLEAR;

      PurgeComm(mPortHandle, lFlags);

      ClearCommError(mPortHandle, 0, 0);

      Trc::write(mTI, 6, "SerialPort::doSendBytes flush done");
   }

   // Success.
   mTxByteCount += tNumWritten;
   Trc::write(mTI, 0, "SerialPort::doSendBytes done");
   return tNumWritten;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace

