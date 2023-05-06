//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "stdafx.h"

#include <Windows.h>

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
};

//******************************************************************************
//******************************************************************************
//******************************************************************************

SerialPort::SerialPort()
{
   mSpecific = new Specific;
   mSpecific->mPortHandle = 0;
   mSpecific->mReadCompletion = 0;
   mSpecific->mWriteCompletion = 0;
   mOpenFlag = false;
   mValidFlag = false;
   mAbortFlag = false;
   mTI = 0;
   mOpenErrorShowCount = 0;
   mCloseErrorShowCount = 0;
   mRxByteCount = 0;
   mTxByteCount = 0;
   mPortErrorCount = 0;
}

SerialPort::~SerialPort(void)
{
   //doClose();
   delete mSpecific;
}

void SerialPort::initialize(SerialSettings& aSettings)
{
   mSettings = aSettings;
   mTI = aSettings.mTraceIndex;
   mOpenErrorShowCount = 0;
   mCloseErrorShowCount = 0;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

bool SerialPort::doOpen()
{
   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Do this first.

   Trc::write(mTI, 0, "SerialPort::doOpen");

   // Guard.
   if (mOpenFlag)
   {
      mPortErrorCount++;
      Trc::write(mTI, 0, "SerialPort::doOpen FAIL already open");
      return false;
   }

   mValidFlag = false;
   mAbortFlag = false;


   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Create overlapped io completion events.

   mSpecific->mReadCompletion = CreateEvent(NULL, TRUE, FALSE, NULL);
   mSpecific->mWriteCompletion = CreateEvent(NULL, TRUE, FALSE, NULL);

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Create file.

   mSpecific->mPortHandle = CreateFile(mSettings.mPortDevice,
      GENERIC_READ | GENERIC_WRITE,
      0,
      0,
      OPEN_EXISTING,
      FILE_FLAG_OVERLAPPED,
      0);

   if (mSpecific->mPortHandle == INVALID_HANDLE_VALUE)
   {
      if (mOpenErrorShowCount == 0)
      {
         printf("serial_open_error_1 %s %d %s\n", mSettings.mPortDevice, errno, strerror(errno));
      }
      mOpenErrorShowCount++;
      mPortErrorCount++;
      Trc::write(mTI, 0, "SerialPort::doOpen FAIL1");
      return false;
   }

   // Setup buffers.
   SetupComm(mSpecific->mPortHandle,0x20000, 0x20000);

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // DCB

   // If the port setup string is not empty then setup with a dcb.
   if (strlen(mSettings.mPortSetup))
   {
      DCB dcb;

      memset(&dcb, 0, sizeof(dcb));
      dcb.DCBlength = sizeof(dcb);

      GetCommState(mSpecific->mPortHandle, &dcb);

      BuildCommDCB(mSettings.mPortSetup, &dcb);

      dcb.fNull = FALSE;
      dcb.fAbortOnError = TRUE;

      // SetCommState
      // This might not work if data is being received while initializing.
      // So loop, retry if not successful

      int  count = 1;
      bool going = true;
      while (going)
      {
         if (SetCommState(mSpecific->mPortHandle, &dcb))
         {
            // Successful, exit loop
            going = false;
         }
         else
         {
            // Failed, continue to retry
            mPortErrorCount++;
            Trc::write(mTI, 0, "SerialPort::doOpen retry");
            doFlush();
            Sleep(100);
            // Retry failed, abort initialization
            if (count++ == 10)
            {
               CloseHandle(mSpecific->mPortHandle);
               mSpecific->mPortHandle = INVALID_HANDLE_VALUE;
               mPortErrorCount++;
               Trc::write(mTI, 0, "SerialPort::doOpen FAIL retry");
               return false;
            }
         }
      }
   }

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Because this scheme is using overlapped io, set timeouts to infinite.
   // The timeouts will be managed by the overlapped io.

   COMMTIMEOUTS tCommTimeouts;
   memset(&tCommTimeouts, 0, sizeof(tCommTimeouts));

   if(!SetCommTimeouts(mSpecific->mPortHandle, &tCommTimeouts))
   {
      CloseHandle(mSpecific->mPortHandle);
      mSpecific->mPortHandle=INVALID_HANDLE_VALUE;
      mPortErrorCount++;
      Trc::write(mTI, 0, "SerialPort::doOpen FAIL2");
      return false;
   }

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Purge.

   doFlush();

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Done.
 
   mOpenFlag = true;
   mValidFlag = true;
   mOpenErrorShowCount = 0;
   mCloseErrorShowCount = 0;
   printf("SerialPort open    PASS          %s\n", mSettings.mPortDevice);
   Trc::write(mTI, 0, "SerialPort::doOpen done");
   return true;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void SerialPort::doClose()
{
   Trc::write(mTI, 0, "SerialPort::doClose");

   // Test if already closed.
   if (!mOpenFlag)
   {
      mPortErrorCount++;
      Trc::write(mTI, 0, "SerialPort::doClose FAIL not open");
      return;
   }

   int tRet = 0;

   // Set invalid.
   mOpenFlag = false;
   mValidFlag = false;

   // Test handle.
   if (mSpecific->mPortHandle == INVALID_HANDLE_VALUE)
   {
      mPortErrorCount++;
      Trc::write(mTI, 0, "SerialPort::doClose INVALID HANDLE");
      return;
   }

   // Flush.
   doFlush();

   // Unblock any pending writes.
   SetEvent(mSpecific->mWriteCompletion);

   // Cancel any pending i/o.
   CancelIoEx(mSpecific->mPortHandle, 0);

   // Close the handles.
   CloseHandle(mSpecific->mPortHandle);
   CloseHandle(mSpecific->mReadCompletion);
   CloseHandle(mSpecific->mWriteCompletion);
   mSpecific->mPortHandle = INVALID_HANDLE_VALUE;
   mSpecific->mReadCompletion = INVALID_HANDLE_VALUE;
   mSpecific->mWriteCompletion = INVALID_HANDLE_VALUE;
   mValidFlag = false;
   Trc::write(mTI, 0, "SerialPort::doClose done");
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Abort pending serial port receive.
// 
// Set the abort flag and write bytes to the event semaphore. This will
// cause a pending receive call to return and the receive call will
// abort because the abort flag is set.

void SerialPort::doAbort()
{
   Trc::write(mTI, 0, "SerialPort::doAbort");

   // Set the abort flag.
   mAbortFlag = true;

   // Guard.
   int tRet = 0;
   if (!mOpenFlag)
   {
      Trc::write(mTI, 0, "SerialPort::doAbort not open");
      return;
   }

   // Guard.
   if (!mValidFlag)
   {
      Trc::write(mTI, 0, "SerialPort::doAbort INVALID");
      return;
   }

   // Post to the event semaphore.
   SetEvent(mSpecific->mReadCompletion);
   Trc::write(mTI, 0, "SerialPort::doAbort done");
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Flush serial port buffers.

void SerialPort::doFlush()
{
   Trc::write(mTI, 0, "SerialPort::doFlush");

   ClearCommError(mSpecific->mPortHandle,0,0);

   DWORD lFlags = 
      PURGE_RXABORT | PURGE_RXCLEAR |
      PURGE_TXABORT | PURGE_TXCLEAR;

   PurgeComm(mSpecific->mPortHandle, lFlags);

   ClearCommError(mSpecific->mPortHandle, 0, 0);

   Trc::write(mTI, 0, "SerialPort::doFlush done");
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Return the number of bytes that are available to receive.

int SerialPort::doGetAvailableReceiveBytes()
{
   COMSTAT tComStat;
   ClearCommError(mSpecific->mPortHandle, 0, &tComStat);
   return (int)tComStat.cbInQue;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// This doesn't work.

int SerialPort::doReceiveAnyBytes(char *aData, int aNumBytes)
{
   return 0;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Receive a requested number of bytes. Block until all of the bytes
// have been received. Return the number of bytes received or a
// negative error code. Copy the bytes into the pointer argument.

int SerialPort::doReceiveAllBytes(char* aData, int aRequestBytes)
{
   Trc::write(mTI, 0, "SerialPort::doReceiveAllBytes");

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
// Receive one byte. Block until the byte has been received. Return
// one or zero or a negative error code. Copy the byte into the
// pointer argument.

int SerialPort::doReceiveOneByte(char* aByte)
{
   return doReceiveAllBytes(aByte, 1);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Send a fixed number of bytes. Return the actual number of bytes
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

   // Clear errors.
   ClearCommError(mSpecific->mPortHandle, 0, 0);

   // Locals.
   DWORD tRet = 0;
   DWORD tNumWritten = 0;
   DWORD tNumToWrite = aNumBytes;
   bool tWaitForCompletion = false;
   OVERLAPPED tOverlapped;
   memset(&tOverlapped, 0, sizeof(tOverlapped));
   tOverlapped.hEvent = mSpecific->mWriteCompletion;
   DWORD tWaitTimeout = 1000;

   // Issue write operation, overlapped i/o.
   tRet = WriteFile(mSpecific->mPortHandle, aData, tNumToWrite, &tNumWritten, &tOverlapped);

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
         ClearCommError(mSpecific->mPortHandle, 0, 0);
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
      tRet = WaitForSingleObject(tOverlapped.hEvent, tWaitTimeout);

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
         printf("serial_poll_error_2 timeout\n");
         return cSerialRetTimeout;
      }
      break;
      case WAIT_OBJECT_0:
      {
         // Check overlapped result for abort or for errors.
         if (!GetOverlappedResult(mSpecific->mPortHandle, &tOverlapped, &tNumWritten, FALSE))
         {
            if (GetLastError() == ERROR_OPERATION_ABORTED)
            {
               ClearCommError(mSpecific->mPortHandle, 0, 0);
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

   // Success.
   mTxByteCount += tNumWritten;
   Trc::write(mTI, 0, "SerialPort::doSendBytes done");
   return tNumWritten;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace

