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
// Receive a requested number of bytes. Block until all of the bytes
// have been received. Copy the bytes into the pointer argument.
// Return the number of bytes received or a negative error code.

int SerialPort::doReceiveBytes(char* aData, int aNumBytes)
{
   // If not bluetooth.
   if (!mUseModemStatus)
   {
      return doReceiveBytes1(aData, aNumBytes);
   }
   // If bluetooth.
   else
   {
      return doReceiveBytes2(aData, aNumBytes);
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Receive a requested number of bytes. Block until all of the bytes
// have been received. Copy the bytes into the pointer argument.
// Return the number of bytes received or a negative error code.

int SerialPort::doReceiveBytes1(char* aData, int aNumBytes)
{
   Trc::write(mTI, 0, "SerialPort::doReceiveBytes1");

   // Locals.
   DWORD tRet = 0;
   DWORD tNumRead = 0;
   DWORD tNumToRead = aNumBytes;
   bool tWaitForCompletion = false;
   memset(&mReadOverlapped, 0, sizeof(mReadOverlapped));
   mReadOverlapped.hEvent = mReadCompletion;
   DWORD tWaitTimeout = mSettings.mRxTimeout == 0 ? INFINITE : mSettings.mRxTimeout;

   // Issue read operation, overlapped i/o.
   tRet = ReadFile(mPortHandle, aData, tNumToRead, &tNumRead, &mReadOverlapped);

   // If the read completes immediately.
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
         Trc::write(mTI, 0, "SerialPort::doReceiveBytes ABORTED1");
         return cSerialRetAbort;
      }
      // Check for errors.
      else if (GetLastError() != ERROR_IO_PENDING)
      {
         mPortErrorCount++;
         Trc::write(mTI, 0, "SerialPort::doReceiveBytes FAIL1");
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
      tRet = WaitForSingleObject(mReadOverlapped.hEvent, tWaitTimeout);

      // Select on the returned status code.
      switch (tRet)
      {
      case WAIT_FAILED:
      {
         mPortErrorCount++;
         Trc::write(mTI, 0, "SerialPort::doReceiveBytes FAIL2");
         return cSerialRetError;
      }
      break;
      case WAIT_ABANDONED:
      {
         mPortErrorCount++;
         Trc::write(mTI, 0, "SerialPort::doReceiveBytes FAIL3");
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
      {
         // Check overlapped result for abort or for errors.
         if (!GetOverlappedResult(mPortHandle, &mReadOverlapped, &tNumRead, FALSE))
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
      }
      break;
      }
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
   Trc::write(mTI, 0, "SerialPort::doReceiveBytes done");
   return tNumRead;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace

