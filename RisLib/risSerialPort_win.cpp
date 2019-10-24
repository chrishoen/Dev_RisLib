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
   mTerminateFlag = false;
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

bool SerialPort::isValid(){return mValidFlag;}

//******************************************************************************
//******************************************************************************
//******************************************************************************

bool SerialPort::doOpen()
{
   mValidFlag=false;

   TS::print(1, "SerialPort::doOpen %s", mSettings.mPortDevice);

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
         TS::print(1, "serial_open_error_1 %d", GetLastError());
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
      TS::print(1, "serial_create_error_2 %d", GetLastError());
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
            TS::print(1, "serial_create_retry %d", GetLastError());
            doPurge();
            Sleep(100);
            // Retry failed, abort initialization
            if (count++ == 10)
            {
               Prn::print(Prn::SerialErrorP1, "serial_create_error_3 %d", GetLastError());
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
      TS::print(1, "serial_create_error_4 %d", GetLastError());
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

   TS::print(1, "SerialPort initialize PASS  $ %s : %16s",
      mSettings.mPortDevice,
      mSettings.mPortSetup);

   return true;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void SerialPort::doClose()
{
   mTerminateFlag = true;
   if (mValidFlag)
   {
      TS::print(1, "SerialPort::doClose %s", mSettings.mPortDevice);
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

void SerialPort::doPurge()
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

int SerialPort::doSendBytes(char* aData, int aNumBytes)
{
   TS::print(4, "SerialPort::doSendBytes START %d", aNumBytes);
   // Guard.
   if (!isValid()) return cRetCodeError;

   TS::PrintLevel tPrintLevel;
   TS::getProgramPrintLevel(tPrintLevel);

   // Local variables.
   DWORD tNumWritten;
   DWORD tRet  = 0;

   OVERLAPPED tOverlapped = {0};

   tOverlapped.hEvent = mSpecific->mTxEventHandle;

   // Write bytes to the port.
   if (WriteFile(mSpecific->mPortHandle, aData, aNumBytes, &tNumWritten, &tOverlapped))
   {
      // Write was successful.
      TS::print(4, "SerialPort::doSendBytes PASS1 %d",aNumBytes);
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
            TS::print(4, "SerialPort::doSendBytes PASS1 %d",aNumBytes);
            return 0;
         }
         else
         {
            TS::print(1, "ERROR SerialPort::doSendBytes ERROR 101, %d", GetLastError());
            return cRetCodeError;
         }
      }
      break;
      default:
      {
         TS::print(1, "ERROR SerialPort::doSendBytes ERROR 102, %d", GetLastError());
         return cRetCodeError;
      }
      break;
   }
   TS::print(4, "SerialPort::doSendBytes PASS3 %d",aNumBytes);
   return 0;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Send data, null terminated string, append an end of line LF (\n,10)

int SerialPort::doSendLine(const char *aData)
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

int SerialPort::doSendOne(char aData)
{
   return doSendBytes(&aData,1);   
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Receive a string, terminated with end of line LF (\n,10) or 
// CRLF (\r\n,13,10). Trims the  terminator and returns a null terminated
// string. Termination mode is determined by settings.

int SerialPort::doReceiveLine(char* aData, int aMaxNumBytes)
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

int SerialPort::doReceiveLineLF(char* aData, int aMaxNumBytes)
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

int SerialPort::doReceiveLineCRLF(char* aData, int aMaxNumBytes)
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

int  SerialPort::doReceiveOne(char *aData)
{
   return doReceiveBytes(aData, 1);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Receive bytes.

int SerialPort::doReceiveBytes(char *aData, int aNumBytes)
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
         TS::print(1, "SerialPort::doReceiveBytes ERROR 101 %d", GetLastError());
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
               TS::print(1, "ERROR SerialPort::doReceiveBytes ERROR 102 %d", GetLastError());
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
            TS::print(1, "ERROR SerialPort::doReceiveBytes TIMEOUT %d", GetLastError());
            return cRetCodeError;
         }
         default:
         {
            TS::print(1, "ERROR SerialPort::doReceiveBytes ERROR 104 %d", GetLastError());
            return cRetCodeError;
         }
      }
   }


   // If the read was aborted then clear hardware error.
   if (GetLastError() == ERROR_OPERATION_ABORTED)
   {
      TS::print(1, "ERROR SerialPort::doReceiveBytes ERROR 105 %d", GetLastError());
      ClearCommError(mSpecific->mPortHandle, 0, 0);
      return cRetCodeError;
   }

   // If the number of bytes read was wrong then error.
   if (tBytesRead != aNumBytes)
   {
      TS::print(1, "ERROR SerialPort::doReceiveBytes ERROR 106 %d", GetLastError());
      return cRetCodeTimeout;
   }

   // Done.
   TS::print(1, "SerialPort::doReceiveBytes PASS1 %d", tBytesRead);
   return tBytesRead;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Return the number of bytes that are available to receive.

int SerialPort::getAvailableReceiveBytes()
{
   COMSTAT tComStat;
   ClearCommError(mSpecific->mPortHandle, 0, &tComStat);
   return (int)tComStat.cbInQue;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace

