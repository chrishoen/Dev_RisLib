/*==============================================================================
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************
#include "stdafx.h"

#include "someSerialParms.h"

#define  _SOMESERIALTHREAD_CPP_
#include "someSerialThread.h"

//******************************************************************************
//******************************************************************************
//******************************************************************************

namespace Some
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Constructor.

SerialThread::SerialThread()
{
   // Set base class thread services.
   BaseClass::setThreadName("Serial");
   BaseClass::setThreadPriorityHigh();

   // Set base class thread priority.
   BaseClass::setThreadPriorityHigh();

   // Initialize variables.
   mRxBuffer[0] = 0;
   mErrorCount = 0;
   mRestartCount = 0;
   mRxCount = 0;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Thread init function. This is called by the base class immediately
// after the thread starts running. It initializes something.

void SerialThread::threadInitFunction()
{
   // Serial port settings.
   mSettings.reset();
   mSettings.setPortDevice(gSerialParms.mSerialPortDevice);
   mSettings.setPortSetup(gSerialParms.mSerialPortDevice);
   mSettings.mRxTimeout = 2000;

   // Initialize and open the serial port.
   mSerialPort.initialize(mSettings);
   mSerialPort.doOpen();

}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Thread run function. This is called by the base class immediately
// after the thread init function. It runs a loop that waits for the
// hid keyboard input.

void SerialThread::threadRunFunction()
{
   // Top of the loop.
   mRestartCount = 0;
restart:
   // Guard.
   if (mTerminateFlag) return;
   int tRet = 0;

   // Sleep.
   if (mRestartCount > 0)
   {
      BaseClass::threadSleep(1000);
   }
   Prn::print(Prn::Show1, "Serial restart %d", mRestartCount);
   mRestartCount++;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Open device.

   // Open the serial port.
   if (!mSerialPort.doOpen())
   {
      // If error then restart.
      goto restart;
   }

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Loop to receive strings.

   while (!BaseClass::mTerminateFlag)
   {
      Prn::print(Prn::Show4, "Serial read start********************************************** %d", mRxCount++);

      //************************************************************************
      //************************************************************************
      //************************************************************************
      // Read a string.

      // Read a string. 
      tRet = mSerialPort.doReadAnyBytes(mRxBuffer, 32);
      if (tRet == 0)
      {
         Prn::print(Prn::Show1, "Serial read EMPTY");
         goto restart;
      }
      else if (tRet == Ris::cSerialRetError)
      {
         Prn::print(Prn::Show1, "Serial read EMPTY");
         goto restart;
      }
      else if (tRet == Ris::cSerialRetAbort)
      {
         Prn::print(Prn::Show1, "Serial read ABORT");
         goto end;
      }
      // Null terminate.
      mRxBuffer[tRet] = 0;

      // Print.
      Prn::print(Prn::Show1, "Serial read  $$$    %2d %d %s",
         tRet, my_trimCRLF(mRxBuffer), mRxBuffer);

   }
   
   // Done.
end:
   return;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Thread exit function. This is called by the base class immediately
// before the thread is terminated. It shuts down the hid api.

void SerialThread::threadExitFunction()
{
   printf("someSerialThread::threadExitFunction\n");

   // Close the serial port.
   mSerialPort.doClose();
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Thread shutdown function. This posts to the close event to
// terminate the thread and it closes the files.

void SerialThread::shutdownThread()
{
   printf("someSerialThread::shutdownThread\n");

   // Request thread run function return.
   mTerminateFlag = true;

   // Abort pending serial port receives
   mSerialPort.doAbort();
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Send a null terminated string via the serial port. This executes in the
// context of the calling thread.

void SerialThread::sendString(const char* aString)
{
   // Guard.
   if (!mSerialPort.mValidFlag) return;
   int tRet = 0;

   // Local variables.
   int tNumBytes = strlen(aString);

   // Write bytes to the port.
   tRet = mSerialPort.doSendBytes(aString, tNumBytes);

   // Test the return code.
   if (tRet < 0)
   {
      Prn::print(Prn::Show1, "Serial write FAIL 101 %d", errno);
      return;
   }
   if (tRet != tNumBytes)
   {
      Prn::print(Prn::Show1, "Serial write FAIL 102");
      return;
   }

   // Print.
   char tTxBuffer[100];
   strcpy(tTxBuffer, aString);
   Prn::print(Prn::Show1, "Serial write $$$$$$ %2d %d %s",
      tNumBytes, my_trimCRLF(tTxBuffer), tTxBuffer);

   return;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Software tests.

void SerialThread::test1()
{
   mSerialPort.doClose();
}

void SerialThread::abort()
{
   mSerialPort.doAbort();
}
//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace