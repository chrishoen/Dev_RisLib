//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "stdafx.h"

#include <windows.h>

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
   HANDLE mCommCompletion;
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

   mSpecific->mReadCompletion = CreateEvent(NULL, TRUE, FALSE, NULL);
   mSpecific->mWriteCompletion = CreateEvent(NULL, TRUE, FALSE, NULL);
   mSpecific->mCommCompletion = CreateEvent(NULL, TRUE, FALSE, NULL);
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
   mSpecific->mPortHandle = INVALID_HANDLE_VALUE;
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
}//namespace

