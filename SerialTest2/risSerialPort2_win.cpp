//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "stdafx.h"

#include <Windows.h>

#include "my_functions.h"
#include "risPortableCalls.h"
#include "prnPrint.h"

#include "risSerialPort2.h"

namespace Ris
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Portable specifics.

class SerialPort2::Specific
{
public:
   HANDLE mPortHandle;
   HANDLE mRxEventHandle;
   HANDLE mTxEventHandle;
};

//******************************************************************************
//******************************************************************************
//******************************************************************************

SerialPort2::SerialPort2()
{
   mSpecific = new Specific;
   mSpecific->mPortHandle = 0;
   mSpecific->mRxEventHandle = 0;
   mSpecific->mTxEventHandle = 0;
   mValidFlag = false;
   mAbortFlag = false;
}

SerialPort2::~SerialPort2(void)
{
   doClose();
   delete mSpecific;
}

void SerialPort2::initialize(SerialSettings& aSettings)
{
   mSettings = aSettings;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

bool SerialPort2::doOpen()
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

   printf("SerialPort2::doOpen %s\n", mSettings.mPortDevice);

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

   printf("SerialPort2 doOpen PASS  $ %s : %16s\n",
      mSettings.mPortDevice,
      mSettings.mPortSetup);

   return true;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void SerialPort2::doClose()
{
   if (mValidFlag)
   {
      //printf("SerialPort2::doClose %s\n", mSettings.mPortDevice);
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

void SerialPort2::doAbort()
{
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Flush serial port buffers.

void SerialPort2::doFlush()
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

int SerialPort2::doSendBytes(const char* aData, int aNumBytes)
{
   //printf("SerialPort2::doSendBytes START %d\n", aNumBytes);
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
      //printf("SerialPort2::doSendBytes PASS1 %d\n",aNumBytes);
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
            //prinft("SerialPort2::doSendBytes PASS1 %d\n",aNumBytes);
            return 0;
         }
         else
         {
            //printf("ERROR SerialPort2::doSendBytes ERROR 101, %d\n", GetLastError());
            return cSerialRetError;
         }
      }
      break;
      default:
      {
         //printf("ERROR SerialPort2::doSendBytes ERROR 102, %d\n", GetLastError());
         return cSerialRetError;
      }
      break;
   }
   //printf("SerialPort2::doSendBytes PASS3 %d\n",aNumBytes);
   return 0;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Receive bytes.

int SerialPort2::doReceiveAnyBytes(char *aData, int aNumBytes)
{
   // Locals.
   DWORD tRet  = 0;
   DWORD tBytesRead  = 0;
   int tBytesTotal = 0;

   bool tReadSuccessful = false;
   bool tWaitingOnRead = false;
   OVERLAPPED tOverlapped = {0};

   tOverlapped.hEvent = mSpecific->mRxEventHandle;

   
   // Issue read operation.
   tRet = ReadFile(mSpecific->mPortHandle, aData, aNumBytes, &tBytesRead, &tOverlapped);

   if (!tRet)
   {
      if (GetLastError() == ERROR_IO_PENDING)
      {
         tWaitingOnRead = true;
      }
      else
      {
         //printf("SerialPort2::doReceiveBytes ERROR 101 %d\n", GetLastError());
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

      // Select on the returned status code.
      switch(tRet)
      {
         // Read completed.
         case WAIT_OBJECT_0:
         {
            if (!GetOverlappedResult(mSpecific->mPortHandle, &tOverlapped, &tBytesRead, FALSE))
            {
               //printf("ERROR SerialPort2::doReceiveBytes ERROR 102 %d\n", GetLastError());
               return cSerialRetError;
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
            //printf("ERROR SerialPort2::doReceiveBytes TIMEOUT %d\n", GetLastError());
            return cSerialRetError;
         }
         default:
         {
            //printf("ERROR SerialPort2::doReceiveBytes ERROR 104 %d\n", GetLastError());
            return cSerialRetError;
         }
      }
   }


   // If the read was aborted then clear hardware error.
   if (GetLastError() == ERROR_OPERATION_ABORTED)
   {
      //printf("ERROR SerialPort2::doReceiveBytes ERROR 105 %d\n", GetLastError());
      ClearCommError(mSpecific->mPortHandle, 0, 0);
      return cSerialRetError;
   }

   // If the number of bytes read was wrong then error.
   if (tBytesRead != aNumBytes)
   {
      //printf("ERROR SerialPort2::doReceiveBytes ERROR 106 %d\n", GetLastError());
      return cSerialRetTimeout;
   }

   // Done.
   //printf("SerialPort2::doReceiveBytes PASS1 %d\n", tBytesRead);
   return tBytesRead;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Receive a requested number of bytes. Block until all of the bytes
// have been received. Return the number of bytes received or a
// negative error code. Copy the bytes into the pointer argument.

int SerialPort2::doReceiveAllBytes(char* aBytes, int aRequestBytes)
{
   // Guard.
   if (!mValidFlag) return cSerialRetError;
   if (aRequestBytes == 0) return 0;
   int tRet = 0;

   // Loop to read all of the requested bytes.
   int tBytesRequired = aRequestBytes;
   int tBytesRemaining = aRequestBytes;
   int tBytesTotal = 0;

   bool tGoing = true;
   while (tGoing)
   {
      // Read any available receive bytes. Block until at least one byte
      // has been received. Return the number of bytes read or a negative
      // error code.
      tRet = doReceiveAnyBytes(&aBytes[tBytesTotal], tBytesRemaining);

      if (tRet > 0)
      {
         tBytesTotal += tRet;
         tBytesRemaining -= tRet;
         if (tBytesTotal == tBytesRequired)
         {
            tGoing = false;
            tRet = tBytesTotal;
         }
      }
      else
      {
         tGoing = false;
      }
   }

   // If success then return the total number of bytes read, which is 
   // equal to the requested number of bytes. If failure then return
   // the negative error code.
   return tRet;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Receive one byte. Block until the byte has been received. Return
// one or zero or a negative error code. Copy the byte into the
// pointer argument.

int SerialPort2::doReceiveOneByte(char* aByte)
{
   return doReceiveAnyBytes(aByte, 1);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Return the number of bytes that are available to receive.

int SerialPort2::doGetAvailableReceiveBytes()
{
   COMSTAT tComStat;
   ClearCommError(mSpecific->mPortHandle, 0, &tComStat);
   return (int)tComStat.cbInQue;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace

