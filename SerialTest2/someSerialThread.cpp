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
   mTxBuffer[0] = 0;
   mErrorCount = 0;
   mRestartCount = 0;
   mRxCount = 0;
   mTxCount = 0;
   mRxReqNumBytes = gSerialParms.mRxReqNumBytes;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Thread init function. This is called by the base class immediately
// after the thread starts running. It initializes the serial port.

void SerialThread::threadInitFunction()
{
   // Serial port settings.
   mSettings.reset();
   mSettings.setPortDevice(gSerialParms.mSerialPortDevice);
   mSettings.setPortSetup(gSerialParms.mSerialPortSetup);

   // Initialize the serial port.
   mSerialPort.initialize(mSettings);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Thread run function. This is called by the base class immediately
// after the thread init function. It runs a loop that blocks on 
// serial port receives and then processes them. The loop terminates
// when the serial port receive is aborted.

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
      // Read bytes.

      // Read. 
      if (gSerialParms.mReadAllFlag)
      {
         tRet = mSerialPort.doReceiveAllBytes(mRxBuffer, mRxReqNumBytes);
      }
      else
      {
         tRet = mSerialPort.doReceiveAnyBytes(mRxBuffer, cBufferSize);
      }

      // Check the return code.
      if (tRet == 0)
      {
         Prn::print(Prn::Show1, "Serial read EMPTY");
         goto restart;
      }
      else if (tRet == Ris::cSerialRetError)
      {
         Prn::print(Prn::Show1, "Serial read ERROR");
         goto restart;
      }
      else if (tRet == Ris::cSerialRetTimeout)
      {
         Prn::print(Prn::Show1, "Serial read TIMEOUT");
         goto restart;
      }
      else if (tRet == Ris::cSerialRetAbort)
      {
         Prn::print(Prn::Show1, "Serial read ABORT");
         goto end;
      }
      // Process the read.
      else
      {
         // Number of bytes.
         mRxCount = tRet;
         bool tRxTest = true;
         for (int i = 0; i < mRxCount; i++)
         {
            if (mRxBuffer[i] != i % 37) tRxTest = false;
         }
         if (tRxTest)
         {
            Prn::print(Prn::Show1, "Serial read  $$$    %5d PASS", mRxCount);
         }
         else
         {
            Prn::print(Prn::Show1, "Serial read  $$$    %5d", mRxCount);
         }
         // Show.
      }
   }
   
   // Done.
end:
   return;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Thread exit function. This is called by the base class immediately
// before the thread is terminated. It is close the serial port.

void SerialThread::threadExitFunction()
{
   printf("someSerialThread::threadExitFunction\n");

   // Close the serial port.
   mSerialPort.doClose();
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Thread shutdown function. This is called out of the context of
// this thread. It aborts the serial port receive and waits for the
// thread to terminate after execution of the thread exit function.

void SerialThread::shutdownThread()
{
   printf("someSerialThread::shutdownThread\n");

   // Abort pending serial port receives
   mSerialPort.doAbort();

   // Wait for thread to terminate.
   BaseClass::shutdownThread();
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Abort a pending receive.

void SerialThread::abort()
{
   mSerialPort.doAbort();
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Send bytes via the serial port. This executes in the context of
// the calling thread.

void SerialThread::sendBytes(const void* aBytes, int aNumBytes)
{
   // Guard.
   if (!mSerialPort.mValidFlag) return;
   int tRet = 0;

   // Send a fixed number of bytes. Return the actual number of bytes
   // sent or a negative error code.
   tRet = mSerialPort.doSendBytes((char*)aBytes, aNumBytes);

   // Test the return code.
   if (tRet < 0)
   {
      Prn::print(Prn::Show1, "Serial write FAIL 101 %d", errno);
      return;
   }
   mTxCount = aNumBytes;

   // Show.
   Prn::print(Prn::Show1, "Serial write $$$$$$ %d", mTxCount);

   return;

}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Send test bytes via the serial port. This executes in the context
// of the calling thread.

void SerialThread::sendTestBytes(int aNumBytes)
{
   // Fill the transmit buffer with constants.
   for (int i = 0; i < aNumBytes; i++)
   {
      mTxBuffer[i] = i % 37;
   }

   // Send the transmit buffer.
   return sendBytes(mTxBuffer, aNumBytes);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace