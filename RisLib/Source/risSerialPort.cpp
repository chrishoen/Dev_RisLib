//******************************************************************************
//******************************************************************************
//******************************************************************************
#include "stdio.h"
#include "prnPrint.h"
#include "risCallPointer.h"

#define  WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "risSerialPort.h"

namespace Ris
{

//******************************************************************************

SerialPort::SerialPort(int aPortNumber,char* aPortSetup)
{
   mPortNumber = aPortNumber;
   strcpy(mPortSetup,aPortSetup);
   mValidFlag=false;
}

SerialPort::~SerialPort(void)
{
   close();
}

//******************************************************************************

void SerialPort::initialize()
{
   Prn::print(Prn::SerialInit,Prn::Init1,"SerialPort::SerialPort BEGIN");

   if (mValidFlag)
   {
      close();
   }

   //--------------------------------------------------------------------------
   // Create file
   char tSerialPortName[24];
   sprintf(tSerialPortName, "COM%u", mPortNumber);

   mHandle = CreateFile(tSerialPortName,  
     GENERIC_READ | GENERIC_WRITE, 
     0, 
     0, 
     OPEN_EXISTING,
     0,
     0);

   if (mHandle==INVALID_HANDLE_VALUE)
   {
      Prn::print(Prn::SerialRun,0,"serial_create_error_1 %d", GetLastError());
      return;
   }

   SetupComm(mHandle,2048,2048);
   //--------------------------------------------------------------------------
   // Purge 

   Sleep(100);

   purge();

   if (GetLastError() != ERROR_SUCCESS)
   {
      Prn::print(Prn::SerialRun,0,"serial_create_error_2 %d", GetLastError());
      CloseHandle(mHandle);
      mHandle=INVALID_HANDLE_VALUE;
      return;
   }
 
   Sleep(100);

   //--------------------------------------------------------------------------
   // DCB

   DCB dcb;

   memset(&dcb, 0, sizeof(dcb));
   dcb.DCBlength = sizeof(dcb);

   GetCommState(mHandle, &dcb);

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
      if (SetCommState(mHandle, &dcb))
      {
         // Successful, exit loop
         going=false;
      }
      else
      {
         // Failed, continue to retry
         Prn::print(Prn::SerialInit,Prn::Init1,"serial_create_retry %d", GetLastError());
         purge();
         Sleep(100);
         // Retry failed, abort initialization
         if(count++ == 10)
         {
            Prn::print(Prn::SerialRun,0,"serial_create_error_3 %d", GetLastError());
            CloseHandle(mHandle);
            mHandle=INVALID_HANDLE_VALUE;
            return;
         }
      }
   }

   //--------------------------------------------------------------------------
   // Set timeout

   COMMTIMEOUTS tComTimeout={0};

   tComTimeout.ReadIntervalTimeout         = 4000;
   tComTimeout.ReadTotalTimeoutMultiplier  = 1;
   tComTimeout.ReadTotalTimeoutConstant    = 4000;
   tComTimeout.WriteTotalTimeoutMultiplier = 0;
   tComTimeout.WriteTotalTimeoutConstant   = 0;

   if(!SetCommTimeouts(mHandle, &tComTimeout))
   {
      Prn::print(Prn::SerialRun,0,"serial_create_error_4 %d", GetLastError());
      CloseHandle(mHandle);
      mHandle=INVALID_HANDLE_VALUE;
      return;
   }

   //--------------------------------------------------------------------------
   // Purge 

   Sleep(100);
   purge();

   //--------------------------------------------------------------------------
   // Done
 
   mValidFlag=true;

   Prn::print(Prn::SerialInit,Prn::Init1,"SerialPort::SerialPort END");
}

//******************************************************************************

void SerialPort::close()
{
   if (mValidFlag)
   {
      CloseHandle(mHandle);
      mHandle = INVALID_HANDLE_VALUE;
      mValidFlag = false;
   } 
}

//******************************************************************************

bool SerialPort::isValid()
{
   return mValidFlag;
}

//******************************************************************************
// Send fixed number of bytes

int SerialPort::send(char const *aData, int aSize)
{
   DWORD tNumWritten;

   if (!isValid()) return RetCodeError;

   if (WriteFile(mHandle, aData, aSize, &tNumWritten, NULL))
   {
      return 0;
   }
   else
   {
      Prn::print(Prn::SerialInit,Prn::Init1, "Write Failed, %d", GetLastError());
      return RetCodeError;
   }
}

//******************************************************************************
// Send null terminated string

int SerialPort::send(char const *aData)
{
   int tSize = (int)strlen(aData);
   return send(aData,tSize);   
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Receive

//******************************************************************************
// Receive Data

int SerialPort::receiveData(char *aData, int aSize, int aTimeout)
{
   DWORD tBytesRead  = 0;
   int tBytesTotal = 0;

   ReadFile(mHandle, aData, aSize, &tBytesRead, NULL);
   if (tBytesRead != aSize)
   {
      return RetCodeError;
   }

   if (GetLastError() != ERROR_SUCCESS)
   {
     // Error in communications
     Prn::print(Prn::SerialRun,0,"serial_receive_error_1 %d", GetLastError());

     // If the read was aborted then clear hardware error
     if (GetLastError()==ERROR_OPERATION_ABORTED)
     {
        ClearCommError(mHandle,0,0);
     }

     return RetCodeError;
   }
    
   return 0;
}

//******************************************************************************
// receive until cr/lf termination 

int  SerialPort::receiveUntilCrLf(char *aData, int aMaxSize,int aTimeout)
{
   int  status=0;
   int  index = 0;
   int  rxStat=0;
   char rxChar=0;
   char rxCharLast=0;
   bool going=true;

   aData[0]=0;

   // Loop to read single bytes, store them, and exit
   // when termination cr/lf is detected
   while ( isValid() && going )
   {
      // Store last byte
      rxCharLast = rxChar;

      // Read one byte
      rxStat=receiveOne(&rxChar,aTimeout);
      if (rxStat >= 0)
      { 
         // Read success
         // Store byte
         aData[index]=rxChar;
         index++;

         if(rxCharLast==13 && rxChar==10)
         {
            // Terminator detected
            going=false;
            aData[index-1]=0;
            status=index-2;
         }
         if(index==aMaxSize-1)
         {
            // Size limit was reached
            going=false;
            aData[index]=0;
            status = index;
         }
      }
      else
      {  
         // Read failure
         status = rxStat;
         going = false;
      }
   }
   return status;
}

//******************************************************************************
// Receive fixed number of bytes

int  SerialPort::receiveOne(char *aData, int aTimeout)
{
   return receiveData(aData, 1, aTimeout);
}


//******************************************************************************
// Purge the comm channel

void SerialPort::purge()
{
   ClearCommError(mHandle,0,0);

   DWORD lFlags;

   lFlags = 
      PURGE_RXABORT | PURGE_RXCLEAR |
      PURGE_TXABORT | PURGE_TXCLEAR;

   PurgeComm(mHandle, lFlags);

}

}//namespace

