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
   HANDLE mRxEventHandle;
   HANDLE mTxEventHandle;
};

//******************************************************************************
//******************************************************************************
//******************************************************************************

SerialPort::SerialPort()
{
   mSpecific = new Specific;
   mSpecific->mPortHandle = 0;
   mSpecific->mRxEventHandle = 0;
   mSpecific->mTxEventHandle = 0;
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
      Trc::write(mTI, 0, "SerialPort::doOpen ERROR already open");
      return false;
   }

   mValidFlag = false;
   mAbortFlag = false;


   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Create events.

   mSpecific->mRxEventHandle = CreateEvent(NULL, TRUE, FALSE, NULL);
   mSpecific->mTxEventHandle = CreateEvent(NULL, TRUE, FALSE, NULL);

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
      Trc::write(mTI, 0, "SerialPort::doOpen ERROR 1");
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
               Trc::write(mTI, 0, "SerialPort::doOpen ERROR retry");
               return false;
            }
         }
      }
   }

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Set timeout.

   COMMTIMEOUTS tComTimeout={0};

   tComTimeout.ReadIntervalTimeout         = 0;
   tComTimeout.ReadTotalTimeoutMultiplier  = 0;
   tComTimeout.ReadTotalTimeoutConstant    = 0;
   tComTimeout.WriteTotalTimeoutMultiplier = 0;
   tComTimeout.WriteTotalTimeoutConstant   = 0;

   if (mSettings.mRxTimeout > 0)
   {
      tComTimeout.ReadIntervalTimeout = mSettings.mRxTimeout;
      tComTimeout.ReadTotalTimeoutMultiplier = mSettings.mRxTimeout;
      tComTimeout.ReadTotalTimeoutConstant = mSettings.mRxTimeout;
   }

   if(!SetCommTimeouts(mSpecific->mPortHandle, &tComTimeout))
   {
      CloseHandle(mSpecific->mPortHandle);
      mSpecific->mPortHandle=INVALID_HANDLE_VALUE;
      mPortErrorCount++;
      Trc::write(mTI, 0, "SerialPort::doOpen ERROR 4");
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
   printf("SerialPort open PASS  %s\n", mSettings.mPortDevice);
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
      Trc::write(mTI, 0, "SerialPort::doClose ERROR not open");
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
   SetEvent(mSpecific->mTxEventHandle);

   // Cancel any pending i/o and close the handles.
   CancelIoEx(mSpecific->mPortHandle, 0);

   CloseHandle(mSpecific->mPortHandle);
   CloseHandle(mSpecific->mRxEventHandle);
   CloseHandle(mSpecific->mTxEventHandle);
   mSpecific->mPortHandle = INVALID_HANDLE_VALUE;
   mSpecific->mRxEventHandle = INVALID_HANDLE_VALUE;
   mSpecific->mTxEventHandle = INVALID_HANDLE_VALUE;
   mValidFlag = false;
   Trc::write(mTI, 0, "SerialPort::doClose done");
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Abort pending serial port receive.
// 
// Set the abort flag and write bytes to the event semaphore. This will
// cause a pending receive call poll to return and the receive call will
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
   SetEvent(mSpecific->mRxEventHandle);
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
// Receive any available bytes. Block until at least one byte has
// been received. Return the number of bytes received or a negative
// error code. Copy the bytes into the pointer argument.
// THERE'S SOMETHING WRONG WITH THIS CODE.

int SerialPort::doReceiveAnyBytes(char *aData, int aNumBytes)
{
   printf("START SerialPort::doReceiveAnyBytes %d\n", aNumBytes);
   // Locals.
   DWORD tRet = 0;
   int tError = 0;
   DWORD tBytesRead = 0;
   int tBytesTotal = 0;
   bool tReadSuccessful = false;
   bool tWaitingOnRead = false;
   OVERLAPPED tOverlapped = { 0 };
   tOverlapped.hEvent = mSpecific->mRxEventHandle;
   DWORD tEventMask = 0;
   int tBytesAvailable = 0;

   // Wait for receive comm event.
   tRet = SetCommMask(mSpecific->mPortHandle, EV_RXCHAR);
   if (tRet == 0)
   {
      printf("SerialPort::doReceiveAnyBytes ERROR 101 %d\n", GetLastError());
      return cSerialRetError;
   }
   tRet = WaitCommEvent(mSpecific->mPortHandle, &tEventMask, NULL);
   if (tRet == 0)
   {
      tError = GetLastError();
      printf("SerialPort::doReceiveAnyBytes ERROR 102 %d\n", tError);
      if (tError == 995)
      {
         return cSerialRetAbort;
      }
      else
      {
         return cSerialRetError;
      }
   }

   // Read available bytes.
   tBytesAvailable = doGetAvailableReceiveBytes();
   printf("AVAILABLE1 %d\n", tBytesAvailable);
   tRet = ReadFile(mSpecific->mPortHandle, aData, tBytesAvailable, &tBytesRead, &tOverlapped);
   printf("AVAILABLE2 %d\n", doGetAvailableReceiveBytes());

   if (!tRet)
   {
      if (GetLastError() == ERROR_IO_PENDING)
      {
         tWaitingOnRead = true;
      }
      else
      {
         printf("SerialPort::doReceiveAnyBytes ERROR 101 %d\n", GetLastError());
         return cSerialRetError;
      }
   }
   else
   {
      tWaitingOnRead = false;
      tReadSuccessful = true;
   }

   if (tWaitingOnRead)
   {
      printf("WAIT1 SerialPort::doReceiveAnyBytes\n");
      // Wait for overlapped i/o completion.
      tRet = WaitForSingleObject(tOverlapped.hEvent, -1);

      printf("WAIT2 SerialPort::doReceiveAnyBytes %d\n", tRet);

      // Test for abort.
      if (mAbortFlag)
      {
         return cSerialRetAbort;
      }

      // Select on the returned status code.
      switch (tRet)
      {
         // Read completed.
      case WAIT_OBJECT_0:
      {
         if (!GetOverlappedResult(mSpecific->mPortHandle, &tOverlapped, &tBytesRead, FALSE))
         {
            tError = GetLastError();
            printf("ERROR SerialPort::doReceiveAnyBytes ERROR 102 %d\n", tError);
            if (tError == 995)
            {
               return cSerialRetAbort;
            }
            else
            {
               return cSerialRetError;
            }
         }
         else
         {
            tReadSuccessful = true;
         }
         tWaitingOnRead = false;
      }
      break;
      // Read timeput.
      case WAIT_TIMEOUT:
      {
         printf("ERROR SerialPort::doReceiveAnyBytes TIMEOUT %d\n", GetLastError());
         return cSerialRetError;
      }
      default:
      {
         printf("ERROR SerialPort::doReceiveAnyBytes ERROR 104 %d\n", GetLastError());
         return cSerialRetError;
      }
      }
   }


   // If the read was aborted then clear hardware error.
   if (GetLastError() == ERROR_OPERATION_ABORTED)
   {
      printf("ERROR SerialPort::doReceiveAnyBytes ERROR 105 %d\n", GetLastError());
      ClearCommError(mSpecific->mPortHandle, 0, 0);
      return cSerialRetError;
   }

   // If the number of bytes read was wrong then error.
   if (tBytesRead != tBytesAvailable)
   {
      printf("ERROR SerialPort::doReceiveAnyBytes ERROR 106 %d %d\n", GetLastError(), tBytesRead);
      return cSerialRetError;
   }

   // Done.
   printf("SerialPort::doReceiveAnyBytes PASS1 %d\n", tBytesRead);
   return tBytesRead;
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
   int tError = 0;
   DWORD tBytesRead = 0;
   int tBytesTotal = 0;
   bool tReadSuccessful = false;
   bool tWaitingOnRead = false;
   OVERLAPPED tOverlapped = { 0 };
   tOverlapped.hEvent = mSpecific->mRxEventHandle;

   // Issue read operation.
   tRet = ReadFile(mSpecific->mPortHandle, aData, aRequestBytes, &tBytesRead, &tOverlapped);

   if (!tRet)
   {
      if (GetLastError() == ERROR_IO_PENDING)
      {
         tWaitingOnRead = true;
      }
      else
      {
         mPortErrorCount++;
         Trc::write(mTI, 0, "SerialPort::doReceiveAllBytes ERROR 1");
         return cSerialRetError;
      }
   }
   else
   {
      tWaitingOnRead = false;
      tReadSuccessful = true;
   }

   if (tWaitingOnRead)
   {
      // Wait for overlapped i/o completion.
      tRet = WaitForSingleObject(tOverlapped.hEvent, -1);

      // Test for abort.
      if (mAbortFlag)
      {
         return cSerialRetAbort;
      }

      // Select on the returned status code.
      switch (tRet)
      {
         // Read completed.
      case WAIT_OBJECT_0:
      {
         if (!GetOverlappedResult(mSpecific->mPortHandle, &tOverlapped, &tBytesRead, FALSE))
         {
            tError = GetLastError();
            if (tError == 995)
            {
               return cSerialRetEmpty;
               return cSerialRetAbort;
            }
            else
            {
               mPortErrorCount++;
               Trc::write(mTI, 0, "SerialPort::doReceiveAllBytes ERROR 2");
               return cSerialRetError;
            }
         }
         else
         {
            tReadSuccessful = true;
         }
         tWaitingOnRead = false;
      }
      break;
      // Read timeput.
      case WAIT_TIMEOUT:
      {
         Trc::write(mTI, 0, "SerialPort::doReceiveAllBytes TIMEOUT");
         printf("serial_poll_error_2 timeout\n");
         return cSerialRetTimeout;
      }
      break;
      default:
      {
         mPortErrorCount++;
         Trc::write(mTI, 0, "SerialPort::doReceiveAllBytes ERROR 3");
         return cSerialRetError;
      }
      break;
      }
   }
   else
   {
   }


   // If the read was aborted then clear hardware error.
   if (GetLastError() == ERROR_OPERATION_ABORTED)
   {
      ClearCommError(mSpecific->mPortHandle, 0, 0);
      mPortErrorCount++;
      Trc::write(mTI, 0, "SerialPort::doReceiveAllBytes ERROR 4");
      return cSerialRetError;
   }

   // If the number of bytes read was wrong then error.
   if (tBytesRead != aRequestBytes)
   {
      mPortErrorCount++;
      Trc::write(mTI, 0, "SerialPort::doReceiveAllBytes ERROR 5 %d timeout", tBytesRead);
      printf("serial_poll_error_5 timeout\n");
      return cSerialRetError;
   }

   // Done.
   mRxByteCount += tBytesRead;
   Trc::write(mTI, 0, "SerialPort::doReceiveAllBytes done");
   return tBytesRead;
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

   // Local variables.
   DWORD tRet = 0;
   DWORD tNumWritten = 0;
   OVERLAPPED tOverlapped = { 0 };
   tOverlapped.hEvent = mSpecific->mTxEventHandle;

   ClearCommError(mSpecific->mPortHandle, 0, 0);

   // Write bytes to the port.
   if (WriteFile(mSpecific->mPortHandle, aData, aNumBytes, &tNumWritten, &tOverlapped))
   {
      // Write was successful.
      mTxByteCount += tNumWritten;
      Trc::write(mTI, 0, "doSendBytes::doSendBytes done 1");
      return tNumWritten;
   }

   // Write is pending.
// tRet = WaitForSingleObject(tOverlapped.hEvent, INFINITE);
   tRet = WaitForSingleObject(tOverlapped.hEvent, 1000);
   switch (tRet)
   {
      // OVERLAPPED structure's event has been signaled. 
   case WAIT_OBJECT_0:
   {
      if (GetOverlappedResult(mSpecific->mPortHandle, &tOverlapped, &tNumWritten, FALSE))
      {
         mTxByteCount += tNumWritten;
         Trc::write(mTI, 0, "doSendBytes::doSendBytes done 2");
         return tNumWritten;
      }
      else
      {
         mPortErrorCount++;
         Trc::write(mTI, 0, "SerialPort::doSendBytes ERROR 1");
         return cSerialRetError;
      }
   }
   break;
   case WAIT_TIMEOUT:
   {
      mPortErrorCount++;
      Trc::write(mTI, 0, "SerialPort::doSendBytes TIMEOUT");
      printf("serial port send timeout\n");
      return cSerialRetTimeout;
   }
   break;
   default:
   {
      mPortErrorCount++;
      Trc::write(mTI, 0, "SerialPort::doSendBytes ERROR 2");
      return cSerialRetError;
   }
   break;
   }
   mTxByteCount += tNumWritten;
   Trc::write(mTI, 0, "doSendBytes::doSendBytes done 3");
   return 0;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace

