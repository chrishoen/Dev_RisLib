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
   if (!mUseModemStatus) return true;

   DWORD tModemStat = 0xff;
   GetCommModemStatus(mPortHandle, &tModemStat);

   mModemValid = (tModemStat & cCommModemMask) != 0;

   if (!mModemValid)
   {
      Trc::write(mTI, 0, "ModemStatus INVALID %x", tModemStat);
      Prn::print(0,      "ModemStatus INVALID %x", tModemStat);
   }
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
      mFirstReadFlag = false;
      mReadPending = false;
      mCommPending = false;
   }

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Start overlapped read.

   // Issue read operation, overlapped i/o.
   tRet = ReadFile(mPortHandle, aData, tNumToRead, &tNumRead, &mReadOverlapped);

   // If the read completes immediately.
   if (tRet)
   {
      // Check for read empty.
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
         Trc::write(mTI, 0, "SerialPort::doReceiveBytes FAIL1");
         return cSerialRetError;
      }

      // Done. Success. The read completed immediately with no errors.
      mRxByteCount += tNumRead;
      Trc::write(mTI, 0, "SerialPort::doReceiveBytes done1");
      return tNumRead;
   }

   // Check for read abort.
   if (GetLastError() == ERROR_OPERATION_ABORTED)
   {
      Trc::write(mTI, 0, "SerialPort::doReceiveBytes ABORTED1");
      ClearCommError(mPortHandle, 0, 0);
      return cSerialRetAbort;
   }
   // Check for read errors.
   else if (GetLastError() != ERROR_IO_PENDING)
   {
      mPortErrorCount++;
      Trc::write(mTI, 0, "SerialPort::doReceiveBytes FAIL1");
      return cSerialRetError;
   }
   // The read returned with ERROR_IO_PENDING.
   mReadPending = true;
   // At this point the read is pending.

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // If not pending then start overlapped wait for comm event. This 
   // might be pending from a previous receive call. Because there might 
   // be spurious comm events, loop until pending or exit if error.
   // A spurious comm event happens when there is a comm event but the
   //  modem status is still
   // okay.

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

      // If the wait comm event completes immediately.
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
            // that will be ignored. This will go back to the top of the loop.
            Trc::write(mTI, 0, "SerialPort::doReceiveBytes spurious comm event");
         }
      }
      else
      {
         // Check for abort.
         if (GetLastError() == ERROR_OPERATION_ABORTED)
         {
            Trc::write(mTI, 0, "SerialPort::doReceiveBytes ABORTED2");
            ClearCommError(mPortHandle, 0, 0);
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
            mCommPending = true;
         }
      }
   }
   // At this point the read and the comm event are both pending.

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Wait for comm event or read completion.

   // Setup the wait parameters.
   memset(&tWaitHandles, 0, 2 * sizeof(HANDLE));
   tWaitHandles[1] = mCommCompletion;
   tWaitHandles[0] = mReadCompletion;
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
      // Read completed and comm event might have completed.
      tTestCommResults = true;
      tTestReadResults = true;
   case WAIT_OBJECT_0 + 1:
   // Read did not completed and comm event completed.
   {
      // read or comm event completion.
      tTestCommResults = false;
      tTestReadResults = true;
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
   // Check the overlapped results for read and comm event completion. 

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Check comm event results.

   if (tTestCommResults)
   {
      // Check comm overlapped result for abort or errors.
      if (!GetOverlappedResult(mPortHandle, &mCommOverlapped, &tNumDummy, FALSE))
      {
         if (GetLastError() == ERROR_IO_INCOMPLETE)
         {
            // Comm event is still pending.
         }
         else if (GetLastError() == ERROR_OPERATION_ABORTED)
         {
            Trc::write(mTI, 0, "SerialPort::doReceiveBytes ABORTED3");
            ClearCommError(mPortHandle, 0, 0);
            return cSerialRetAbort;
         }
         else
         {
            mPortErrorCount++;
            Trc::write(mTI, 0, "SerialPort::doReceiveBytes FAIL4");
            return cSerialRetError;
         }
      }
      // Comm event completion.
      else
      {
         // Check the comm modem status.
         if (!doGetModemStatus())
         {
            Trc::write(mTI, 0, "SerialPort::doReceiveBytes FAIL modem3");
            return cSerialRetError;
         }
         // Comm event is no longer pending.
         mCommPending = false;
      }
   }

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Check read results.

   if (tTestReadResults)
   {
      // Check read overlapped result for abort or errors.
      if (!GetOverlappedResult(mPortHandle, &mReadOverlapped, &tNumRead, FALSE))
      {
         if (GetLastError() == ERROR_IO_INCOMPLETE)
         {
            // The read is still pending.
         }
         else if (GetLastError() == ERROR_OPERATION_ABORTED)
         {
            Trc::write(mTI, 0, "SerialPort::doReceiveBytes ABORTED4");
            ClearCommError(mPortHandle, 0, 0);
            return cSerialRetAbort;
         }
         else
         {
            mPortErrorCount++;
            Trc::write(mTI, 0, "SerialPort::doReceiveBytes FAIL4 %d", GetLastError());
            return cSerialRetError;
         }
         // The read is still pending. WaitForMultipleObjects returned because
         // the comm event completed. Restart the wait for comm event and call
         // WaitForMultipleObjects again.
         if (!mCommPending)
         {
            // Should never get here.
            Trc::write(mTI, 0, "SerialPort::doReceiveBytes FAIL992");
            return cSerialRetError;
         }
         goto RestartComm;
      }
      else
      {
         // The read completed successfully.
         mReadPending = false;
      }
   }
   // At this point the read completed successfully. If the read and comm both 
   // completed at the same then the comm will no longer be pending.

   if (mReadPending)
   {
      // Should never get here.
      Trc::write(mTI, 0, "SerialPort::doReceiveBytes FAIL996");
      return cSerialRetError;
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

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Done. The read completed successfully. The comm event might have
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

