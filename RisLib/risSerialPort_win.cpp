//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "stdafx.h"

#include <Windows.h>

#include "my_functions.h"
#include "risPortableCalls.h"
#include "prnPrint.h"

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
   mValidFlag = false;
   mAbortFlag = false;
}

SerialPort::~SerialPort(void)
{
   doClose();
   delete mSpecific;
}

void SerialPort::initialize(SerialSettings& aSettings)
{
   mSettings = aSettings;
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

   // Close the serial port, if already open.
   if (mValidFlag)
   {
      doClose();
      return true;
   }

   mAbortFlag = false;

   printf("SerialPort::doOpen %s\n", mSettings.mPortDevice);

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
      printf("serial_open_error_1 %d %s\n", GetLastError(), mSettings.mPortDevice);
      return false;
   }

   SetupComm(mSpecific->mPortHandle,2048,2048);

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Purge 

   Sleep(100);

   doFlush();

   if (GetLastError() != ERROR_SUCCESS)
   {
      printf("serial_create_error_2 %d\n", GetLastError());
      CloseHandle(mSpecific->mPortHandle);
      mSpecific->mPortHandle=INVALID_HANDLE_VALUE;
      return false; 
   }
 
   Sleep(100);

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
            printf("serial_create_retry %d\n", GetLastError());
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
   tComTimeout.ReadTotalTimeoutConstant    = mSettings.mRxTimeout;
   tComTimeout.WriteTotalTimeoutMultiplier = 0;
   tComTimeout.WriteTotalTimeoutConstant   = 0;

   if(!SetCommTimeouts(mSpecific->mPortHandle, &tComTimeout))
   {
      printf("serial_create_error_4 %d\n", GetLastError());
      CloseHandle(mSpecific->mPortHandle);
      mSpecific->mPortHandle=INVALID_HANDLE_VALUE;
      return false;
   }

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Purge.

   Sleep(100);
   doFlush();

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Done.
 
   mValidFlag = true;
#if 0
   printf("SerialPort doOpen PASS  $ %s : %16s\n",
      mSettings.mPortDevice,
      mSettings.mPortSetup);
#endif
   return true;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void SerialPort::doClose()
{
   if (mValidFlag)
   {
      //printf("SerialPort::doClose %s\n", mSettings.mPortDevice);
      CancelIoEx(mSpecific->mPortHandle,0);
      CloseHandle(mSpecific->mPortHandle);
      CloseHandle(mSpecific->mRxEventHandle);
      CloseHandle(mSpecific->mTxEventHandle);
      mSpecific->mPortHandle = INVALID_HANDLE_VALUE;
      mSpecific->mRxEventHandle = INVALID_HANDLE_VALUE;
      mSpecific->mTxEventHandle = INVALID_HANDLE_VALUE;
      mValidFlag = false;
   } 
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
   doClose();
   return;

   mAbortFlag = true;
   SetEvent(mSpecific->mRxEventHandle);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Flush serial port buffers.

void SerialPort::doFlush()
{
   ClearCommError(mSpecific->mPortHandle,0,0);

   DWORD lFlags;

   lFlags = 
      PURGE_RXABORT | PURGE_RXCLEAR |
      PURGE_TXABORT | PURGE_TXCLEAR;

   PurgeComm(mSpecific->mPortHandle, lFlags);

}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Send fixed number of bytes

int SerialPort::doSendBytes(const char* aData, int aNumBytes)
{
   // Guard.
   if (!mValidFlag) return cSerialRetError;

   // Local variables.
   DWORD tNumWritten;
   DWORD tRet  = 0;

   OVERLAPPED tOverlapped = {0};

   tOverlapped.hEvent = mSpecific->mTxEventHandle;

   // Write bytes to the port.
   if (WriteFile(mSpecific->mPortHandle, aData, aNumBytes, &tNumWritten, &tOverlapped))
   {
      // Write was successful.
      //printf("SerialPort::doSendBytes PASS1 %d\n",aNumBytes);
      return 0;
   }

   // Write is pending.
   tRet = WaitForSingleObject(tOverlapped.hEvent, INFINITE);
   switch(tRet)
   {
      // OVERLAPPED structure's event has been signaled. 
      case WAIT_OBJECT_0:
      {
         if (GetOverlappedResult(mSpecific->mPortHandle, &tOverlapped, &tNumWritten, FALSE))
         {
            //printf("SerialPort::doSendBytes PASS1 %d\n",aNumBytes);
            return 0;
         }
         else
         {
            printf("ERROR SerialPort::doSendBytes ERROR 101, %d\n", GetLastError());
            return cSerialRetError;
         }
      }
      break;
      default:
      {
         printf("ERROR SerialPort::doSendBytes ERROR 102, %d\n", GetLastError());
         return cSerialRetError;
      }
      break;
   }
   //printf("SerialPort::doSendBytes PASS3 %d\n",aNumBytes);
   return 0;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Receive bytes.

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
// Receive bytes.

int SerialPort::doReceiveAllBytes(char* aData, int aRequestBytes)
{
   //printf("START SerialPort::doReceiveAllBytes %d\n", aRequestBytes);
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
      default:
      {
         printf("ERROR SerialPort::doReceiveAllBytes ERROR 104 %d\n", GetLastError());
         return cSerialRetError;
      }
      }
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
// printf("SerialPort::doReceiveAllBytes PASS1 %d\n", tBytesRead);
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
}//namespace

