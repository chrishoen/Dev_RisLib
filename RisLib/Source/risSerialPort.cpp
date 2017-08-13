//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "stdafx.h"

#include <stdio.h>

#include "prnPrint.h"
#include "risCallPointer.h"

#include "risSerialPort.h"

namespace Ris
{

//******************************************************************************
//******************************************************************************
//******************************************************************************

SerialPort::SerialPort()
{
   mPortHandle=0;
}

SerialPort::~SerialPort(void)
{
   doClose();
}

bool SerialPort::isValid(){return mValidFlag;}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void SerialPort::doOpen(int aPortNumber,char* aPortSetup,int aRxTimeout)
{
   mPortNumber = aPortNumber;
   strcpy(mPortSetup,aPortSetup);
   mValidFlag=false;

   Prn::print(Prn::SerialInit1,"SerialPort::doOpen %d",mPortNumber);

   //--------------------------------------------------------------------------
   // Create file
   char tSerialPortName[24];
   sprintf(tSerialPortName, "COM%u", mPortNumber);

   mPortHandle = CreateFile(tSerialPortName,  
     GENERIC_READ | GENERIC_WRITE, 
     0, 
     0, 
     OPEN_EXISTING,
     0,
     0);

   if (mPortHandle==INVALID_HANDLE_VALUE)
   {
      Prn::print(Prn::SerialInit2,"serial_create_error_1 %d", GetLastError());
      return;
   }

   SetupComm(mPortHandle,2048,2048);
   //--------------------------------------------------------------------------
   // Purge 

   Sleep(100);

   doPurge();

   if (GetLastError() != ERROR_SUCCESS)
   {
      Prn::print(Prn::SerialInit2,"serial_create_error_2 %d", GetLastError());
      CloseHandle(mPortHandle);
      mPortHandle=INVALID_HANDLE_VALUE;
      return;
   }
 
   Sleep(100);

   //--------------------------------------------------------------------------
   // DCB

   DCB dcb;

   memset(&dcb, 0, sizeof(dcb));
   dcb.DCBlength = sizeof(dcb);

   GetCommState(mPortHandle, &dcb);

   BuildCommDCB(mPortSetup, &dcb);
  
   dcb.fNull         = FALSE;
   dcb.fAbortOnError = TRUE;

   // SetCommState
   // This might not work if data is being received while initializing.
   // So loop, retry if not successful
   
   int  count=1;
   bool going=true;
   while (going)
   {
      if (SetCommState(mPortHandle, &dcb))
      {
         // Successful, exit loop
         going=false;
      }
      else
      {
         // Failed, continue to retry
         Prn::print(Prn::SerialInit2,"serial_create_retry %d", GetLastError());
         doPurge();
         Sleep(100);
         // Retry failed, abort initialization
         if(count++ == 10)
         {
            Prn::print(Prn::SerialInit2,"serial_create_error_3 %d", GetLastError());
            CloseHandle(mPortHandle);
            mPortHandle=INVALID_HANDLE_VALUE;
            return;
         }
      }
   }

   //--------------------------------------------------------------------------
   // Set timeout

   COMMTIMEOUTS tComTimeout={0};

   tComTimeout.ReadIntervalTimeout         = 0;
   tComTimeout.ReadTotalTimeoutMultiplier  = 0;
   tComTimeout.ReadTotalTimeoutConstant    = aRxTimeout;
   tComTimeout.WriteTotalTimeoutMultiplier = 0;
   tComTimeout.WriteTotalTimeoutConstant   = 0;

   if(!SetCommTimeouts(mPortHandle, &tComTimeout))
   {
      Prn::print(Prn::SerialInit2,"serial_create_error_4 %d", GetLastError());
      CloseHandle(mPortHandle);
      mPortHandle=INVALID_HANDLE_VALUE;
      return;
   }

   //--------------------------------------------------------------------------
   // Purge 

   Sleep(100);
   doPurge();

   //--------------------------------------------------------------------------
   // Done
 
   mValidFlag=true;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void SerialPort::doClose()
{
   if (mValidFlag)
   {
      Prn::print(Prn::SerialInit1,"SerialPort::doClose %d",mPortNumber);
      CancelIoEx(mPortHandle,0);
      CloseHandle(mPortHandle);
      mPortHandle = INVALID_HANDLE_VALUE;
      mValidFlag = false;
   } 
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Purge the comm channel

void SerialPort::doPurge()
{
   ClearCommError(mPortHandle,0,0);

   DWORD lFlags;

   lFlags = 
      PURGE_RXABORT | PURGE_RXCLEAR |
      PURGE_TXABORT | PURGE_TXCLEAR;

   PurgeComm(mPortHandle, lFlags);

}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Send fixed number of bytes

int SerialPort::doSendBytes(char* aData, int aNumBytes)
{
   DWORD tNumWritten;
   Prn::print(0,"LINE200");
   if (!isValid()) return cRetCodeError;
   Prn::print(0,"LINE201 %d",aNumBytes);

   if (WriteFile(mPortHandle, aData, aNumBytes, &tNumWritten, NULL))
   {
      Prn::print(0,"LINE202");
      return 0;
   }
   else
   {
      Prn::print(0,"LINE203");
      Prn::print(Prn::SerialRun1, "SerialPort::doSendBytes ERROR, %d", GetLastError());
      return cRetCodeError;
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Send null terminated string

int SerialPort::doSendZString(char* aData)
{
   int tNumBytes = (int)strlen(aData);
   return doSendBytes(aData,tNumBytes);   
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
// receive until cr/lf termination 

int  SerialPort::doReceiveUntilCRLF(char *aData, int aMaxNumBytes)
{
   int  tStatus=0;
   int  tIndex = 0;
   int  tRxStatus=0;
   char tRxChar=0;
   char tRxCharLast=0;
   bool tGoing=true;

   aData[0]=0;

   // Loop to read single bytes, store them, and exit
   // when termination cr/lf is detected
   while ( isValid() && tGoing )
   {
      // Store last byte
      tRxCharLast = tRxChar;

      // Read one byte
      tRxStatus=doReceiveOne(&tRxChar);
      if (tRxStatus >= 0)
      { 
         // Read success
         // Store byte
         aData[tIndex]=tRxChar;
         tIndex++;

         if(tRxCharLast==13 && tRxChar==10)
         {
            // Terminator detected
            tGoing=false;
            aData[tIndex-1]=0;
            tStatus=tIndex-2;
         }
         if(tIndex==aMaxNumBytes-1)
         {
            // NumBytes limit was reached
            tGoing=false;
            aData[tIndex]=0;
            tStatus = tIndex;
         }
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
// receive until cr termination 

int  SerialPort::doReceiveUntilCR(char *aData, int aMaxNumBytes)
{
   int  tStatus=0;
   int  tIndex = 0;
   int  tRxStatus=0;
   char tRxChar=0;
   bool tGoing=true;

   aData[0]=0;

   // Loop to read single bytes, store them, and exit
   // when termination cr/lf is detected
   while ( isValid() && tGoing )
   {
      // Read one byte
      tRxStatus = doReceiveOne(&tRxChar);
      if (tRxStatus >= 0)
      { 
         // Read success
         // Store byte
         aData[tIndex] = tRxChar;
         tIndex++;

         // If CR
         if(tRxChar==13)
         {
            // Terminator detected
            tGoing = false;
            aData[tIndex]=0;
            tStatus = tIndex-1;
         }
         if(tIndex == aMaxNumBytes-1)
         {
            // NumBytes limit was reached
            tGoing = false;
            aData[tIndex] = 0;
            tStatus = tIndex;
         }
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
// Receive one byte

int  SerialPort::doReceiveOne(char *aData)
{
   return doReceiveBytes(aData, 1);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Receive

int SerialPort::doReceiveBytes(char *aData, int aNumBytes)
{
   //---------------------------------------------------------------------------
   // Locals

   DWORD tBytesRead  = 0;
   int tBytesTotal = 0;

   //---------------------------------------------------------------------------
   // Read from port.

   ReadFile(mPortHandle, aData, aNumBytes, &tBytesRead, NULL);

   // If read error:
   if (GetLastError() != ERROR_SUCCESS)
   {
     // Error in communications
     Prn::print(Prn::SerialRun1,"SerialPort::doReceiveBytes ERROR %d", GetLastError());

     // If the read was aborted then clear hardware error.
     if (GetLastError()==ERROR_OPERATION_ABORTED)
     {
        ClearCommError(mPortHandle,0,0);
     }

     return cRetCodeError;
   }
    
   if (tBytesRead != aNumBytes)
   {
      return cRetCodeTimeout;
   }

   return 0;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace

