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

int SerialPort::doReceiveBytes2(char* aData, int aNumBytes)
{
   Trc::write(mTI, 0, "SerialPort::doReceiveBytes2 %s", mFirstReadFlag ? "first" : "");

   // Check the comm modem status.
   if (!doGetModemStatus())
   {
      Trc::write(mTI, 0, "SerialPort::doReceiveBytes FAIL modem1");
      return cSerialRetError;
   }

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
   bool tTestReadResults = false;
   memset(&mReadOverlapped, 0, sizeof(mReadOverlapped));
   mReadOverlapped.hEvent = mReadCompletion;
   mReadPending = false;

   // Comm.
   DWORD tEvtMask = 0;
   DWORD tNumDummy = 0;
   bool tTestCommResults = false;
   memset(&mCommOverlapped, 0, sizeof(mCommOverlapped));
   mCommOverlapped.hEvent = mCommCompletion;
   mCommPending = false;

   // Check for first read after successful open.
   if (mFirstReadFlag)
   {
      // Set variables.
      mFirstReadFlag = false;
      mReadPending = false;
      mCommPending = false;
   }

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Start overlapped ReadFile.

   // Issue read operation, overlapped i/o.
   tRet = ReadFile(mPortHandle, aData, tNumToRead, &tNumRead, &mReadOverlapped);

   // If it completes immediately.
   if (tRet)
   {
      // Check for read empty.
      if (tNumRead == 0)
      {
         mPortErrorCount++;
         Trc::write(mTI, 0, "SerialPort::doReceiveBytes FAIL read1 empty");
         return cSerialRetEmpty;
      }
      // Check number of bytes.
      else if (tNumRead != tNumToRead)
      {
         mPortErrorCount++;
         Trc::write(mTI, 0, "SerialPort::doReceiveBytes FAIL read1 num");
         return cSerialRetError;
      }

      // Done. Success. The read completed immediately with no errors.
      mRxByteCount += tNumRead;
      Trc::write(mTI, 0, "SerialPort::doReceiveBytes done1");
      return tNumRead;
   }

   // Check for pending. 
   if (GetLastError() == ERROR_IO_PENDING)
   {
      // ReadFile is pending.
      mReadPending = true;
   }
   // Check for abort.
   else if (GetLastError() == ERROR_OPERATION_ABORTED)
   {
      Trc::write(mTI, 0, "SerialPort::doReceiveBytes ABORTED read1");
      ClearCommError(mPortHandle, 0, 0);
      return cSerialRetAbort;
   }
   // Anything else is an error.
   else
   {
      mPortErrorCount++;
      Trc::write(mTI, 0, "SerialPort::doReceiveBytes FAIL read1 error %d", GetLastError());
      return cSerialRetError;
   }
   // Note: At this point ReadFile is pending.

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // If not pending then start overlapped WaitCommEvent. This 
   // might be pending from a previous receive call. Because there might 
   // be spurious comm events, loop until pending or exit if error.
   // A spurious comm event happens when there is a comm event but the
   // modem status is still okay.

RestartComm:

   while (!mCommPending)
   {
      // Set the comm event mask. Do this each time.
      if (!SetCommMask(mPortHandle, cCommEvtMask))
      {
         Trc::write(mTI, 0, "SerialPort::doReceiveBytes FAIL comm mask %d", GetLastError());
         return cSerialRetError;
      }

      // Issue wait comm event operation, overlapped i/o.
      tRet = WaitCommEvent(mPortHandle, &tEvtMask, &mCommOverlapped);

      // If it completes immediately.
      if (tRet)
      {
         // Check the comm modem status.
         if (!doGetModemStatus())
         {
            Trc::write(mTI, 0, "SerialPort::doReceiveBytes FAIL modem2");
            return cSerialRetError;
         }
         else
         {
            // The modem status was okay, so there was a spurious comm event
            // that will be ignored. This will repeat the loop because comm
            // is still pending.
            Trc::write(mTI, 0, "SerialPort::doReceiveBytes spurious comm event");
         }
      }
      else
      {
         // Check for pending.
         if (GetLastError() == ERROR_IO_PENDING)
         {
            // WaitCommEvent is pending.
            mCommPending = true;
         }
         // Check for abort.
         else if (GetLastError() == ERROR_OPERATION_ABORTED)
         {
            Trc::write(mTI, 0, "SerialPort::doReceiveBytes ABORTED comm1");
            ClearCommError(mPortHandle, 0, 0);
            return cSerialRetAbort;
         }
         // Anything else is an error.
         else
         {
            Trc::write(mTI, 0, "SerialPort::doReceiveBytes FAIL comm1 error %d", GetLastError());
            return cSerialRetError;
         }
      }
   }
   // Note: At this point ReadFile and WaitForEvent are both pending.

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Wait for ReadFile or WaitForCommEvent completion.

   // Setup the wait parameters.
   memset(&tWaitHandles, 0, 2 * sizeof(HANDLE));
   tWaitHandles[0] = mReadCompletion;
   tWaitHandles[1] = mCommCompletion;
   tWaitCount = 2;
   tTestReadResults = false;
   tTestCommResults = false;

   // Wait for overlapped i/o completion.
   tRet = WaitForMultipleObjects(tWaitCount, tWaitHandles, FALSE, tWaitTimeout);

   // Select on the returned status code for timeout or errors.
   switch (tRet)
   {
   case WAIT_FAILED:
   {
      mPortErrorCount++;
      Trc::write(mTI, 0, "SerialPort::doReceiveBytes FAIL WAIT_FAILED");
      return cSerialRetError;
   }
   break;
   case WAIT_TIMEOUT:
   {
      Trc::write(mTI, 0, "SerialPort::doReceiveBytes WAIT_TIMEOUT");
      printf("serial_poll_error_2 timeout\n");
      return cSerialRetTimeout;
   }
   break;
   case WAIT_OBJECT_0:
   {
      // ReadFile completed and WaitCommEvent might have completed.
      tTestReadResults = true;
      tTestCommResults = true;
   }
   break;
   case WAIT_OBJECT_0 + 1:
   {
      // ReadFile did not complete and WaitCommEvent completed.
      tTestReadResults = false;
      tTestCommResults = true;
   }
   break;
   default:
   {
      // Should never get here. 
      Trc::write(mTI, 0, "SerialPort::doReceiveBytes FAIL991");
      return cSerialRetError;
   }
   }
   
   // Note: At this point the wait call returned with no timeout or errors.
   // Check the overlapped results. 

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Check WaitCommEvent results.

   if (tTestCommResults)
   {
      // Check overlapped result.
      if (!GetOverlappedResult(mPortHandle, &mCommOverlapped, &tNumDummy, FALSE))
      {
         // Check for still pending.
         if (GetLastError() == ERROR_IO_INCOMPLETE)
         {
            // WaitCommEvent is still pending.
         }
         // Check for abort.
         else if (GetLastError() == ERROR_OPERATION_ABORTED)
         {
            Trc::write(mTI, 0, "SerialPort::doReceiveBytes ABORTED comm2");
            ClearCommError(mPortHandle, 0, 0);
            return cSerialRetAbort;
         }
         // Anything else is an error.
         else
         {
            mPortErrorCount++;
            Trc::write(mTI, 0, "SerialPort::doReceiveBytes FAIL comm2 error %d", GetLastError());
            return cSerialRetError;
         }
         // Note: WaitCommEvent is still pending and no errors.
      }
      // WaitCommEvent completion.
      else
      {
         // Check the comm modem status.
         if (!doGetModemStatus())
         {
            Trc::write(mTI, 0, "SerialPort::doReceiveBytes FAIL modem3");
            return cSerialRetError;
         }
         // WaitCommEvent completed successfully.
         mCommPending = false;
      }
   }
   // Note: At this point WaitCommEvent is or is not still pending.

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Check ReadFile results.

   if (tTestReadResults)
   {
      // Check overlapped result.
      if (!GetOverlappedResult(mPortHandle, &mReadOverlapped, &tNumRead, FALSE))
      {
         // Check for still pending.
         if (GetLastError() == ERROR_IO_INCOMPLETE)
         {
            // ReadFile is still pending.
         }
         // Check for abort.
         else if (GetLastError() == ERROR_OPERATION_ABORTED)
         {
            Trc::write(mTI, 0, "SerialPort::doReceiveBytes ABORTED read2");
            ClearCommError(mPortHandle, 0, 0);
            return cSerialRetAbort;
         }
         // Anything else is an error.
         else
         {
            mPortErrorCount++;
            Trc::write(mTI, 0, "SerialPort::doReceiveBytes FAIL read2 error %d", GetLastError());
            return cSerialRetError;
         }
      }
      else
      {
         // ReadFile completed successfully.
         mReadPending = false;
      }
   }
   // At this point, ReadFile OR WaitCommEvent completed successfully.
   if (mReadPending && mCommPending)
   {
      // Should never get here.
      Trc::write(mTI, 0, "SerialPort::doReceiveBytes FAIL996");
      return cSerialRetError;
   }

   // If ReadFile is still pending and WaitCommEvent is not pending
   // then there was a spurious event, so restart the WaitCommEvent
   // and fall through to repeat the WaitForMultipleObjects.
   if (mReadPending && !mCommPending)
   {
      goto RestartComm;
   }

   // Note: At this point, ReadFile completed successfully and WaitCommEvent
   // is or is not still pending.

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // ReadFile completed successfully. Check for errors.

   // Check overlapped result for read empty.
   if (tNumRead == 0)
   {
      mPortErrorCount++;
      Trc::write(mTI, 0, "SerialPort::doReceiveBytes FAIL read2 empty");
      return cSerialRetEmpty;
   }
   // Check number of bytes.
   if (tNumRead != tNumToRead)
   {
      mPortErrorCount++;
      Trc::write(mTI, 0, "SerialPort::doReceiveBytes FAIL read2 num");
      return cSerialRetError;
   }

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Done. ReadFile completed successfully. WaitCommEvent might have
   // or might not have completed successfully. If it did not complete
   // then it will be restarted the next time receive is called.

   // Success.
   mRxByteCount += tNumRead;
   Trc::write(mTI, 0, "SerialPort::doReceiveBytes done2");
   return tNumRead;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace

