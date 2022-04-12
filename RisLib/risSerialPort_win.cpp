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

   // Guard.
   if (mOpenFlag)
   {
      printf("serial_open_error already open\n");
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
         Trc::write(mTI, 0, "serial_open_error_1 %s %d %s", mSettings.mPortDevice, errno, strerror(errno));
      }
      mOpenErrorShowCount++;
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
            Trc::write(mTI, 0, "serial_create_retry %d", GetLastError());
            doFlush();
            Sleep(100);
            // Retry failed, abort initialization
            if (count++ == 10)
            {
               CloseHandle(mSpecific->mPortHandle);
               mSpecific->mPortHandle = INVALID_HANDLE_VALUE;
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

   tComTimeout.ReadIntervalTimeout = 0;
   tComTimeout.ReadTotalTimeoutMultiplier = 0;
   tComTimeout.ReadTotalTimeoutConstant = 0;
   tComTimeout.WriteTotalTimeoutMultiplier = 0;
   tComTimeout.WriteTotalTimeoutConstant = 2000;

   if(!SetCommTimeouts(mSpecific->mPortHandle, &tComTimeout))
   {
      Trc::write(mTI, 0, "serial_create_error_4 %d", GetLastError());
      CloseHandle(mSpecific->mPortHandle);
      mSpecific->mPortHandle=INVALID_HANDLE_VALUE;
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
 
   printf("SerialPort open PASS  %s\n", mSettings.mPortDevice);
   Trc::write(mTI, 0, "SerialPort open PASS  %s", mSettings.mPortDevice);
   mOpenFlag = true;
   mValidFlag = true;
   mOpenErrorShowCount = 0;
   mCloseErrorShowCount = 0;
   return true;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void SerialPort::doClose()
{
   // Test if already closed.
   if (!mOpenFlag)
   {
      printf("serial_close not open\n");
      return;
   }

   int tRet = 0;

   // Set invalid.
   mOpenFlag = false;
   mValidFlag = false;

   // Test handle.
   if (mSpecific->mPortHandle == INVALID_HANDLE_VALUE)
   {
      printf("serial_close invalid handle\n");
      return;
   }

   // Flush.
   Trc::write(mTI, 0, "serial_close flush");
   doFlush();
   SetEvent(mSpecific->mTxEventHandle);

   // Cancel any pending i/o and close the handles.
   Trc::write(mTI, 0, "serial_close cancel");
   CancelIoEx(mSpecific->mPortHandle, 0);

   Trc::write(mTI, 0, "serial_close handles");
   CloseHandle(mSpecific->mPortHandle);
   CloseHandle(mSpecific->mRxEventHandle);
   CloseHandle(mSpecific->mTxEventHandle);
   mSpecific->mPortHandle = INVALID_HANDLE_VALUE;
   mSpecific->mRxEventHandle = INVALID_HANDLE_VALUE;
   mSpecific->mTxEventHandle = INVALID_HANDLE_VALUE;
   mValidFlag = false;
   Trc::write(mTI, 1, "serial_close done");
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
   // Set the abort flag.
   mAbortFlag = true;

   // Guard.
   int tRet = 0;
   if (!mValidFlag) return;

   // Post to the event semaphore.
   SetEvent(mSpecific->mRxEventHandle);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Flush serial port buffers.

void SerialPort::doFlush()
{
   ClearCommError(mSpecific->mPortHandle,0,0);

   DWORD lFlags = 
      PURGE_RXABORT | PURGE_RXCLEAR |
      PURGE_TXABORT | PURGE_TXCLEAR;

   PurgeComm(mSpecific->mPortHandle, lFlags);

   ClearCommError(mSpecific->mPortHandle, 0, 0);
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
   //printf("SerialPort::doReceiveAllBytes START %d\n", aRequestBytes);
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
   Trc::write(mTI, 1, "SerialPort::doReceiveAllBytes READ begin");
   tRet = ReadFile(mSpecific->mPortHandle, aData, aRequestBytes, &tBytesRead, &tOverlapped);
   Trc::write(mTI, 1, "SerialPort::doReceiveAllBytes READ end 1");

   if (!tRet)
   {
      if (GetLastError() == ERROR_IO_PENDING)
      {
         tWaitingOnRead = true;
      }
      else
      {
         printf("SerialPort::doReceiveAllBytes ERROR 101 %d\n", GetLastError());
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
      Trc::write(mTI, 1, "SerialPort::doReceiveAllBytes READ end 2");

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
               printf("ERROR SerialPort::doReceiveAllBytes ERROR 102 %d\n", tError);
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
         printf("ERROR SerialPort::doReceiveAllBytes TIMEOUT %d\n", GetLastError());
         return cSerialRetError;
      }
      break;
      default:
      {
         printf("ERROR SerialPort::doReceiveAllBytes ERROR 104 %d\n", GetLastError());
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
      printf("ERROR SerialPort::doReceiveAllBytes ERROR 105 %d\n", GetLastError());
      ClearCommError(mSpecific->mPortHandle, 0, 0);
      return cSerialRetError;
   }

   // If the number of bytes read was wrong then error.
   if (tBytesRead != aRequestBytes)
   {
      printf("ERROR SerialPort::doReceiveAllBytes ERROR 106 %d %d\n", GetLastError(), tBytesRead);
      return cSerialRetError;
   }

   // Done.
   Trc::write(mTI, 1, "SerialPort::doReceiveAllBytes PASS1 %d", tBytesRead);
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
   // Guard.
   if (!mValidFlag) return cSerialRetError;

   ClearCommError(mSpecific->mPortHandle, 0, 0);

   // Local variables.
   DWORD tNumWritten;
   DWORD tRet = 0;

   OVERLAPPED tOverlapped = { 0 };

   tOverlapped.hEvent = mSpecific->mTxEventHandle;

   // Write bytes to the port.
   Trc::write(mTI, 1, "doSendBytes begin");
   if (WriteFile(mSpecific->mPortHandle, aData, aNumBytes, &tNumWritten, &tOverlapped))
   {
      // Write was successful.
      Trc::write(mTI, 1, "doSendBytes PASS1");
      return tNumWritten;
   }

   // Write is pending.
   Trc::write(mTI, 1, "doSendBytes wait begin");
   tRet = WaitForSingleObject(tOverlapped.hEvent, INFINITE);
   Trc::write(mTI, 1, "doSendBytes wait end");
   switch (tRet)
   {
      // OVERLAPPED structure's event has been signaled. 
   case WAIT_OBJECT_0:
   {
      if (GetOverlappedResult(mSpecific->mPortHandle, &tOverlapped, &tNumWritten, FALSE))
      {
         Trc::write(mTI, 1, "doSendBytes PASS2");
         //printf("SerialPort::doSendBytes PASS1 %d\n",aNumBytes);
         return tNumWritten;
      }
      else
      {
         Trc::write(mTI, 1, "ERROR SerialPort::doSendBytes ERROR 101, %d", GetLastError());
         printf("ERROR SerialPort::doSendBytes ERROR 101, %d\n", GetLastError());
         return cSerialRetError;
      }
   }
   break;
   default:
   {
      Trc::write(mTI, 1, "ERROR SerialPort::doSendBytes ERROR 102, %d", GetLastError());
      printf("ERROR SerialPort::doSendBytes ERROR 102, %d\n", GetLastError());
      return cSerialRetError;
   }
   break;
   }
   Trc::write(mTI, 1, "doSendBytes PASS3");
   return 0;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace

