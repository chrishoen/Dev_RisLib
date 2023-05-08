//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "stdafx.h"

#include <windows.h>

#include "my_functions.h"
#include "risPortableCalls.h"
#include "prnPrint.h"
#include "trcTrace.h"

#include "risSerialPort_win.h"

namespace Ris
{

//******************************************************************************
//******************************************************************************
//******************************************************************************

SerialPort::SerialPort()
{
   mOpenFlag = false;
   mValidFlag = false;
   mAbortFlag = false;
   mTI = 0;
   mOpenErrorShowCount = 0;
   mCloseErrorShowCount = 0;
   mRxByteCount = 0;
   mTxByteCount = 0;
   mPortErrorCount = 0;

   mPortHandle = 0;
   mReadCompletion = 0;
   mWriteCompletion = 0;
   mCommCompletion = 0;
   memset(&mReadOverlapped, 0, sizeof(mReadOverlapped));
   memset(&mWriteOverlapped, 0, sizeof(mWriteOverlapped));
   memset(&mCommOverlapped, 0, sizeof(mCommOverlapped));
   mReadPending = false;
   mWritePending = false;
   mCommPending = false;
   mFirstReadFlag = false;
}

SerialPort::~SerialPort(void)
{
   //doClose();
}

void SerialPort::initialize(SerialSettings& aSettings)
{
   mSettings = aSettings;
   mTI = aSettings.mTraceIndex;
   mOpenErrorShowCount = 0;
   mCloseErrorShowCount = 0;

   mReadCompletion = CreateEvent(NULL, TRUE, FALSE, NULL);
   mWriteCompletion = CreateEvent(NULL, TRUE, FALSE, NULL);
   mCommCompletion = CreateEvent(NULL, TRUE, FALSE, NULL);
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

   mPortHandle = CreateFile(mSettings.mPortDevice,
      GENERIC_READ | GENERIC_WRITE,
      0,
      0,
      OPEN_EXISTING,
      FILE_FLAG_OVERLAPPED,
      0);

   if (mPortHandle == INVALID_HANDLE_VALUE)
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
   SetupComm(mPortHandle,0x20000, 0x20000);

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

      GetCommState(mPortHandle, &dcb);

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
         if (SetCommState(mPortHandle, &dcb))
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
               CloseHandle(mPortHandle);
               mPortHandle = INVALID_HANDLE_VALUE;
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

   if(!SetCommTimeouts(mPortHandle, &tCommTimeouts))
   {
      CloseHandle(mPortHandle);
      mPortHandle=INVALID_HANDLE_VALUE;
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
   mFirstReadFlag = true;
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
   if (mPortHandle == INVALID_HANDLE_VALUE)
   {
      mPortErrorCount++;
      Trc::write(mTI, 0, "SerialPort::doClose INVALID HANDLE");
      return;
   }

   // Flush.
   doFlush();

   // Unblock any pending writes.
   SetEvent(mWriteCompletion);

   // Cancel any pending i/o.
   CancelIoEx(mPortHandle, 0);

   // Close the handles.
   CloseHandle(mPortHandle);
   mPortHandle = INVALID_HANDLE_VALUE;
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
   SetEvent(mReadCompletion);
   Trc::write(mTI, 0, "SerialPort::doAbort done");
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Flush serial port buffers.

void SerialPort::doFlush()
{
   Trc::write(mTI, 0, "SerialPort::doFlush");

   ClearCommError(mPortHandle,0,0);

   DWORD lFlags = 
      PURGE_RXABORT | PURGE_RXCLEAR |
      PURGE_TXABORT | PURGE_TXCLEAR;

   PurgeComm(mPortHandle, lFlags);

   ClearCommError(mPortHandle, 0, 0);

   Trc::write(mTI, 0, "SerialPort::doFlush done");
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace

