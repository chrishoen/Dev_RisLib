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
   mTerminateFlag = false;
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

bool SerialPort2::isValid(){return mValidFlag;}

//******************************************************************************
//******************************************************************************
//******************************************************************************

bool SerialPort2::doOpen()
{
   mValidFlag=false;

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


   while (1)
   {
      if (mTerminateFlag) return false;

      mSpecific->mPortHandle = CreateFile(mSettings.mPortDevice,
         GENERIC_READ | GENERIC_WRITE,
         0,
         0,
         OPEN_EXISTING,
         FILE_FLAG_OVERLAPPED,
         0);

      if (mSpecific->mPortHandle == INVALID_HANDLE_VALUE)
      {
         //printf("serial_open_error_1 %d %s\n", GetLastError(), mSettings.mPortDevice);
         Sleep(2000);
      }
      else
      {
         break;
      }
   }

   SetupComm(mSpecific->mPortHandle,2048,2048);

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Purge 

   Sleep(100);

   doPurge();

   if (GetLastError() != ERROR_SUCCESS)
   {
      //printf("serial_create_error_2 %d\n", GetLastError());
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
            //printf("serial_create_retry %d\n", GetLastError());
            doPurge();
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
      //printf("serial_create_error_4 %d\n", GetLastError());
      CloseHandle(mSpecific->mPortHandle);
      mSpecific->mPortHandle=INVALID_HANDLE_VALUE;
      return false;
   }

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Purge.

   Sleep(100);
   doPurge();

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Done.
 
   mValidFlag = true;

   printf("SerialPort2 initialize PASS  $ %s : %16s\n",
      mSettings.mPortDevice,
      mSettings.mPortSetup);

   return true;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void SerialPort2::doClose()
{
   mTerminateFlag = true;
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
// Purge the comm channel

void SerialPort2::doPurge()
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
   if (!isValid()) return cRetCodeError;

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
            return cRetCodeError;
         }
      }
      break;
      default:
      {
         //printf("ERROR SerialPort2::doSendBytes ERROR 102, %d\n", GetLastError());
         return cRetCodeError;
      }
      break;
   }
   //printf("SerialPort2::doSendBytes PASS3 %d\n",aNumBytes);
   return 0;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Send data, null terminated string, append an end of line LF (\n,10)

int SerialPort2::doSendLine(const char *aData)
{
   // Copy the input string to a temp buffer and append a terminator.
   if (mSettings.mTermMode == cSerialTermMode_LF)
   {
      char tBuffer[200];
      strncpy(tBuffer, aData, 196);
      tBuffer[199] = 0;
      int tLength = (int)strlen(tBuffer);
      tBuffer[tLength] = '\n';
      tLength++;
      tBuffer[tLength] = 0;
      return doSendBytes(tBuffer, tLength);
   }
   else
   {
      char tBuffer[200];
      strncpy(tBuffer, aData, 195);
      tBuffer[199] = 0;
      int tLength = (int)strlen(tBuffer);
      tBuffer[tLength] = '\r';
      tLength++;
      tBuffer[tLength] = '\n';
      tLength++;
      tBuffer[tLength] = 0;
      return doSendBytes(tBuffer, tLength);
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Send one byte

int SerialPort2::doSendOne(char aData)
{
   return doSendBytes(&aData,1);   
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Receive a string, terminated with end of line LF (\n,10) or 
// CRLF (\r\n,13,10). Trims the  terminator and returns a null terminated
// string. Termination mode is determined by settings.

int SerialPort2::doReceiveLine(char* aData, int aMaxNumBytes)
{
   if (mSettings.mTermMode == cSerialTermMode_LF)
   {
      return doReceiveLineLF(aData, aMaxNumBytes);
   }
   else
   {
      return doReceiveLineCRLF(aData, aMaxNumBytes);
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Receive a string, terminated with end of line LF (\n,10).

int SerialPort2::doReceiveLineLF(char* aData, int aMaxNumBytes)
{
   int  tStatus = -1;
   int  tIndex = 0;
   int  tRxStatus = 0;
   char tRxChar = 0;
   bool tGoing = true;

   aData[0] = 0;

   // Loop to read single bytes, store them, and exit
   // when termination cr/lf is detected
   while (isValid() && tGoing)
   {
      // Read one byte
      tRxStatus = doReceiveOne(&tRxChar);
      if (tRxStatus >= 0)
      {
         // Read success
         // Store byte
         aData[tIndex] = tRxChar;

         // If CR
         if (tRxChar == 10)
         {
            // Terminator detected, strip if off
            tGoing = false;
            aData[tIndex] = 0;
            tStatus = tIndex;
         }
         if (tIndex == aMaxNumBytes - 1)
         {
            // NumBytes limit was reached
            tGoing = false;
            aData[tIndex] = 0;
            tStatus = tIndex;
         }

         // Increment
         tIndex++;
      }
      else
      {
         // Read failure
         tStatus = tRxStatus;
         tGoing = false;
      }
   }
   return tStatus;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Receive a string, terminated with end of line LF (\r\n,13,10).

int SerialPort2::doReceiveLineCRLF(char* aData, int aMaxNumBytes)
{
   int  tStatus = -1;
   int  tIndex = 0;
   int  tRxStatus = 0;
   char tRxChar = 0;
   char tLastRxChar = 0;
   bool tGoing = true;

   aData[0] = 0;

   // Loop to read single bytes, store them, and exit
   // when termination cr/lf is detected
   while (isValid() && tGoing)
   {
      // Save last received char.
      tLastRxChar = tRxChar;
      // Read one byte.
      tRxStatus = doReceiveOne(&tRxChar);
      if (tRxStatus >= 0)
      {
         // Read success.
         // Store byte.
         aData[tIndex] = tRxChar;

         // If CR
         if (tRxChar == 10 && tLastRxChar == 13)
         {
            // Terminator detected, strip if off
            tGoing = false;
            aData[tIndex - 1] = 0;
            tStatus = tIndex;
         }
         if (tIndex == aMaxNumBytes - 1)
         {
            // NumBytes limit was reached
            tGoing = false;
            aData[tIndex] = 0;
            tStatus = tIndex;
         }

         // Increment
         tIndex++;
      }
      else
      {
         // Read failure
         tStatus = tRxStatus;
         tGoing = false;
      }
   }
   return tStatus;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Receive one byte.

int  SerialPort2::doReceiveOne(char *aData)
{
   return doReceiveBytes(aData, 1);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Receive bytes.

int SerialPort2::doReceiveBytes(char *aData, int aNumBytes)
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
         return cRetCodeError;
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
               return cRetCodeError;
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
            return cRetCodeError;
         }
         default:
         {
            //printf("ERROR SerialPort2::doReceiveBytes ERROR 104 %d\n", GetLastError());
            return cRetCodeError;
         }
      }
   }


   // If the read was aborted then clear hardware error.
   if (GetLastError() == ERROR_OPERATION_ABORTED)
   {
      //printf("ERROR SerialPort2::doReceiveBytes ERROR 105 %d\n", GetLastError());
      ClearCommError(mSpecific->mPortHandle, 0, 0);
      return cRetCodeError;
   }

   // If the number of bytes read was wrong then error.
   if (tBytesRead != aNumBytes)
   {
      //printf("ERROR SerialPort2::doReceiveBytes ERROR 106 %d\n", GetLastError());
      return cRetCodeTimeout;
   }

   // Done.
   //printf("SerialPort2::doReceiveBytes PASS1 %d\n", tBytesRead);
   return tBytesRead;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Return the number of bytes that are available to receive.

int SerialPort2::getAvailableReceiveBytes()
{
   COMSTAT tComStat;
   ClearCommError(mSpecific->mPortHandle, 0, &tComStat);
   return (int)tComStat.cbInQue;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace

