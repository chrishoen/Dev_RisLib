//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "stdafx.h"

#include <windows.h>

#include "my_functions.h"
#include "risPortableCalls.h"
#include "prnPrint.h"
#include "trcTrace.h"

#include "risSerialPort.h"

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
   HANDLE mCommCompletion;
};

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Receive a requested number of bytes. Block until all of the bytes
// have been received. Copy the bytes into the pointer argument.
// Return the number of bytes received or a negative error code.

int SerialPort::doReceiveAllBytes2(char* aData, int aRequestBytes)
{
   Trc::write(mTI, 0, "SerialPort::doReceiveAllBytes2");

   // Locals.
   DWORD tRet = 0;
   DWORD tNumRead = 0;
   DWORD tNumToRead = aRequestBytes;
   bool tWaitForCompletion = false;
   OVERLAPPED tOverlapped;
   memset(&tOverlapped, 0, sizeof(tOverlapped));
   tOverlapped.hEvent = mSpecific->mReadCompletion;
   DWORD tWaitTimeout = mSettings.mRxTimeout == 0 ? INFINITE : mSettings.mRxTimeout;

   // Issue read operation, overlapped i/o.
   tRet = ReadFile(mSpecific->mPortHandle, aData, tNumToRead, &tNumRead, &tOverlapped);

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
         ClearCommError(mSpecific->mPortHandle, 0, 0);
         Trc::write(mTI, 0, "SerialPort::doReceiveAllBytes ABORTED1");
         return cSerialRetAbort;
      }
      // Check for errors.
      else if (GetLastError() != ERROR_IO_PENDING)
      {
         mPortErrorCount++;
         Trc::write(mTI, 0, "SerialPort::doReceiveAllBytes FAIL1");
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
         mPortErrorCount++;
         Trc::write(mTI, 0, "SerialPort::doReceiveAllBytes FAIL2");
         return cSerialRetError;
      }
      break;
      case WAIT_ABANDONED:
      {
         mPortErrorCount++;
         Trc::write(mTI, 0, "SerialPort::doReceiveAllBytes FAIL3");
         return cSerialRetError;
      }
      break;
      case WAIT_TIMEOUT:
      {
         Trc::write(mTI, 0, "SerialPort::doReceiveAllBytes TIMEOUT");
         printf("serial_poll_error_2 timeout\n");
         return cSerialRetTimeout;
      }
      break;
      case WAIT_OBJECT_0:
      {
         // Check overlapped result for abort or for errors.
         if (!GetOverlappedResult(mSpecific->mPortHandle, &tOverlapped, &tNumRead, FALSE))
         {
            if (GetLastError() == ERROR_OPERATION_ABORTED)
            {
               ClearCommError(mSpecific->mPortHandle, 0, 0);
               Trc::write(mTI, 0, "SerialPort::doReceiveAllBytes ABORTED2");
               return cSerialRetAbort;
            }
            else
            {
               mPortErrorCount++;
               Trc::write(mTI, 0, "SerialPort::doReceiveAllBytes FAIL4");
               return cSerialRetError;
            }
         }
         // Check overlapped result for read empty.
         if (tNumRead == 0)
         {
            mPortErrorCount++;
            Trc::write(mTI, 0, "SerialPort::doReceiveAllBytes FAIL read empty");
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
      Trc::write(mTI, 0, "SerialPort::doReceiveAllBytes FAIL5");
      return cSerialRetError;
   }

   // Success.
   mRxByteCount += tNumRead;
   Trc::write(mTI, 0, "SerialPort::doReceiveAllBytes done");
   return tNumRead;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace

